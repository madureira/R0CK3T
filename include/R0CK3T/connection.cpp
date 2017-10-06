#include "connection.h"
#include "connection_manager.h"

namespace R0CK3T {

	Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager, RequestHandler& handler)
		: m_socket(std::move(socket)), m_connectionManager(manager), m_requestHandler(handler)
	{
	}

	void Connection::start()
	{
		read();
	}

	void Connection::stop()
	{
		m_socket.close();
	}

	void Connection::read()
	{
		auto self(shared_from_this());
		m_socket.async_read_some(boost::asio::buffer(m_buffer), [this, self](boost::system::error_code errorCode, std::size_t bytesTransferred)
		{
			if (!errorCode)
			{
				RequestParser::ResultType result;
				std::tie(result, std::ignore) = m_requestParser.parse(m_httpRequest, m_buffer.data(), m_buffer.data() + bytesTransferred);

				if (result == RequestParser::GOOD)
				{
					m_requestHandler.handleRequest(m_httpRequest, m_httpResponse);
					write();
				}
				else if (result == RequestParser::BAD)
				{
					m_httpResponse = HttpResponse::stockReply(HttpResponse::bad_request);
					write();
				}
				else
				{
					read();
				}
			}
			else if (errorCode != boost::asio::error::operation_aborted)
			{
				m_connectionManager.stop(shared_from_this());
			}
		});
	}

	void Connection::write()
	{
		auto self(shared_from_this());
		boost::asio::async_write(m_socket, m_httpResponse.toBuffers(), [this, self](boost::system::error_code errorCode, std::size_t)
		{
			if (!errorCode)
			{
				boost::system::error_code ignoredErrorCode;
				m_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignoredErrorCode);
			}

			if (errorCode != boost::asio::error::operation_aborted)
			{
				m_connectionManager.stop(shared_from_this());
			}
		});
	}

}
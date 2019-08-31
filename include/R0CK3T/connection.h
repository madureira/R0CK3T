#pragma once

#include <array>
#include <memory>
#include <iostream>
#include <boost/asio.hpp>
#include "http_request.h"
#include "http_response.h"
#include "request_parser.h"
#include "request_handler.h"

namespace R0CK3T {

	class ConnectionManager;

	class Connection : public std::enable_shared_from_this<Connection>
	{
	public:
		Connection(const Connection&) = delete;
		Connection& operator=(const Connection&) = delete;

		explicit Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager, RequestHandler& handler);

		void start();

		void stop();

	private:
		void read();

		void write();

		boost::asio::ip::tcp::socket m_socket;

		ConnectionManager& m_connectionManager;

		RequestHandler& m_requestHandler;

		std::array<char, 8192> m_buffer;

		RequestParser m_requestParser;

		HttpRequest m_httpRequest;

		HttpResponse m_httpResponse;
	};

}
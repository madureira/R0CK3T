#include "server.h"

namespace R0CK3T {

	Server::Server(const std::string& address, const std::string& port, const std::string& documentRoot)
		: m_ioService(), m_signals(m_ioService), m_acceptor(m_ioService), m_connectionManager(), m_socket(m_ioService), m_requestHandler(documentRoot)
	{
		m_app = std::make_shared<App>();
		m_requestHandler.addApp(m_app);

		m_signals.add(SIGINT);
		m_signals.add(SIGTERM);
#if defined(SIGQUIT)
		m_signals.add(SIGQUIT);
#endif

		awaitStop();

		boost::asio::ip::tcp::resolver resolver(m_ioService);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({ address, port });
		m_acceptor.open(endpoint.protocol());
		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		m_acceptor.bind(endpoint);
		m_acceptor.listen();

		accept();
	}

	std::shared_ptr<App> Server::app()
	{
		return m_app;
	}

	void Server::run()
	{
		std::cout << "R0CK3T running..." << std::endl;
		m_ioService.run();
	}

	void Server::accept()
	{
		m_acceptor.async_accept(m_socket, [this](boost::system::error_code errorCode)
		{
			if (!m_acceptor.is_open())
			{
				return;
			}

			if (!errorCode)
			{
				m_connectionManager.start(std::make_shared<Connection>(std::move(m_socket), m_connectionManager, m_requestHandler));
			}

			accept();
		});
	}

	void Server::awaitStop()
	{
		m_signals.async_wait([this](boost::system::error_code /*ec*/, int /*signo*/)
		{
			m_acceptor.close();
			m_connectionManager.stopAll();
		});
	}

}
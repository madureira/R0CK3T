#include "server.h"

namespace R0CK3T {

	Server::Server(std::string configFile)
		: m_ioService(), m_signals(m_ioService), m_acceptor(m_ioService), m_connectionManager(), m_socket(m_ioService), m_requestHandler("")
	{
		std::string slash(1, boost::filesystem::path::preferred_separator);

		if (configFile.empty())
		{
			configFile = boost::filesystem::initial_path().string() + slash + "example\\config.json";
		}

		Settings settings;
		std::ifstream file(configFile);

		if (file.is_open())
		{
			json jsonObj;
			file >> jsonObj;

			if (!jsonObj.is_null())
			{
				settings.address = jsonObj.at("address").get<std::string>();
				settings.port = jsonObj.at("port").get<std::string>();
				settings.templatesPath = jsonObj.at("templatesPath").get<std::string>();
			}
		}

		Config::getInstance().setSettings(settings);

		m_app = std::make_shared<App>();
		m_requestHandler.addApp(m_app);

		m_signals.add(SIGINT);
		m_signals.add(SIGTERM);
#if defined(SIGQUIT)
		m_signals.add(SIGQUIT);
#endif
		awaitStop();

		boost::asio::ip::tcp::resolver resolver(m_ioService);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({ settings.address, settings.port });
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
		std::cout << "Listen at: " << Config::getInstance().setting().address << ":" << Config::getInstance().setting().port << std::endl;

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
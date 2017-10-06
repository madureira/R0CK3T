#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <memory>
#include "connection.h"
#include "connection_manager.h"
#include "request_handler.h"
#include "app.h"

namespace R0CK3T {

	class Server
	{
	public:
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		explicit Server(const std::string& address, const std::string& port, const std::string& documentRoot);

		std::shared_ptr<App> app();

		void run();

	private:
		void accept();
		void awaitStop();
		
		ConnectionManager m_connectionManager;
		RequestHandler m_requestHandler;
		std::shared_ptr<App> m_app;

		boost::asio::io_service m_ioService;
		boost::asio::signal_set m_signals;
		boost::asio::ip::tcp::acceptor m_acceptor;
		boost::asio::ip::tcp::socket m_socket;
	};

}
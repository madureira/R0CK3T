#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/asio.hpp>
#include "vendors/nlohmann/json.hpp"
#include "connection.h"
#include "connection_manager.h"
#include "request_handler.h"
#include "app.h"
#include "config.h"

namespace R0CK3T {

	using json = nlohmann::json;

	class Server
	{
	public:
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;

		explicit Server(std::string configFile = "");

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
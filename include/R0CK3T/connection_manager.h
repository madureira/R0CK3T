#pragma once

#include <set>
#include "connection.h"

namespace R0CK3T {

	class ConnectionManager
	{
	public:
		ConnectionManager(const ConnectionManager&) = delete;
		ConnectionManager& operator=(const ConnectionManager&) = delete;

		ConnectionManager();

		void start(std::shared_ptr<Connection> connection);

		void stop(std::shared_ptr<Connection> connection);

		void stopAll();

	private:
		std::set<std::shared_ptr<Connection>> m_connections;
	};

}
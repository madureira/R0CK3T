#include "connection_manager.h"

namespace R0CK3T {

	ConnectionManager::ConnectionManager()
	{
	}

	void ConnectionManager::start(std::shared_ptr<Connection> connection)
	{
		m_connections.insert(connection);
		connection->start();
	}

	void ConnectionManager::stop(std::shared_ptr<Connection> connection)
	{
		m_connections.erase(connection);
		connection->stop();
	}

	void ConnectionManager::stopAll()
	{
		for (auto connection : m_connections)
		{
			connection->stop();
		}
		m_connections.clear();
	}

}
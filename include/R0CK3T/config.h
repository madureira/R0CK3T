#pragma once

#include <string>
#include "vendors/nlohmann/json.hpp"

namespace R0CK3T {

	struct Settings
	{
		std::string address;
		std::string port;
		std::string templatesPath;
	};

	class Config
	{
	public:
		Config(Config const&) = delete;
		Config& operator=(Config const&) = delete;

		static Config& getInstance()
		{
			static Config instance;
			return instance;
		}

		void setSettings(Settings settings)
		{
			m_settings = settings;
		}

		Settings setting()
		{
			return m_settings;
		}

	private:
		Config() {}
		Settings m_settings;
	};

}
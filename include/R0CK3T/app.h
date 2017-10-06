#pragma once

#include "http_request.h"
#include <boost/algorithm/string/replace.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <map>

namespace R0CK3T {

	class App
	{
	public:
		App() { }
		~App() { }

		void route(std::string route, void* fn)
		{
			m_routesPtr[route] = fn;
		}

		void exec(std::string route, const HttpRequest& request)
		{
			auto search = m_routesPtr.find(route);

			if (search != m_routesPtr.end())
			{
				try {
					reinterpret_cast<void(*)(const HttpRequest&)> (search->second)(request);
				}
				catch(const std::exception& e)
				{
					std::cerr << "Error trying to execute route function" << std::endl;
					std::cerr << e.what() << std::endl;
				}
			}
			else
			{
				std::cerr << "Route no found!" << std::endl;
			}
		}

		std::string findResource(std::string resource)
		{
			std::string resourceMatch;

			for (auto res : m_routesPtr)
			{
				std::string url = boost::regex_replace(res.first, boost::regex("(:.[^/]+)"), "(.+[^/])");
				url = "^" + url + "$";

				if (boost::regex_search(resource, boost::regex(url, boost::regex::icase)))
				{
					std::cout << "URL: " << resource << " matchs resource: " << res.first << std::endl;
					resourceMatch = res.first;
					break;
				}
			}

			return resourceMatch;
		}

		void extractParamsFromUrl(std::string requestUrl, std::string resource, HttpRequest& request)
		{
			request.urlParams[":productId"] = "123";
		}

	private:
		std::map<std::string, void*> m_routesPtr;
	};

}
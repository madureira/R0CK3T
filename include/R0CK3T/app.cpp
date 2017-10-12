#include "app.h"

namespace R0CK3T {

	App::App() {
	}

	App::~App() {
		// clear pointers
	}

	void App::route(std::string route, void* fn)
	{
		route = removeLastSlash(route);
		m_routesPtr[route] = fn;
	}

	void App::exec(std::string route, const HttpRequest& request, HttpResponse& response)
	{
		auto search = m_routesPtr.find(route);

		if (search != m_routesPtr.end())
		{
			try {
				reinterpret_cast<void(*)(const HttpRequest&, HttpResponse&)> (search->second)(request, response);
			}
			catch (const std::exception& e)
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

	std::string App::findResource(std::string resource)
	{
		resource = removeLastSlash(resource);

		std::string resourceMatch;

		for (auto res : m_routesPtr)
		{
			std::string url = boost::regex_replace(res.first, boost::regex("(:.[^/]+)"), "(.[^/]+)");
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

	void App::extractParamsFromUrl(std::string requestUrl, std::string resource, HttpRequest& request)
	{
		std::string::const_iterator begin = resource.begin();
		std::string::const_iterator end = resource.end();
		boost::match_results<std::string::const_iterator> keyMatches;

		std::vector<std::string> foundKeys;
		
		while (boost::regex_search(begin, end, keyMatches, boost::regex("(:.[^/]*)")))
		{
			std::string key(keyMatches[1].first, keyMatches[2].second);
			boost::replace_all(key, ":", "");
			request.urlParams[key] = "";
			foundKeys.push_back(key);
			begin = keyMatches[0].second;
		}

		if (foundKeys.size() > 0)
		{
			boost::smatch valueMatches;

			std::string resourceToMatch = boost::regex_replace(resource, boost::regex("(:.[^/]+)"), "(.[^/]+)");

			if (boost::regex_search(requestUrl, valueMatches, boost::regex(resourceToMatch, boost::regex::icase)))
			{
				int keyIdx = 0;
				for (int i = 1; i < valueMatches.size(); i++)
				{
					std::string value(valueMatches[i].first, valueMatches[i].second);
					request.urlParams[foundKeys.at(keyIdx)] = value;
					keyIdx++;
				}
			}
		}
	}

	std::string App::removeLastSlash(std::string uri)
	{
		if (uri.length() > 0) {
			std::string::iterator it = uri.end() - 1;

			if (*it == '/')
			{
				uri.erase(it);
			}
		}

		return uri;
	}

}
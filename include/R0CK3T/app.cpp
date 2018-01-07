#include "app.h"

namespace R0CK3T {

	App::App() {
	}

	App::~App() {
		m_GETs.clear();
		m_POSTs.clear();
		m_PUTs.clear();
		m_DELETEs.clear();
	}

	void App::get(std::string route, void* fn)
	{
		m_GETs[removeLastSlash(route)] = fn;
	}

	void App::post(std::string route, void* fn)
	{
		m_POSTs[removeLastSlash(route)] = fn;
	}

	void App::put(std::string route, void* fn)
	{
		m_PUTs[removeLastSlash(route)] = fn;
	}

	void App::del(std::string route, void* fn)
	{
		m_DELETEs[removeLastSlash(route)] = fn;
	}

	void App::exec(std::string route, const HttpRequest& request, HttpResponse& response)
	{
		std::map<std::string, void*>* resourcesPtrs = findResourcesByMethod(request.method);

		auto search = resourcesPtrs->find(route);

		if (search != resourcesPtrs->end())
		{
			try {
				reinterpret_cast<void(*)(const HttpRequest&, HttpResponse&)> (search->second)(request, response);
			}
			catch (const std::exception& e)
			{
				std::cerr << "Error: Trying to execute route function" << std::endl;
				std::cerr << e.what() << std::endl;
			}
		}
		else
		{
			std::cerr << "Route no found!" << std::endl;
		}
	}

	std::string App::findResource(std::string resource, std::string httpMethod)
	{
		resource = removeLastSlash(resource);

		std::string resourceMatch;

		std::map<std::string, void*>* resourcesPtrs = findResourcesByMethod(httpMethod);

		for (auto res : *resourcesPtrs)
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
		if (uri.length() > 1) {
			std::string::iterator it = uri.end() - 1;
			if (*it == '/')
				uri.erase(it);
		}
		return uri;
	}

	std::map<std::string, void*>* App::findResourcesByMethod(std::string httpMethod)
	{
		std::map<std::string, void*>* resourcesPtrs = nullptr;

		if (httpMethod == "GET")
		{
			resourcesPtrs = &m_GETs;
		}
		else if (httpMethod == "POST")
		{
			resourcesPtrs = &m_POSTs;
		}
		else if (httpMethod == "PUT")
		{
			resourcesPtrs = &m_PUTs;
		}
		else if (httpMethod == "DELETE")
		{
			resourcesPtrs = &m_DELETEs;
		}

		return resourcesPtrs;
	}

}
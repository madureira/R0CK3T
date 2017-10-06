#pragma once

#include <boost/algorithm/string/replace.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <vector>

namespace R0CK3T {

	class Resources
	{
	public:
		Resources()
		{
		}

		std::string uri(std::string resource)
		{
			m_resourcesList.push_back(resource);
			return resource;
		}

		std::string findResource(std::string resource)
		{
			std::string resourceMatch;

			for (auto res : m_resourcesList)
			{
				std::string url = "^" + res + "$";
				boost::replace_all(url, "[int]", "(\\d+)");
				boost::replace_all(url, "[string]", "(.+[^/])");

				if (boost::regex_search(resource, boost::regex(url, boost::regex::icase)))
				{
					std::cout << "URL: " << resource << " matchs resource: " << res << std::endl;
					resourceMatch = res;
					break;
				}
			}

			return resourceMatch;
		}

	private:
		std::vector<std::string> m_resourcesList;
	};

}
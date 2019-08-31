#pragma once

#include <map>
#include <string>
#include <vector>
#include "header.h"

namespace R0CK3T {

	struct HttpRequest
	{
		std::string method;
		std::string uri;
		int http_version_major;
		int http_version_minor;
		std::vector<Header> headers;
		std::map<std::string, std::string> urlParams;

		std::string getUrlParam(std::string key)
		{
			auto found = urlParams.find(key);
			if (found != urlParams.end()) {
				return found->second;
			}
			return "";
		}
	};

}
#pragma once

#include "http_request.h"
#include "http_response.h"
#include <boost/algorithm/string/replace.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <map>

namespace R0CK3T {

	class App
	{
	public:
		App();
		~App();

		void route(std::string route, void* fn);
		void exec(std::string route, const HttpRequest& request, HttpResponse& response);
		std::string findResource(std::string resource);
		void extractParamsFromUrl(std::string requestUrl, std::string resource, HttpRequest& request);

	private:
		std::string removeLastSlash(std::string uri);

		std::map<std::string, void*> m_routesPtr;
	};

}
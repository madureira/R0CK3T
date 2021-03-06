#pragma once

#include <map>
#include <string>
#include <iostream>
#include <boost/regex.hpp>
#include <boost/algorithm/string/replace.hpp>
#include "http_request.h"
#include "http_response.h"

namespace R0CK3T {

	class App
	{
	public:
		App();
		~App();

		void get(std::string route, void* fn);
		void post(std::string route, void* fn);
		void put(std::string route, void* fn);
		void del(std::string route, void* fn);

		void exec(std::string route, const HttpRequest& request, HttpResponse& response);
		std::string findResource(std::string resource, std::string httpMethod);
		void extractParamsFromUrl(std::string requestUrl, std::string resource, HttpRequest& request);

	private:
		std::string removeLastSlash(std::string uri);
		std::map<std::string, void*>* findResourcesByMethod(std::string httpMethod);

		std::map<std::string, void*> m_GETs;
		std::map<std::string, void*> m_POSTs;
		std::map<std::string, void*> m_PUTs;
		std::map<std::string, void*> m_DELETEs;
	};

}
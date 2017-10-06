#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <memory>

namespace R0CK3T {

	class App;

	struct HttpResponse;
	struct HttpRequest;

	class RequestHandler
	{
	public:
		RequestHandler(const RequestHandler&) = delete;
		RequestHandler& operator=(const RequestHandler) = delete;

		explicit RequestHandler(const std::string& documentRoot);
		
		void addApp(std::shared_ptr<App> app);

		void handleRequest(HttpRequest& request, HttpResponse& response);

	private:
		std::string m_documentRoot;
		std::shared_ptr<App> m_app;
		static bool urlDecode(const std::string& input, std::string& output);
	};

}
#include "request_handler.h"
#include "mime_types.h"
#include "http_response.h"
#include "http_request.h"
#include "app.h"
#include <iostream>
#include <cstdarg>

namespace R0CK3T {

	RequestHandler::RequestHandler(const std::string& documentRoot) : m_documentRoot(documentRoot)
	{
	}

	void RequestHandler::addApp(std::shared_ptr<App> app)
	{
		m_app = app;
	}

	void RequestHandler::handleRequest(HttpRequest& request, HttpResponse& response)
	{
		std::string requestPath;
		if (!urlDecode(request.uri, requestPath))
		{
			response = HttpResponse::stockReply(HttpResponse::bad_request);
			return;
		}

		if (requestPath.empty() || requestPath[0] != '/' || requestPath.find("..") != std::string::npos)
		{
			response = HttpResponse::stockReply(HttpResponse::bad_request);
			return;
		}

		std::string resourceMatch = m_app->findResource(requestPath, request.method);

		if (!resourceMatch.empty())
		{
			m_app->extractParamsFromUrl(requestPath, resourceMatch, request);
			m_app->exec(resourceMatch, request, response);
		}
		else {
			if (requestPath[requestPath.size() - 1] == '/')
			{
				requestPath += "index.html";
			}

			std::size_t lastSlashPos = requestPath.find_last_of("/");
			std::size_t lastDotPos = requestPath.find_last_of(".");
			std::string extension;

			if (lastDotPos != std::string::npos && lastDotPos > lastSlashPos)
			{
				extension = requestPath.substr(lastDotPos + 1);
			}

			std::string fullPath = m_documentRoot + requestPath;
			std::ifstream is(fullPath.c_str(), std::ios::in | std::ios::binary);
			if (!is)
			{
				response = HttpResponse::stockReply(HttpResponse::not_found);
				return;
			}

			response.status = HttpResponse::ok;
			char buffer[512];
			while (is.read(buffer, sizeof(buffer)).gcount() > 0)
			{
				response.content.append(buffer, is.gcount());
			}
			response.headers.resize(2);
			response.headers[0].name = "Content-Length";
			response.headers[0].value = std::to_string(response.content.size());
			response.headers[1].name = "Content-Type";
			response.headers[1].value = mime_types::extensionToType(extension);
		}
	}

	bool RequestHandler::urlDecode(const std::string& input, std::string& output)
	{
		output.clear();
		output.reserve(input.size());
		for (std::size_t i = 0; i < input.size(); ++i)
		{
			if (input[i] == '%')
			{
				if (i + 3 <= input.size())
				{
					int value = 0;
					std::istringstream is(input.substr(i + 1, 2));
					if (is >> std::hex >> value)
					{
						output += static_cast<char>(value);
						i += 2;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else if (input[i] == '+')
			{
				output += ' ';
			}
			else
			{
				output += input[i];
			}
		}
		return true;
	}

}
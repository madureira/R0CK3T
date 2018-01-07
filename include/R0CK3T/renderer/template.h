#pragma once

#include "../http_response.h"
#include "../mime_types.h"
#include "../vendors/mstch/mstch.hpp"
#include "../config.h"
#include <string>
#include <fstream>
#include <regex>
#include <map>

namespace R0CK3T {
	
	class Template
	{
	public:
		Template(Template const&) = delete;
		Template& operator=(Template const&) = delete;

		static Template& instance()
		{
			static Template instance;
			return instance;
		}

		void render(std::string templateName, mstch::map data, HttpResponse& response)
		{
			instance().present(templateName, data, {}, response);
		}

		void render(std::string templateName, mstch::map data, std::map<std::string, std::string> partials, HttpResponse& response)
		{
			std::map<std::string, std::string> tmpls;

			for (auto const &templateFile : partials)
			{
				tmpls[templateFile.first] = instance().loadTemplate(templateFile.second);
			}

			instance().present(templateName, data, tmpls, response);
		}

	private:
		Template() {}

		std::map<std::string, std::string> m_templates;

		void present(std::string templateName, mstch::map data, std::map<std::string, std::string> partials, HttpResponse& response)
		{
			if (partials.empty())
			{
				response.content = mstch::render(loadTemplate(templateName), data);
			}
			else
			{
				response.content = mstch::render(loadTemplate(templateName), data, partials);
			}

			if (!response.hasStatus())
			{
				response.status = HttpResponse::ok;
			}

			response.headers.resize(2);
			response.headers[0].name = "Content-Length";
			response.headers[0].value = std::to_string(response.content.size());
			response.headers[1].name = "Content-Type";
			response.headers[1].value = mime_types::extensionToType("html");

		}

		std::string loadTemplate(std::string fileName)
		{
			if (m_templates.find(fileName) == m_templates.end()) {
				Settings settings = Config::getInstance().setting();

				std::string output;
				std::string fullPath = settings.templatesPath + fileName;
				std::ifstream is(fullPath.c_str(), std::ios::in | std::ios::binary);
				if (!is)
				{
					return "";
				}

				char buffer[512];
				while (is.read(buffer, sizeof(buffer)).gcount() > 0)
				{
					output.append(buffer, is.gcount());
				}

				m_templates[fileName] = output;

				return output;
			}
			else {
				return m_templates.at(fileName);
			}
		}
	};

}
#pragma once

#include <map>
#include <string>
#include <fstream>
#include "../vendors/mstch/mstch.hpp"

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

		std::string render(std::string templateName, mstch::map& data)
		{
			std::map<std::string, std::string> tmpls;
			return present(templateName, data, tmpls);
		}

		std::string render(std::string templateName, mstch::map& data, std::map<std::string, std::string>& partials)
		{
			std::map<std::string, std::string> tmpls;

			for (auto const& templateFile : partials)
			{
				tmpls[templateFile.first] = instance().loadTemplate(templateFile.second);
			}

			return instance().present(templateName, data, tmpls);
		}

	private:
		Template() {}

		std::map<std::string, std::string> m_templates;

		std::string present(std::string& templateName, mstch::map& data, std::map<std::string, std::string>& partials)
		{
			if (partials.empty())
			{
				return mstch::render(loadTemplate(templateName), data);
			}

			return mstch::render(loadTemplate(templateName), data, partials);
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
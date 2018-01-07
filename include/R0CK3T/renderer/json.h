#pragma once

#include "../vendors/nlohmann/json.hpp"
#include "../http_response.h"

using json = nlohmann::json;

namespace R0CK3T {

	class JSON
	{
	public:
		static void render(json data, HttpResponse& response)
		{
			if (!response.hasStatus())
			{
				response.status = HttpResponse::ok;
			}
			
			response.content = data.dump();
			response.headers.resize(2);
			response.headers[0].name = "Content-Length";
			response.headers[0].value = std::to_string(response.content.size());
			response.headers[1].name = "Content-Type";
			response.headers[1].value = "application/json";
		}
	};

}
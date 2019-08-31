#pragma once

#include "../../include/R0CK3T.h"

namespace example {

	using namespace R0CK3T;

	class HomeController {
	public:
		static void index(const HttpRequest& request, HttpResponse& response)
		{
			json data = { { { "id", 123 },{ "name", "C++ Book" } },{ { "id", 456 },{ "name", "Java Book" } } };

			response.json(data);
		}
	};

}
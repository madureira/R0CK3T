#pragma once

#include "../../include/R0CK3T.h"
#include <iostream>

namespace example {

	using namespace R0CK3T;

	class ProductController {
	public:
		static void index(const HttpRequest& request, HttpResponse& response)
		{
			std::cout << ">> List of products <<" << std::endl;
		}

		static void getById(HttpRequest& request, HttpResponse& response)
		{
			std::cout << "Id: " << request.getUrlParam("productId") << std::endl;
		}

		static void getByName(const HttpRequest& request, HttpResponse& response)
		{
			std::cout << "Name: " <<  std::endl;
		}
	};

}
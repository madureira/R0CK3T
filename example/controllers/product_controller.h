#pragma once

#include "../../include/R0CK3T.h"
#include <iostream>

namespace example {

	using namespace R0CK3T;

	class ProductController {
	public:
		static void index(const HttpRequest& request)
		{
			std::cout << ">> List of products <<" << std::endl;
		}

		static void getById(const HttpRequest& request)
		{
			std::cout << "Id: " << request.urlParams.at("productId") << std::endl;
		}

		static void getByName(const HttpRequest& request)
		{
			std::cout << "Name: " <<  std::endl;
		}
	};

}
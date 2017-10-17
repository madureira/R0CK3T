#pragma once

#include "../../include/R0CK3T.h"
#include <map>
#include <string>

namespace example {

	using namespace R0CK3T;

	class ProductController
	{
	public:
		static void index(const HttpRequest& request, HttpResponse& response)
		{
			mstch::map data { { "title", std::string{ "List!!!!!!" } } };
			Template::instance().render("products_list.tmpl", data, getPartials(), response);
		}

		static void getById(HttpRequest& request, HttpResponse& response)
		{
			mstch::map data{ { "productId", request.getUrlParam("productId") } };
			Template::instance().render("product.tmpl", data, getPartials(), response);
		}

	private:
		static std::map<std::string, std::string> getPartials()
		{
			return { { "header", "header.tmpl" },{ "footer", "footer.tmpl" } };
		}
	};

}
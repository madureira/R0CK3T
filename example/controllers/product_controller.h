#pragma once

#include "../../include/R0CK3T.h"

namespace example {

	using namespace R0CK3T;

	class ProductController
	{
	public:
		static void index(const HttpRequest& request, HttpResponse& response)
		{
			mstch::map data{ { "title", std::string{ "List!!!!!!" } } };
			auto html = Template::instance().render("products_list.tmpl", data, getPartials());

			response.send(html);
		}

		static void getById(HttpRequest& request, HttpResponse& response)
		{
			mstch::map data{ { "productId", request.getUrlParam("productId") } };
			auto html = Template::instance().render("product.tmpl", data, getPartials());

			response.send(html);
		}

	private:
		static std::map<std::string, std::string> getPartials()
		{
			return { { "header", "header.tmpl" },{ "footer", "footer.tmpl" } };
		}
	};

}
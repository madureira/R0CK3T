#include <boost/test/unit_test.hpp>
#include "../../include/R0CK3T/app.h"

namespace unit_test {

	using namespace R0CK3T;

	BOOST_AUTO_TEST_SUITE(AppTest)

	class DummyClass
	{
	public:
		static void dummyFunction() { }
		static void anotherDummyFunction() { }
	};

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource)
	{
		App app;
		app.route("/test", &DummyClass::dummyFunction);
		app.route("/test/:dummyParam", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test", app.findResource("/test"));
	}

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_with_url_param)
	{
		App app;
		app.route("/test", &DummyClass::dummyFunction);
		app.route("/test/:dummyParam", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test/:dummyParam", app.findResource("/test/123"));
	}

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_with_end_slash)
	{
		App app;
		app.route("/test/", &DummyClass::dummyFunction);
		app.route("/another", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test", app.findResource("/test"));
		BOOST_REQUIRE_EQUAL("/another", app.findResource("/another/"));
	}

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_with_multiple_params)
	{
		App app;
		app.route("/test/:id/name/:name/price/:price", &DummyClass::dummyFunction);
		app.route("/another/:id/product/:name", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test/:id/name/:name/price/:price", app.findResource("/test/123/name/something/price/22.50"));
		BOOST_REQUIRE_EQUAL("/another/:id/product/:name", app.findResource("/another/456/product/notebook/"));
	}

	BOOST_AUTO_TEST_CASE(should_extract_numeric_param_from_url)
	{
		HttpRequest request;

		App app;
		app.extractParamsFromUrl("/client/123", "/client/:id", request);

		BOOST_REQUIRE_EQUAL("123", request.getUrlParam("id"));
	}

	BOOST_AUTO_TEST_CASE(should_extract_text_param_from_url)
	{
		HttpRequest request;

		App app;
		app.extractParamsFromUrl("/another/test@test.com", "/another/:email", request);

		BOOST_REQUIRE_EQUAL("test@test.com", request.getUrlParam("email"));
	}

	BOOST_AUTO_TEST_CASE(should_extract_multiple_params_from_url)
	{
		HttpRequest request;

		App app;
		app.extractParamsFromUrl("/product/123/CppBook/price/35.99", "/product/:id/:name/price/:price", request);

		BOOST_REQUIRE_EQUAL("123", request.getUrlParam("id"));
		BOOST_REQUIRE_EQUAL("CppBook", request.getUrlParam("name"));
		BOOST_REQUIRE_EQUAL("35.99", request.getUrlParam("price"));
	}

	BOOST_AUTO_TEST_SUITE_END()

}
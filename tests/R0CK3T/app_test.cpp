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

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_get_method)
	{
		App app;
		app.get("/test", &DummyClass::dummyFunction);
		app.post("/test/:dummyParam", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test", app.findResource("/test", "GET"));
	}

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_post_method)
	{
		App app;
		app.post("/test", &DummyClass::dummyFunction);
		app.get("/test/:dummyParam", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test", app.findResource("/test", "POST"));
	}

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_put_method)
	{
		App app;
		app.get("/test", &DummyClass::dummyFunction);
		app.put("/test/:dummyParam", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test/:dummyParam", app.findResource("/test/123", "PUT"));
	}

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_with_url_param)
	{
		App app;
		app.get("/test", &DummyClass::dummyFunction);
		app.get("/test/:dummyParam", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test/:dummyParam", app.findResource("/test/123", "GET"));
	}

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_with_end_slash)
	{
		App app;
		app.get("/test/", &DummyClass::dummyFunction);
		app.get("/another", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test", app.findResource("/test", "GET"));
		BOOST_REQUIRE_EQUAL("/another", app.findResource("/another/", "GET"));
	}

	BOOST_AUTO_TEST_CASE(should_find_a_registered_resource_with_multiple_params)
	{
		App app;
		app.get("/test/:id/name/:name/price/:price", &DummyClass::dummyFunction);
		app.get("/another/:id/product/:name", &DummyClass::anotherDummyFunction);

		BOOST_REQUIRE_EQUAL("/test/:id/name/:name/price/:price", app.findResource("/test/123/name/something/price/22.50", "GET"));
		BOOST_REQUIRE_EQUAL("/another/:id/product/:name", app.findResource("/another/456/product/notebook/", "GET"));
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
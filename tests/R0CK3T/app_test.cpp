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

	BOOST_AUTO_TEST_SUITE_END()

}
#include <boost/test/unit_test.hpp>
#include "../../include/R0CK3T/http_request.h"

namespace unit_test {

	using namespace R0CK3T;
	
	BOOST_AUTO_TEST_SUITE(HttpRequestTest)

	BOOST_AUTO_TEST_CASE(should_get_url_param)
	{
		HttpRequest request;

		request.urlParams["some_param"] = "some_value";

		BOOST_REQUIRE_EQUAL("some_value", request.getUrlParam("some_param"));	
	}

	BOOST_AUTO_TEST_CASE(should_return_empty_string_case_url_param_is_not_found)
	{
		HttpRequest request;

		BOOST_REQUIRE_EQUAL("", request.getUrlParam("some_param"));
	}

	BOOST_AUTO_TEST_SUITE_END()

}
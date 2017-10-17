#include <boost/test/unit_test.hpp>
#include "../../include/R0CK3T/mime_types.h"

namespace unit_test {

	using namespace R0CK3T;

	BOOST_AUTO_TEST_SUITE(MimeTypesTest)

	BOOST_AUTO_TEST_CASE(should_return_mime_type_given_an_extension)
	{
		BOOST_REQUIRE_EQUAL("image/gif", mime_types::extensionToType("gif"));
		BOOST_REQUIRE_EQUAL("text/html", mime_types::extensionToType("html"));
	}

	BOOST_AUTO_TEST_CASE(should_return_default_mime_type_given_a_not_found_extension)
	{
		BOOST_REQUIRE_EQUAL("text/plain", mime_types::extensionToType("xxx"));
	}

	BOOST_AUTO_TEST_SUITE_END()

}
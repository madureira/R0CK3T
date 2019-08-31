#pragma once

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "./vendors/nlohmann/json.hpp"
#include "./mime_types.h"
#include "./header.h"

using jsonObject = nlohmann::json;

namespace R0CK3T {

	struct HttpResponse
	{
		enum StatusType
		{
			ok = 200,
			created = 201,
			accepted = 202,
			no_content = 204,
			multiple_choices = 300,
			moved_permanently = 301,
			moved_temporarily = 302,
			not_modified = 304,
			bad_request = 400,
			unauthorized = 401,
			forbidden = 403,
			not_found = 404,
			internal_server_error = 500,
			not_implemented = 501,
			bad_gateway = 502,
			service_unavailable = 503
		} status;

		std::vector<Header> headers;

		std::string content;

		std::vector<boost::asio::const_buffer> toBuffers();

		bool hasStatus();

		static HttpResponse stockReply(StatusType status);

		void json(jsonObject data);

		void send(std::string data);
	};

}
#pragma once

#include <tuple>

namespace R0CK3T {

	struct HttpRequest;

	class RequestParser
	{
	public:
		RequestParser();

		void reset();

		enum ResultType { GOOD, BAD, INDETERMINATE };

		template <typename InputIterator>
		std::tuple<ResultType, InputIterator> parse(HttpRequest& request, InputIterator begin, InputIterator end)
		{
			while (begin != end)
			{
				ResultType result = consume(request, *begin++);
				if (result == GOOD || result == BAD)
				{
					return std::make_tuple(result, begin);
				}
			}
			return std::make_tuple(INDETERMINATE, begin);
		}

	private:
		ResultType consume(HttpRequest& request, char input);

		static bool isChar(int c);

		static bool isCtl(int c);

		static bool isTspecial(int c);

		static bool isDigit(int c);

		enum State
		{
			method_start,
			method,
			uri,
			http_version_h,
			http_version_t_1,
			http_version_t_2,
			http_version_p,
			http_version_slash,
			http_version_major_start,
			http_version_major,
			http_version_minor_start,
			http_version_minor,
			expecting_newline_1,
			header_line_start,
			header_lws,
			header_name,
			space_before_header_value,
			header_value,
			expecting_newline_2,
			expecting_newline_3
		} m_state;
	};

}
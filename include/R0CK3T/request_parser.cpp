#include "request_parser.h"
#include "http_request.h"

namespace R0CK3T {

	RequestParser::RequestParser() : m_state(method_start)
	{
	}

	void RequestParser::reset()
	{
		m_state = method_start;
	}

	RequestParser::ResultType RequestParser::consume(HttpRequest& request, char input)
	{
		switch (m_state)
		{
		case method_start:
			if (!isChar(input) || isCtl(input) || isTspecial(input))
			{
				return BAD;
			}
			else
			{
				m_state = method;
				request.method.push_back(input);
				return INDETERMINATE;
			}
		case method:
			if (input == ' ')
			{
				m_state = uri;
				return INDETERMINATE;
			}
			else if (!isChar(input) || isCtl(input) || isTspecial(input))
			{
				return BAD;
			}
			else
			{
				request.method.push_back(input);
				return INDETERMINATE;
			}
		case uri:
			if (input == ' ')
			{
				m_state = http_version_h;
				return INDETERMINATE;
			}
			else if (isCtl(input))
			{
				return BAD;
			}
			else
			{
				request.uri.push_back(input);
				return INDETERMINATE;
			}
		case http_version_h:
			if (input == 'H')
			{
				m_state = http_version_t_1;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case http_version_t_1:
			if (input == 'T')
			{
				m_state = http_version_t_2;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case http_version_t_2:
			if (input == 'T')
			{
				m_state = http_version_p;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case http_version_p:
			if (input == 'P')
			{
				m_state = http_version_slash;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case http_version_slash:
			if (input == '/')
			{
				request.http_version_major = 0;
				request.http_version_minor = 0;
				m_state = http_version_major_start;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case http_version_major_start:
			if (isDigit(input))
			{
				request.http_version_major = request.http_version_major * 10 + input - '0';
				m_state = http_version_major;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case http_version_major:
			if (input == '.')
			{
				m_state = http_version_minor_start;
				return INDETERMINATE;
			}
			else if (isDigit(input))
			{
				request.http_version_major = request.http_version_major * 10 + input - '0';
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case http_version_minor_start:
			if (isDigit(input))
			{
				request.http_version_minor = request.http_version_minor * 10 + input - '0';
				m_state = http_version_minor;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case http_version_minor:
			if (input == '\r')
			{
				m_state = expecting_newline_1;
				return INDETERMINATE;
			}
			else if (isDigit(input))
			{
				request.http_version_minor = request.http_version_minor * 10 + input - '0';
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case expecting_newline_1:
			if (input == '\n')
			{
				m_state = header_line_start;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case header_line_start:
			if (input == '\r')
			{
				m_state = expecting_newline_3;
				return INDETERMINATE;
			}
			else if (!request.headers.empty() && (input == ' ' || input == '\t'))
			{
				m_state = header_lws;
				return INDETERMINATE;
			}
			else if (!isChar(input) || isCtl(input) || isTspecial(input))
			{
				return BAD;
			}
			else
			{
				request.headers.push_back(Header());
				request.headers.back().name.push_back(input);
				m_state = header_name;
				return INDETERMINATE;
			}
		case header_lws:
			if (input == '\r')
			{
				m_state = expecting_newline_2;
				return INDETERMINATE;
			}
			else if (input == ' ' || input == '\t')
			{
				return INDETERMINATE;
			}
			else if (isCtl(input))
			{
				return BAD;
			}
			else
			{
				m_state = header_value;
				request.headers.back().value.push_back(input);
				return INDETERMINATE;
			}
		case header_name:
			if (input == ':')
			{
				m_state = space_before_header_value;
				return INDETERMINATE;
			}
			else if (!isChar(input) || isCtl(input) || isTspecial(input))
			{
				return BAD;
			}
			else
			{
				request.headers.back().name.push_back(input);
				return INDETERMINATE;
			}
		case space_before_header_value:
			if (input == ' ')
			{
				m_state = header_value;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case header_value:
			if (input == '\r')
			{
				m_state = expecting_newline_2;
				return INDETERMINATE;
			}
			else if (isCtl(input))
			{
				return BAD;
			}
			else
			{
				request.headers.back().value.push_back(input);
				return INDETERMINATE;
			}
		case expecting_newline_2:
			if (input == '\n')
			{
				m_state = header_line_start;
				return INDETERMINATE;
			}
			else
			{
				return BAD;
			}
		case expecting_newline_3:
			return (input == '\n') ? GOOD : BAD;
		default:
			return BAD;
		}
	}

	bool RequestParser::isChar(int c)
	{
		return c >= 0 && c <= 127;
	}

	bool RequestParser::isCtl(int c)
	{
		return (c >= 0 && c <= 31) || (c == 127);
	}

	bool RequestParser::isTspecial(int c)
	{
		switch (c)
		{
		case '(': case ')': case '<': case '>': case '@':
		case ',': case ';': case ':': case '\\': case '"':
		case '/': case '[': case ']': case '?': case '=':
		case '{': case '}': case ' ': case '\t':
			return true;
		default:
			return false;
		}
	}

	bool RequestParser::isDigit(int c)
	{
		return c >= '0' && c <= '9';
	}

}
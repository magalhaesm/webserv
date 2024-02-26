#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

#include "Message.hpp"
#include "HTTPParser.hpp"
#include "HTTPConstants.hpp"
#include "HTTPRequestBuilder.hpp"

SCENARIO("HTTP requests can be parsed correctly")
{
    HTTPRequestBuilder builder;

    GIVEN("A valid HTTP GET request with a query string")
    {
        Message msg;
        builder.setHost("example.com");
        builder.setPath("/test?name=John%20Doe&age=30&city=New%20York");
        std::string raw = builder.getRequest();

        WHEN("parsing the request")
        {
            HTTPParser::parseRequest(raw, msg);

            THEN("the request line is parsed correctly")
            {
                CHECK(msg.method == GET);
                CHECK(msg.path == "/test");
                CHECK(msg.version == "HTTP/1.1");
                CHECK(msg.headers["host"] == "example.com");
                CHECK(msg.error == 0);
                CHECK(msg.query == "name=John Doe&age=30&city=New York");
            }
        }
    }
}

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>

#include "Message.hpp"
#include "HTTPParser.hpp"
#include "HTTPConstants.hpp"
#include "HTTPRequestBuilder.hpp"

// NOTE: https://martinfowler.com/bliki/GivenWhenThen.html

SCENARIO("Parsing HTTP GET requests")
{
    Message msg;
    std::string raw;
    HTTPRequestBuilder builder;

    GIVEN("a valid GET request with a query string")
    {
        builder.setMethod("GET");
        builder.setHost("example.com");
        builder.setPath("/test?name=John%20Doe&age=30&city=New%20York");

        raw = builder.getRequest();

        WHEN("parsing")
        {
            HTTPParser::parseRequest(raw, msg);

            THEN("the request line is parsed correctly")
            {
                CHECK_EQ(msg.method, GET);
                CHECK_EQ(msg.path, "/test");
                CHECK_EQ(msg.version, "HTTP/1.1");
                CHECK_EQ(msg.headers["host"], "example.com");
                CHECK_EQ(msg.error, 0);
                CHECK_EQ(msg.query, "name=John Doe&age=30&city=New York");
            }
        }
    }
}

SCENARIO("Parsing HTTP POST requests")
{
    Message msg;
    std::string raw;
    HTTPRequestBuilder builder;

    GIVEN("a valid request with a URL-encoded body")
    {
        builder.setMethod("POST");
        builder.setContentType("application/x-www-form-urlencoded");
        builder.setBody("name=John%20Doe&age=30&city=New%20York");
        raw = builder.getRequest();

        WHEN("parsing")
        {
            HTTPParser::parseRequest(raw, msg);

            THEN("the request and body are parsed correctly")
            {
                CHECK_EQ(msg.method, POST);
                CHECK_EQ(msg.error, 0);
                CHECK_EQ(msg.body->get("content"), "name=John Doe&age=30&city=New York");
            }
        }
    }

    GIVEN("a valid request with a form-data body")
    {
        builder.setMethod("POST");
        builder.setContentType("multipart/form-data");
        builder.setFileToSend("file.txt", "I'm full of contents!");

        raw = builder.getRequest();

        WHEN("parsing")
        {
            HTTPParser::parseRequest(raw, msg);

            THEN("the request and body are parsed correctly")
            {
                CHECK_EQ(msg.method, POST);
                CHECK_EQ(msg.error, 0);
                CHECK_EQ(msg.body->get("file"), "file.txt");
            }
        }
    }

    GIVEN("a valid request with an oversized body")
    {
        builder.setMethod("POST");
        builder.setContentType("application/x-www-form-urlencoded");
        builder.setBody("I'm a veeeeeeery long content");

        raw = builder.getRequest();
        int maxBodySize = 10;

        WHEN("parsing")
        {
            HTTPParser::parseRequest(raw, msg, maxBodySize);

            THEN("the error status code is correctly set to 413")
            {
                CHECK_EQ(msg.error, 413);
            }
        }
    }
}

SCENARIO("Parsing HTTP DELETE requests")
{
    Message msg;
    std::string raw;
    HTTPRequestBuilder builder;

    GIVEN("a valid DELETE request")
    {
        builder.setMethod("DELETE");
        builder.setPath("/some-content");
        raw = builder.getRequest();

        WHEN("parsing")
        {
            HTTPParser::parseRequest(raw, msg);

            THEN("the request line is parsed correctly")
            {
                CHECK_EQ(msg.method, DELETE);
                CHECK_EQ(msg.path, "/some-content");
                CHECK_EQ(msg.version, "HTTP/1.1");
                CHECK_EQ(msg.error, 0);
            }
        }
    }
}

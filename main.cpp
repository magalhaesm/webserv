#include "Server.hpp"
#include "EventListener.hpp"

int main(void)
{
    EventListener listener;

    ConfigSpec config(8080, "TestServer1");
    Server server(config);
    listener.subscribe(&server);

    ConfigSpec config2(8081, "TestServer2");
    Server server2(config2);
    listener.subscribe(&server2);

    listener.start();
}

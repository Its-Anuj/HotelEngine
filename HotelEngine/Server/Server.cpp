#include <iostream>
#define ASIO_DISABLE_IOCP
#include "crow.h"

int main(int argc, char const *argv[])
{
    crow::SimpleApp App;

    CROW_ROUTE(App, "/api/command")
        .methods(crow::HTTPMethod::GET, crow::HTTPMethod::POST)([](const crow::request &Request)
                                                                {
        // Construct a structured JSON reply object
        auto incoming_json = crow::json::load(Request.body);

        // Validation check
        if (!incoming_json) {
            std::cerr << "Received malformed or empty data packet.\n";
            return crow::response(400, "Bad Request: Invalid JSON");
        }

        auto TestID = incoming_json["TestID"].i();
        std::cout << "[Server Log] Received from client " << TestID << "\n";

        crow::json::wvalue reply;
        reply["Response"] = "U Fuckin Dumbass";

        return crow::response(200, reply.dump()); 
    });

    std::cout << "Run Server" << "\n";

    App.port(18080)
        .bindaddr("127.0.0.1")
        .run();

    return 0;
}

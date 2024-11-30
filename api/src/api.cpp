#include "crow.h"
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>

int main() {
    mongocxx::instance instance{};
    mongocxx::client client{mongocxx::uri{"mongodb://root:example@mongodb:27017"}};
    auto db = client["test_db"];
    auto collection = db["test_collection"];

    crow::SimpleApp app;

    CROW_ROUTE(app, "/")
    ([] {
        CROW_LOG_INFO << "Root route accessed.";
        return "Hello from C++ API!";
    });

    CROW_ROUTE(app, "/data")
    ([&] {
        bsoncxx::builder::stream::document document{};
        document << "name" << "example";
        collection.insert_one(document.view());

        crow::json::wvalue json_response;
        json_response["name"] = "example";

        CROW_LOG_INFO << "Inserted document into MongoDB.";
        return crow::response(200, json_response);
    });

    CROW_ROUTE(app, "/find/<string>")
    ([&](const std::string& name) {
        // Query the collection for documents matching the name
        auto cursor = collection.find(
            bsoncxx::builder::stream::document{} << "name" << name << bsoncxx::builder::stream::finalize);

        crow::json::wvalue result;
        size_t index = 0;

        // Convert each BSON document to JSON and add it to the response
        for (const auto& doc : cursor) {
            // Convert BSON to JSON and add to the array with meaningful indexing
            auto json_doc = crow::json::load(bsoncxx::to_json(doc));
            result["documents"][index] = json_doc;
            index++;
        }

        // Return a 404 response if no documents were found
        if (index == 0) {
            return crow::response(404, crow::json::wvalue{{"error", "No documents found"}});
        }

        // Add metadata to the response
        result["count"] = index;
        result["status"] = "success";

        // Return the JSON response
        return crow::response(result);
    });

    CROW_ROUTE(app, "/delete/<string>")
    ([&](const std::string& name) {
        auto result = collection.delete_one(bsoncxx::builder::stream::document{} << "name" << name << bsoncxx::builder::stream::finalize);

        if (result && result->deleted_count() > 0) {
            return crow::response{"Document deleted"};
        }
        return crow::response(404, "Document not found");
    });

    app.port(8080).multithreaded().run();
}
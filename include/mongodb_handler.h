#ifndef MONGODB_HANDLER_H
#define MONGODB_HANDLER_H

#include <string>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <iostream>

class MongoDBHandler {
public:
    MongoDBHandler(const std::string& uri, const std::string& db_name, const std::string& collection_name);
    void storeJson(const std::string& json_data);

private:
    mongocxx::client client;
    mongocxx::database db;
    mongocxx::collection collection;
};

#endif // MONGODB_HANDLER_H
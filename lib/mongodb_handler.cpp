#include "mongodb_handler.h"

MongoDBHandler::MongoDBHandler(const std::string& uri, const std::string& db_name, const std::string& collection_name)
    : client(mongocxx::uri{uri}), db(client[db_name]), collection(db[collection_name]) {}

void MongoDBHandler::storeJson(const std::string& json_data) {
    bsoncxx::document::value doc = bsoncxx::from_json(json_data);
    collection.insert_one(doc.view());
}
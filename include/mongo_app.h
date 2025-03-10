#ifndef MONGO_APP_H
#define MONGO_APP_H

#include "application.h"
#include "proto_json_converter.h"
#include "mongodb_handler.h"

class MongoApp : public Application
{
public:
    MongoApp(const int receive_port, const char *receive_ip, bool debug, bool debug_sub);

    void update() override; // Override the update method

private:
    mongocxx::instance instance_; // MongoDB instance (must be a member)
    mongocxx::client client_;     // MongoDB client
};

#endif // MONGO_APP_H
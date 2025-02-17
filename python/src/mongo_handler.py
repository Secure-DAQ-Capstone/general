from pymongo import MongoClient
import json

class MongoDBHandler:
    def __init__(self, uri, db_name, collection_name):
        self.client = MongoClient(uri)
        self.db = self.client[db_name]
        self.collection = self.db[collection_name]

    def store_json(self, json_data):
        doc = json.loads(json_data)
        self.collection.insert_one(doc)
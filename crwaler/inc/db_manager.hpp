#ifndef STORING_HPP
#define STORING_HPP

#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>

#include <string>
#include <unordered_map>
#include <iostream>
#include <mutex>

class DBManager
{
private:
    mongocxx::instance m_instance;
    const mongocxx::uri m_uri;
    mongocxx::client m_client;
    mongocxx::database m_db;
    mongocxx::collection m_words_collection;
    mongocxx::collection m_urls_collection;
    mongocxx::collection m_update_collection;

    mutable std::mutex mtx;
    void create_index(mongocxx::collection &collection, const std::string &filed);
    void bson_update_root_urls(mongocxx::bulk_write &bulk, const std::string &word, const std::string &root_url);
    void bson_push_link(mongocxx::bulk_write &bulk, const std::string &word, const std::string &root_url, const std::string &url, size_t count);

        public : DBManager();
    ~DBManager() = default;
    void store_urls(std::unordered_map<std::string, size_t> &urls_dict, std::string &parent_key);
    void store_words(std::unordered_map<std::string, size_t> &wordData, std::string &url);
    // void insertArrayIntoMongoDB();

    // std::string hash_url(const std::string &url);

    // std::string popAndUpdate();

    // void insert_word_count_to_mongodb(const std::string &word, const std::string &url, size_t count);
};
#endif
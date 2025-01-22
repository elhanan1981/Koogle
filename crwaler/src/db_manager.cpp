#include "../inc/db_manager.hpp"
#include "../inc/settings.hpp"
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <vector>

DBManager::DBManager()
    : m_uri(Settings::URI_CONNECTION), m_client(m_uri), m_db(m_client[Settings::DB_NAME]), m_words_collection(m_db[Settings::WORDS_COLLECTION]), m_urls_collection(m_db[Settings::URLS_COLLECTION]), m_update_collection(m_db[Settings::UPDATE_COLLECTION])

{
    this->create_index(m_words_collection, "word");
}

void DBManager::create_index(mongocxx::collection &collection, const std::string &filed)
{
    bsoncxx::builder::stream::document index_builder;
    index_builder << filed << 1;
    collection.create_index(index_builder.view());
}

void DBManager::store_urls(std::unordered_map<std::string, size_t> &urls_dict, std::string &parent_key)
{
    std::unique_lock<std::mutex> g{mtx};
    try
    {
        bsoncxx::builder::stream::document document{};

        document << "_id" << parent_key;

        for (const auto &[url, count] : urls_dict)
        {
            document << url << static_cast<int64_t>(count);
        }
        m_urls_collection.insert_one(document.view());
    }
    catch (const mongocxx::exception &e)
    {
    }
    catch (const std::exception &e)
    {
    }
}

void DBManager::store_words(std::unordered_map<std::string, size_t> &wordData, std::string &url)
{
    try
    {
        auto bulk = m_words_collection.create_bulk_write();

        for (const auto &[word, count] : wordData)
        {
            try
            {
                bson_update_root_urls(bulk, word, Settings::ROOT_URL);
                bson_push_link(bulk, word, Settings::ROOT_URL, url, count);
            }
            catch (const std::exception &e)
            {
                std::cerr << "Error creating BSON document for word: " << word << " - " << e.what() << std::endl;
            }
        }

        try
        {
            std::unique_lock<std::mutex> lock(mtx);
            bulk.execute();
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error executing BulkWrite for url: " << url << " - " << e.what() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error updating Word documents: " << e.what() << std::endl;
        throw;
    }
}

void DBManager::bson_update_root_urls(mongocxx::bulk_write &bulk, const std::string &word, const std::string &root_url)
{
    bsoncxx::builder::stream::document filter_builder, update_builder;

    // Filter for the specific word and root_url to avoid duplication
    filter_builder << "word" << word
                   << "roots_urls.root_url" << bsoncxx::builder::stream::open_document
                   << "$ne" << root_url
                   << bsoncxx::builder::stream::close_document;

    // Ensure the "roots_urls" array contains the "root_url"
    update_builder << "$addToSet" << bsoncxx::builder::stream::open_document
                   << "roots_urls" << bsoncxx::builder::stream::open_document
                   << "root_url" << root_url
                   << "links" << bsoncxx::builder::stream::open_array << bsoncxx::builder::stream::close_array
                   << bsoncxx::builder::stream::close_document
                   << bsoncxx::builder::stream::close_document;

    mongocxx::model::update_one add_op{filter_builder.view(), update_builder.view()};
    add_op.upsert(true);

    bulk.append(mongocxx::model::write{std::move(add_op)});
}

void DBManager::bson_push_link(mongocxx::bulk_write &bulk, const std::string &word, const std::string &root_url, const std::string &url, size_t count)
{
    bsoncxx::builder::stream::document filter_builder, update_builder;

    // Filter for the specific word and root_url
    filter_builder << "word" << word
                   << "roots_urls.root_url" << root_url;

    // Add the link to the "links" array
    bsoncxx::builder::stream::document push_builder;
    push_builder << "roots_urls.$.links" << bsoncxx::builder::stream::open_document
                 << "url" << url
                 << "count" << bsoncxx::types::b_int32{static_cast<int32_t>(count)}
                 << bsoncxx::builder::stream::close_document;

    update_builder << "$push" << push_builder.view();

    mongocxx::model::update_one push_op{filter_builder.view(), update_builder.view()};
    push_op.upsert(false); // No need to upsert here, as the document should already exist

    bulk.append(mongocxx::model::write{std::move(push_op)});
}

// void DBManager::store_words(std::unordered_map<std::string, size_t> &words_vec, std::string &url)
// {
//     std::unique_lock<std::mutex> g{mtx};
//     for (const auto &[word, count] : words_vec)
//     {
//         bsoncxx::builder::stream::document filter;
//         bsoncxx::builder::stream::document set_update;
//         bsoncxx::builder::stream::document push_update;

//         // Create filter for the word
//         filter << "word" << word;

//         // Create the set update document for roots_urls
//         set_update << "$set" << bsoncxx::builder::stream::open_document
//                    << "roots_urls." + url + ".count" << static_cast<int64_t>(count)
//                    << bsoncxx::builder::stream::close_document;

//         mongocxx::options::update options{};
//         options.upsert(true);

//         // First, ensure the roots_urls subdocument is set up correctly
//         m_words_collection.update_one(filter.view(), set_update.view(), options);

//         // Create the push update document to add words to the array
//         push_update << "$push" << bsoncxx::builder::stream::open_document
//                     << "roots_urls." + url + ".words" << word
//                     << bsoncxx::builder::stream::close_document;

//         // Then, push the word into the words array
//         m_words_collection.update_one(filter.view(), push_update.view(), options);
//     }
// }

// void DBManager::insertArrayIntoMongoDB()
// {
//     bsoncxx::builder::stream::document filterDoc{};
//     filterDoc << "action" << "update";

//     // URL to append
//     std::string url = "http://example.com";

//     // Create an update document to push the URL into the 'update' array
//     bsoncxx::builder::stream::document updateDoc{};
//     updateDoc << "$push" << bsoncxx::builder::stream::open_document
//               << "array_urls" << url
//               << bsoncxx::builder::stream::close_document;

//     // Options to specify upsert
//     mongocxx::options::update updateOptions{};
//     updateOptions.upsert(true);

//     // Perform the update operation with upsert option
//     auto result = m_update_collection.update_one(filterDoc.view(), updateDoc.view(), updateOptions);

//     if (result)
//     {
//         std::cout << "Modified count: " << result->modified_count() << std::endl;
//         if (result->upserted_id())
//         {
//             std::cout << "Upserted ID: " << result->upserted_id()->get_oid().value.to_string() << std::endl;
//         }
//     }
//     else
//     {
//         std::cout << "Update operation failed." << std::endl;
//     }
// }

// std::string DBManager::popAndUpdate()
// {
//     auto filter = bsoncxx::builder::stream::document{} << "action" << "update" << bsoncxx::builder::stream::finalize;
//     auto result = m_update_collection.find_one(filter.view());

//     if (result) {
//         std::cout << "Document found with 'action' set to 'update'." << std::endl;
//         auto update_array = result->view()["array_urls"].get_array().value;
//         if (!update_array.empty()) {
//             std::cout << "Array of URLs found in the document." << std::endl;
//             auto update_doc = update_array[0].get_document().value;
//             std::cout<<"hi  hi\n";

//             auto oid_value = update_doc["$oid"].get_oid().value.to_string();
//             std::cout << "OID value: " << oid_value << std::endl;
//             std::string url = bsoncxx::to_json(bsoncxx::from_json(oid_value));
//             return url;
//         } else {
//             std::cerr << "No update URLs in the collection." << std::endl;
//             return "";
//         }
//     } else {
//         std::cerr << "No documents with 'action' set to 'update' in the collection." << std::endl;
//         return "";
//     }
// }
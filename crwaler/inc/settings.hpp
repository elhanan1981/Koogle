#ifndef SETTINGS_HPP
#define SETTINGS_HPP
#include <iostream>
class Settings
{
public:
    static inline std::string URI_CONNECTION = "mongodb+srv://shmumudror0:OZT7Vlf4tXgIkTDw@koogle.40sqdds.mongodb.net/?retryWrites=true&w=majority&appName=Koogle&tls=true&tlsAllowInvalidCertificates=true";
    static inline std::string DB_NAME = "koogle";
    static inline std::string URLS_COLLECTION = "linker";
    static inline std::string WORDS_COLLECTION = "indexer";
    static inline std::string UPDATE_COLLECTION = "update";

    static inline size_t NUM_THREADS = 10;

    static inline std::string START_URL = "https://harrypotter.fandom.com/wiki/Main_Page";
    static inline std::string ROOT_URL = "https://harrypotter.fandom";
    static inline size_t MAX_DEPTH = 2;
    static inline std::string POISON_APPLE = "";

};

#endif
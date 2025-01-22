#include "./inc/crawler.hpp"
#include "./inc/db_manager.hpp"

int main()
{
    // DBManager manager;
    // manager.insertArrayIntoMongoDB();
    Crawler myCrawler;
    myCrawler.manage_threads();

    return 0;
}

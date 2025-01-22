#include "../inc/worker.hpp"
#include "../inc/download.hpp"
#include "../inc/parser.hpp"
#include "../inc/indexer.hpp"
#include "../inc/settings.hpp"

Worker::Worker()
{
    m_max_depth = Settings::MAX_DEPTH;
    m_queue.push(Settings::START_URL, 0);
    m_set.insert(Settings::START_URL);
}

Worker::~Worker()
{
}

void Worker::work()
{
    std::string current_url;
    size_t current_depth;
    m_queue.pop(current_url, current_depth);

    while (current_url != Settings::POISON_APPLE)
    {
        std::cout<<current_url<<"\n";
        std::string html_content = Download::download_html(current_url);
        
        if (!html_content.empty())
        {
            std::vector<std::string> links = Parser::extract_url(html_content);
            std::vector<std::string> words = Parser::extract_words(html_content);
            
            auto links_um = Indexer::index(links);
            auto words_um = Indexer::index(words);

            handle_new_urls(links, ++current_depth);

            m_storing.store_words(words_um, current_url);
            m_storing.store_urls(links_um, current_url);
        }
        m_queue.pop(current_url, current_depth);
    }
}

void Worker::handle_new_urls(std::vector<std::string> urls, const size_t depth)
{
    if (depth < m_max_depth)
    {
        for (auto url : urls)
        {
            if (!m_set.contains(url) && url.find(Settings::ROOT_URL) != std::string::npos)
            {
                m_queue.push(url, depth);
                m_set.insert(url);
            }
        }
    }
}
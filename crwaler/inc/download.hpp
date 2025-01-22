#ifndef DOWNLOAD_HPP
#define DOWNLOAD_HPP

#include <vector>
#include <string>

class Download
{
public:
    Download() = default;
    ~Download() = default;

    static std::string download_html(const std::string &url);
};

#endif
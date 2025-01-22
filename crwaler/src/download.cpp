#include "../inc/download.hpp"

#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

std::string Download::download_html(const std::string &url)
{
    std::ostringstream html_os;
    try
    {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;

        request.setOpt(new curlpp::options::Url(url));

        request.setOpt(new curlpp::options::WriteStream(&html_os));
        request.perform();

        return html_os.str();
    }
    catch (curlpp::RuntimeError &e)
    {
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }
    catch (curlpp::LogicError &e)
    {
        std::cerr << "Logic error: " << e.what() << std::endl;
    }
    return "";
}

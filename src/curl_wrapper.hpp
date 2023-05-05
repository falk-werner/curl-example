#ifndef CURL_WRAPPER_HPP
#define CURL_WRAPPER_HPP

#include <string>
#include <functional>
#include <curl/curl.h>

namespace curl_wrapper
{

using write_cb = std::function<size_t (void * data, size_t size, size_t count)>;

class curl_easy
{
    curl_easy(curl_easy const &) = delete;
    curl_easy& operator=(curl_easy const &) = delete;
    curl_easy(curl_easy &&) = delete;
    curl_easy& operator=(curl_easy &&) = delete;

public:
    curl_easy();
    ~curl_easy();

    void set_url(std::string const & url);
    void follow_location(bool value = true);
    void set_write_callback(write_cb callback);

    long perform();

private:
    CURL * handle;
    write_cb write_callback;
};


}

#endif

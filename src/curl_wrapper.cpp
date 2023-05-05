#include "curl_wrapper.hpp"
#include <stdexcept>

extern "C"
{

static size_t curl_wrapper_write_callback(void * data, size_t size, size_t count, void * user_data)
{
    auto * const callback = reinterpret_cast<curl_wrapper::write_cb *>(user_data);
    return (*callback)(data, size, count);
}

}


namespace curl_wrapper
{

curl_easy::curl_easy()
: handle(curl_easy_init())
{
    if (nullptr == handle)
    {
        throw std::runtime_error("failed to create cURL handle");
    }
}

curl_easy::~curl_easy()
{
    curl_easy_cleanup(handle);
}

void curl_easy::set_url(std::string const & url)
{
    auto const status = curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    if (CURLE_OK != status)
    {
        throw std::runtime_error("failed to set URL");            
    }
}

void curl_easy::set_write_callback(write_cb callback)
{
    write_callback = callback;
    void * const ptr = reinterpret_cast<void*>(&write_callback);

    auto status = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &curl_wrapper_write_callback);
    if (CURLE_OK != status)
    {
        throw std::runtime_error("failed to set write function");            
    }

    status = curl_easy_setopt(handle, CURLOPT_WRITEDATA, ptr);
    if (CURLE_OK != status)
    {
        throw std::runtime_error("failed to set write data");            
    }
}

void curl_easy::follow_location(bool value)
{
    auto const status = curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, value ? 1L : 0L);
    if (CURLE_OK != status)
    {
        throw std::runtime_error("failed to set URL");            
    }

}

long curl_easy::perform()
{
    auto const status = curl_easy_perform(handle);
    if (CURLE_OK != status)
    {
        throw std::runtime_error("failed to perform request");            
    }

    long http_status = 500;
    curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_status);

    return http_status;
}


}
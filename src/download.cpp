#include <iostream>
#include <sstream>
#include <stdexcept>
#include "curl_wrapper.hpp"

using curl_wrapper::curl_easy;

std::string fetch(std::string const & url)
{
    std::ostringstream stream;

    curl_easy download;
    download.set_url(url);
    download.follow_location();
    download.set_write_callback([&stream](void * data, size_t size, size_t count) {
        size_t const bytes = size * count;
        auto * ptr = reinterpret_cast<char*>(data);
        stream.write(ptr, bytes);
        return bytes;
    });
    long const http_status = download.perform();

    if ((http_status < 200) || (300 <= http_status))
    {
        throw new std::runtime_error("failed to fetch");
    }

    return stream.str();
}

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        try
        {
            std::string const url = argv[1];

            auto contents = fetch(url);
            std::cout << contents << std::endl;
        }
        catch (std::exception const & ex)
        {
            std::cerr << "error: " << ex.what() << std::endl;
        }
    }
    else
    {
        std::cout << "usage: download <url>" << std::endl;
    }
    return EXIT_SUCCESS;
}

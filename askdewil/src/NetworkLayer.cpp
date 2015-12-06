#include "NetworkLayer.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <cpprest/http_client.h>

std::string parse_stream(std::istream &stream)
{
    std::string line;
    while(std::getline(stream, line))
    {
        if (!line.length())
        {
            std::ostringstream ss;
            ss << stream.rdbuf();
            return ss.str();
        }
    }
    return std::string();
}

std::string NetworkLayer::fetch_url(const std::string &url)
{
    using namespace web::http;
    using namespace concurrency::streams;
    client::http_client client("http://sniken.asuscomm.com:8090/static/http.dat");
    auto resp = client.request(methods::GET).then([=](http_response response)
    {
        concurrency::streams::container_buffer<std::string> bufferStream;
        auto bytesRead = response.body().read_to_end(bufferStream).get();
        printf("Received response status code:%u\n", response.status_code());
        printf("Received response:%s\n", bufferStream.collection().c_str());
        printf("Received bytesRead:%lu\n", bytesRead);
    });
    resp.wait();

    std::ifstream file(url.c_str(), std::ifstream::in);
    return parse_stream(file);
}
#include "NetworkLayer.h"

#include <cpprest/http_client.h>


void NetworkLayer::fetch_url(const std::string &url,const std::string &resp_body,
                                    std::function<void(std::string)> handler) {
    using namespace web::http;
    using namespace concurrency::streams;
    client::http_client client(url);

    auto resp = client.request(methods::GET, std::string(""), resp_body).then([=](http_response response) {
        concurrency::streams::container_buffer<std::string> bufferStream;
        auto bytesRead = response.body().read_to_end(bufferStream).get();
        service_.post(std::bind(handler, bufferStream.collection()));
        printf("Received response status code:%u\n", response.status_code());
        printf("Received response:%s\n", bufferStream.collection().c_str());
        printf("Received bytesRead:%lu\n", bytesRead);
    });
}
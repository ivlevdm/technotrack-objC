#ifndef ASKDEWIL_NETWORKLAYER_H
#define ASKDEWIL_NETWORKLAYER_H

#include <string>
#include <boost/asio.hpp>

class NetworkLayer {
public:
    NetworkLayer(boost::asio::io_service &service) : service_(service) {}
    void fetch_url(const std::string &url, const std::string &resp_body,
                          std::function<void(std::string)> handler);

private:
    boost::asio::io_service &service_;
};



#endif //ASKDEWIL_NETWORKLAYER_H

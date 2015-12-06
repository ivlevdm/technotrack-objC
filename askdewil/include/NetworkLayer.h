#ifndef ASKDEWIL_NETWORKLAYER_H
#define ASKDEWIL_NETWORKLAYER_H

#include <string>
#include <boost/asio.hpp>

class NetworkLayer {
public:
    NetworkLayer(boost::asio::io_service &service) : service_(service) {}
    std::string fetch_url(const std::string &url);

private:
    boost::asio::io_service &service_;
};



#endif //ASKDEWIL_NETWORKLAYER_H

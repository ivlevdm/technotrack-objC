#ifndef ASKDEWIL_BUSINESSLOGIC_H
#define ASKDEWIL_BUSINESSLOGIC_H

#include "NetworkLayer.h"
#include "ProtocolLayer.h"

class BusinessLogic {
public:
    static BusinessLogic& get_instance() {
        static BusinessLogic instance;

        return instance;
    }

    void run();

    BusinessLogic(BusinessLogic const&) = delete;
    void operator=(BusinessLogic const&) = delete;

private:
    BusinessLogic() : network_(NetworkLayer(service_)), protocol_(Protocol(network_, service_)) {};

    boost::asio::io_service service_;
    NetworkLayer network_;
    Protocol protocol_;
};

#endif //ASKDEWIL_BUSINESSLOGIC_H

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

    static void printQuestions(std::vector<Question> &&questions);

    void run();

    BusinessLogic(BusinessLogic const&) = delete;
    void operator=(BusinessLogic const&) = delete;

private:
    BusinessLogic() : network_(service_), protocol_(network_, service_),
        stream_in_({service_, STDIN_FILENO}), stream_out_({service_, STDOUT_FILENO}),
        work_(service_) {};

    void print_questions(std::vector<Question> questions);

    boost::asio::io_service service_;
    NetworkLayer network_;
    Protocol protocol_;
    boost::asio::posix::stream_descriptor stream_in_;
    boost::asio::posix::stream_descriptor stream_out_;
    boost::asio::io_service::work work_;
};

#endif //ASKDEWIL_BUSINESSLOGIC_H

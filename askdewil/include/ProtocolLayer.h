#ifndef ASKDEWIL_PROTOCOLLAYER_H
#define ASKDEWIL_PROTOCOLLAYER_H

#include "NetworkLayer.h"
#include <vector>
#include <string>

struct Question
{
    uint32_t id;
    std::string text;
    time_t creation_time;
};

struct Answer
{
    uint32_t id;
    uint32_t question_id;
    std::string text;
    time_t creation_time;
};

class Protocol {
public:
    Protocol(const NetworkLayer &network, boost::asio::io_service &service) :
            network_(network),
            service_(service) {}
    std::vector<Question> get_last_question(uint32_t cnt = 1);
    Question get_question_by_id(uint32_t id);


private:
    NetworkLayer network_;
    boost::asio::io_service &service_;
};

#endif //ASKDEWIL_PROTOCOLLAYER_H

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
    void get_last_questions(uint32_t cnt,
                            std::function<void(std::vector<Question>)> handler);
    void get_question_by_id(uint32_t id, std::function<void(Question)> handler);
    void get_last_answer( uint32_t question_id, uint32_t cnt,
                         std::function<void(std::vector<Answer>)> handler);


private:
    static const std::string url;

    NetworkLayer network_;
    boost::asio::io_service &service_;
};

#endif //ASKDEWIL_PROTOCOLLAYER_H

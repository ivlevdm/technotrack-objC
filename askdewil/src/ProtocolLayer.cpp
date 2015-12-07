#include "ProtocolLayer.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/iterator/transform_iterator.hpp>

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

using boost::property_tree::ptree;

const std::string Protocol::url = "http://sniken.asuscomm.com:8090/static/http.html";

Answer parseAnswer(const ptree &node) {
    return Answer {
            .id = node.get<uint32_t>("id"),
            .question_id = node.get<uint32_t>("parent"),
            .text = node.get<std::string>("text",""),
            .creation_time = node.get<time_t>("ts")
    };
}

Question parseQuestion(const ptree &node) {
    return Question {
            .id = node.get<uint32_t>("id"),
            .text = node.get<std::string>("text", ""),
            .creation_time = node.get<time_t>("ts")
    };
}

template <typename T>
std::vector<T> parseArray(const ptree &node, std::function<T(const ptree &node)> parser) {
    std::vector<T> result;
    boost::transform(node | boost::adaptors::map_values, std::back_inserter(result), parser);
    return result;
}

std::vector<Question> parseQuestions(const ptree &node)
{
    std::vector<Question> result;
    boost::transform(node | boost::adaptors::map_values, std::back_inserter(result), parseQuestion);
    return result;
}

void Protocol::get_last_questions(uint32_t cnt,
                                  std::function<void(std::vector<Question>)> handler) {

    network_.fetch_url(url, std::string(""), [handler](std::string json){
        ptree parsedJson;
        std::stringstream stream (json);
        boost::property_tree::read_json(stream, parsedJson);
        Question q = parseQuestion(parsedJson);
        std::cout << q.text;
    });
    /*
    std::string json = _network.fetch_url(_url);

    ptree parsedJson;
    std::stringstream stream (json);
    boost::property_tree::read_json(stream, parsedJson);
    return parseQuestion(parsedJson);
     */
}
#pragma clang diagnostic pop
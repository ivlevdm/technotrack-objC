#include "BuisnessLogic.h"
#include <boost/asio/write.hpp>

void BusinessLogic::run() {
    protocol_.get_last_questions(5, std::bind(&BusinessLogic::print_questions, this, std::placeholders::_1));
    service_.run();
}

void BusinessLogic::print_questions(std::vector<Question> questions) {
    using namespace boost::asio;
    auto handler = [](const boost::system::error_code&, std::size_t) {
        std::cout << ", world!\n";
    };
    async_write(stream_out_, buffer("Hello"), handler);
}
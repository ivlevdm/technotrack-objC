#include "BuisnessLogic.h"

void BusinessLogic::run() {
    boost::asio::io_service::work work(service_);
    protocol_.get_last_questions(5, [](std::vector<Question>){ std::cout << "Hello, I'm lambda"; });

    service_.run();
}

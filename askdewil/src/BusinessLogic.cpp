#include "BuisnessLogic.h"

void BusinessLogic::run() {
    boost::asio::io_service::work work(service_);
    std::vector<Question> questions = protocol_.get_last_question(5);

    service_.run();
}

#include <controller_service.hpp>

controller_service::controller_service() : rtm_(vsomeip::runtime::get()), app_(rtm_->create_application()), stop_(false) {
    stop_thread_ = std::thread{&controller_service::stop, this};
}

controller_service::~controller_service() {}

void controller_service::on_message_cb(const std::shared_ptr<vsomeip::message>& _response)
{

}

void controller_service::on_state_cb(vsomeip_v3::state_type_e _state)
{

}

bool controller_service::init() {
    // init the application
    if (!app_->init()) {
        std::clog << "Couldn't initialize application" << std::endl;
        return false;
    }

    // register a message handler callback for messages sent to our service
    app_->register_message_handler(service_id, service_instance_id, service_method_id,
                                    std::bind(&controller_service::on_message_cb, this, std::placeholders::_1));

    // register a state handler to get called back after registration at the
    // runtime was successful
    app_->register_state_handler(std::bind(&controller_service::on_state_cb, this, std::placeholders::_1));
    return true;
}

void controller_service::stop() {
    std::unique_lock<std::mutex> its_lock(mutex_);

    while (!stop_) {
        condition_.wait(its_lock);
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    // Stop offering the service
    app_->stop_offer_service(service_id, service_instance_id);
    // unregister the state handler
    app_->unregister_state_handler();
    // unregister the message handler
    app_->unregister_message_handler(service_id, service_instance_id, service_method_id);
    // shutdown the application
    app_->stop();
}

int main(int argc, char* argv[]) {
    std::cout << "controller_service started " << argc << " arguments.\n";
    return EXIT_SUCCESS;
}
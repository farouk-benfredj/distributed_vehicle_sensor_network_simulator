#include <iostream>
#include <chrono>
#include <sensor_service.hpp>

sensor_service::sensor_service()
{
    rtm_ = vsomeip_v3::runtime::get();
    app_ = rtm_->create_application();
}

sensor_service::~sensor_service(){}

void sensor_service::on_state_cb(vsomeip::state_type_e _state) {
    if (_state == vsomeip::state_type_e::ST_REGISTERED) {
        app_->request_service(SENSOR_SERVICE_ID, SENSOR_INSTANCE_ID);
    }
}

void sensor_service::on_message_cb(const std::shared_ptr<vsomeip::message>& _response)
{
    // to-do
}

void sensor_service::on_availability_cb(vsomeip::service_t _service, vsomeip::instance_t _instance,
    bool _is_available)
{
    // to-do
}

bool sensor_service::init()
{
    if(!app_->init())
    {
        std::cerr << "Failed to init application" << std::endl;
        return false;
    }
    // register a state handler to get called back after registration at the
    // runtime was successful
    app_->register_state_handler(std::bind(&sensor_service::on_state_cb, this, std::placeholders::_1));

    // register a callback for responses from the service
    app_->register_message_handler(vsomeip::ANY_SERVICE, SENSOR_INSTANCE_ID, vsomeip::ANY_METHOD,
        std::bind(&sensor_service::on_message_cb, this, std::placeholders::_1));

    // register a callback which is called as soon as the service is available
    app_->register_availability_handler(SENSOR_SERVICE_ID, SENSOR_INSTANCE_ID,
        std::bind(&sensor_service::on_availability_cb, this, std::placeholders::_1,
        std::placeholders::_2, std::placeholders::_3));
    return true;
}

void sensor_service::start()
{
    app_->start();
}

void sensor_service::stop()
{
    app_->unregister_state_handler();
    app_->unregister_message_handler(vsomeip::ANY_SERVICE, SENSOR_INSTANCE_ID, vsomeip::ANY_METHOD);
    app_->clear_all_handler();
    app_->release_service(SENSOR_SERVICE_ID, SENSOR_INSTANCE_ID);
    app_->stop();
}

int main(int argc, char* argv[]) {
    std::cout << "sensor_service started " << argc << " arguments.\n";

    sensor_service sensor_client;
    if (sensor_client.init()) {
        sensor_client.start();
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
    return 0;
}
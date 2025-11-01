#include <iostream>
#include <chrono>
#include <sensor_service.hpp>

sensor_service::sensor_service()
{
    rtm_ = vsomeip_v3::runtime::get();
    app_ = rtm_->create_application();
}

// sensor_service::sensor_service() : rtm_(vsomeip_v3::runtime::get()), app_(rtm_->create_application()) {}

sensor_service::~sensor_service(){}

void sensor_service::on_state_cb(vsomeip_v3::state_type_e _state) {
    if (_state == vsomeip_v3::state_type_e::ST_REGISTERED) {
        app_->request_service(SENSOR_SERVICE_ID, SENSOR_INSTANCE_ID);
    }
}

void sensor_service::on_message_cb(const std::shared_ptr<vsomeip_v3::message>& _response)
{
    if (SENSOR_SERVICE_ID == _response->get_service() && SENSOR_INSTANCE_ID == _response->get_instance()
            && vsomeip::message_type_e::MT_RESPONSE == _response->get_message_type()
            && vsomeip::return_code_e::E_OK == _response->get_return_code()) {
        // Get the payload and print it
        std::shared_ptr<vsomeip::payload> payload = _response->get_payload();
        std::string resp = std::string(reinterpret_cast<const char*>(payload->get_data()), 0, payload->get_length());
        std::clog << "Received: " << resp << std::endl;
        stop();
    }
}

void sensor_service::on_availability_cb(vsomeip_v3::service_t _service, vsomeip_v3::instance_t _instance,
    bool _is_available)
{
    if (SENSOR_SERVICE_ID == _service && SENSOR_INSTANCE_ID == _instance && _is_available) {
        // The service is available then we send the request
        // Create a new request
        std::shared_ptr<vsomeip::message> rq = rtm_->create_request();
        // Set the controller_service as target of the request
        rq->set_service(SENSOR_SERVICE_ID);
        rq->set_instance(SENSOR_INSTANCE_ID);
        rq->set_method(SENSOR_SERVICE_ID);

        // Create a payload which will be sent to the service
        std::shared_ptr<vsomeip::payload> pl = rtm_->create_payload();
        std::string str("Hello from sensor_service");
        std::vector<vsomeip::byte_t> pl_data(std::begin(str), std::end(str));

        pl->set_data(pl_data);
        rq->set_payload(pl);
        // Send the request to the service. Response will be delivered to the
        // registered message handler
        std::clog << "Sending: " << str << std::endl;
        app_->send(rq);
    }
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
    app_->register_message_handler(vsomeip_v3::ANY_SERVICE, SENSOR_INSTANCE_ID, vsomeip_v3::ANY_METHOD,
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
    app_->unregister_message_handler(vsomeip_v3::ANY_SERVICE, SENSOR_INSTANCE_ID, vsomeip_v3::ANY_METHOD);
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
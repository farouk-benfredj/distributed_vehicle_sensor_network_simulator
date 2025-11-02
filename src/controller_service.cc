#ifdef APP_ENABLE_SIGNAL_HANDLING
#include <csignal>
#endif

#include <controller_service.hpp>

controller_service::controller_service() :
    rtm_(vsomeip_v3::runtime::get()),
    app_(rtm_->create_application("controller_service")),
    stop_(false)
{
    stop_thread_ = std::thread{&controller_service::stop, this};
}

controller_service::~controller_service()
{
    if (std::this_thread::get_id() != stop_thread_.get_id()) {
        if (stop_thread_.joinable()) {
            stop_thread_.join();
        }
    } else {
        stop_thread_.detach();
    }
}

void controller_service::on_message_cb(const std::shared_ptr<vsomeip_v3::message>& _request)
{  
    // Create a response based upon the request
    std::shared_ptr<vsomeip_v3::message> resp = rtm_->create_response(_request);

    // Construct string to send back
    std::string str("Hello from controller_service");
    str.append(reinterpret_cast<const char*>(_request->get_payload()->get_data()), 0, _request->get_payload()->get_length());

    // Create a payload which will be sent back to the client
    std::shared_ptr<vsomeip_v3::payload> resp_pl = rtm_->create_payload();
    std::vector<vsomeip_v3::byte_t> pl_data(str.begin(), str.end());
    resp_pl->set_data(pl_data);
    resp->set_payload(resp_pl);

    // Send the response back
    app_->send(resp);
    // we have finished
    terminate();
}

void controller_service::on_state_cb(vsomeip_v3::state_type_e _state)
{
    if(_state == vsomeip_v3::state_type_e::ST_REGISTERED)
    {
        // we are registered at the runtime and can offer our service
        app_->offer_service(service_id, service_instance_id);
    }
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

void controller_service::start()
{
    // start the application and wait for the on_event callback to be called
    // this method only returns when app_->stop() is called (to-do check)
    app_->start();
}

void controller_service::terminate()
{
    std::lock_guard<std::mutex> its_lock(mutex_);
    stop_ = true;
    condition_.notify_one();
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

#ifdef APP_ENABLE_SIGNAL_HANDLING
controller_service* cs_ptr(nullptr);
void handle_signal(int _signal) {
    if (cs_ptr != nullptr && (_signal == SIGINT || _signal == SIGTERM))
        cs_ptr->terminate();
}
#endif

int main(int argc, char* argv[]) {
    std::cout << "controller_service started " << argc << " arguments.\n";

    // explicitly mark these parameters as unused
    (void)argc;
    (void)argv;
 
    controller_service controller_service;

#ifdef APP_ENABLE_SIGNAL_HANDLING
    cs_ptr = &controller_service;
    handle_signal(SIGINT);
    handle_signal(SIGTERM);
#endif

    if(controller_service.init())
    {
        std::cerr << "Failed to init contoller_service" << std::endl;
        controller_service.start();
#ifdef APP_ENABLE_SIGNAL_HANDLING
        controller_service.stop();
#endif
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
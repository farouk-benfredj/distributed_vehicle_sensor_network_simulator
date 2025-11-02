#pragma once
#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vsomeip/vsomeip.hpp>

static constexpr vsomeip_v3::service_t service_id = 0x1111;
static constexpr vsomeip_v3::instance_t service_instance_id = 0x2222;
static constexpr vsomeip_v3::method_t service_method_id = 0x3333;

/**
 * @brief Controller Service class to handle incoming requests.
 *
 */
class controller_service
{
private:
    std::shared_ptr<vsomeip_v3::runtime> rtm_;
    std::shared_ptr<vsomeip_v3::application> app_;
    bool stop_;
    std::mutex mutex_;
    std::thread stop_thread_;
    std::condition_variable condition_;

public:
    controller_service();
    ~controller_service();

    bool init();
    void start();
    void stop();
    void terminate();
    void on_message_cb(const std::shared_ptr<vsomeip_v3::message>& _request);
    void on_state_cb(vsomeip_v3::state_type_e _state);
};

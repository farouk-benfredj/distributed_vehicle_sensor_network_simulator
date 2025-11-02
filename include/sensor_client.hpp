#pragma once
#include <iostream>
#include <random>
#include <thread>
#include <vsomeip/vsomeip.hpp>
#include <vsomeip/handler.hpp>

struct sensor_data {
    double speed;
    double temperature;
    double latitude;
    double longitude;
};

static constexpr vsomeip_v3::service_t sc_service_id = 0x1234;
static constexpr vsomeip_v3::instance_t sc_instance_id = 0x5678;
static constexpr vsomeip_v3::method_t sc_method_id = 0x9ABC; // to-do - unused

/**
 * @brief Sensor Service class to publish mock data (speed, temperature, GPS). 
 *
 */
class sensor_client
{
private:
    std::shared_ptr<vsomeip_v3::application> app_;
    std::shared_ptr<vsomeip_v3::runtime> rtm_;

public:
    sensor_client();
    ~sensor_client();

    bool init();
    void start();
    void stop();
    void on_state_cb(vsomeip_v3::state_type_e _state);
    void on_message_cb(const std::shared_ptr<vsomeip_v3::message>& _response);
    void on_availability_cb(vsomeip_v3::service_t _service, vsomeip_v3::instance_t _instance, bool _is_available);
};

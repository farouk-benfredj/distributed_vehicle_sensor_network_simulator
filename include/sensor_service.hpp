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

/**
 * @brief Sensor Service class to publishe mock data (speed, temperature, GPS). 
 *
 */
class sensor_service
{
private:
    static constexpr vsomeip_v3::service_t SENSOR_SERVICE_ID = 0x1234;
    static constexpr vsomeip_v3::instance_t SENSOR_INSTANCE_ID = 0x5678;
    static constexpr vsomeip_v3::method_t SENSOR_METHOD_ID = 0x9ABC;

    std::shared_ptr<vsomeip::application> app_;
    std::shared_ptr<vsomeip::runtime> rtm_;

public:
    sensor_service(/* args */);
    ~sensor_service();

    bool init();
    void start();
    void stop();
    void on_state_cbk(vsomeip::state_type_e _state);
    void on_message_cbk(const std::shared_ptr<vsomeip::message>& _response);
    void on_availability_cbk(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
};

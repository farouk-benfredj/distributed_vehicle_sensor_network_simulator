#pragma once
#include <iostream>
#include <random>
#include <thread>
#include <vsomeip/vsomeip.hpp>

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

    void init();
    void start();
    void stop();
};

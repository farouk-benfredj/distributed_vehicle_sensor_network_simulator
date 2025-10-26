#include <iostream>
#include <chrono>
#include <sensor_service.hpp>

sensor_service::sensor_service(/* args */){}

sensor_service::~sensor_service(){}

void sensor_service::init()
{
    if(!app_->init())
    {
        std::cerr << "Failed to init application" << std::endl;
    }
}

void sensor_service::start()
{
    app_->start();
}

void sensor_service::stop()
{
    app_->stop();
}

int main(int argc, char* argv[]) {
    sensor_service ss;
    std::cout << "sensor_service started " << argc << " arguments.\n";

    return 0;
}
#include <iostream>
#include <sensor_service.hpp>

int main(int argc, char* argv[]) {
    sensor_service ss;
    std::cout << "sensor_service started " << argc << " arguments.\n";

    return 0;
}
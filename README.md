# distributed_vehicle_sensor_network_simulator
Simulate a small in-vehicle network of ECUs communicating via SOME/IP inside Docker containers.

## Classes
sensor_service → publishes mock data (speed, temperature, GPS).  
controller_service → subscribes, processes data, makes control decisions.  
dashboard_service → listens and displays system status.

                            +-------------------+
                            | sensor_service ECU |
                            |-------------------|
                            | Publishes data:   |
                            |   - Speed         |
                            |   - Temp          |
                            +--------+----------+
                                        |
                                        ▼
                            +-------------------+
                            | controller_service |
                            |-------------------|
                            | Subscribes to:    |
                            |   - Speed         |
                            |   - Temp          |
                            | Publishes:        |
                            |   - Control state |
                            +--------+----------+
                                        |
                                        ▼
                            +-------------------+
                            | dashboard_service  |
                            |-------------------|
                            | Displays all data |
                            | Logs / REST API   |
                            +-------------------+

## Build
```bash
cd distributed_vehicle_sensor_network_simulator
mkdir build && cd build
cmake -B build .
make -j$(nproc)
```

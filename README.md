# distributed_vehicle_sensor_network_simulator
Simulate a small in-vehicle network of ECUs communicating via SOME/IP inside Docker containers.

## Classes
sensor_client → publishes mock data (speed, temperature, GPS).  
controller_service → subscribes, processes data, makes control decisions.  
dashboard_service → listens and displays system status.

                            +-------------------+
                            | sensor_client ECU |
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
#APP_ENABLE_SIGNAL_HANDLING = 1 => Enable clean SIGNAL HANDELING FOR APP (SIGTRM + SIGINT)
cmake -B build . -DAPP_ENABLE_SIGNAL_HANDLING=1 .
make -j$(nproc)
```

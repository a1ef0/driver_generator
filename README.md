# driver_generator
Sample data generator for the AHLS course at IU Summer 2023

## Build
```sh
docker build -f Dockerfile --tag grpc-cmake:1.56.0 .
```

## Run
```sh
docker run -it grpc-cmake:1.56.0 ./build/grpc/data_transfer_api_client <transfer_rate> <remote_snmp_ip>
```

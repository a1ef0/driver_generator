# driver_generator
Sample data generator for the AHLS course at IU Summer 2023

## Build
```sh
docker build -f Dockerfile --tag <insert_your_tag_here> .
```

## Pull docker image
```sh
docker pull ghcr.io/y4cer/snmp_driver_generator:main
```

## Run
```sh
docker run -it <the_tag_you_inserted_or_pulled> ./build/driver <sensor_id><requests per second> <remote_snmp_client_ip> <remote_grpc_server_addr>
```

//
// Created by a1ef0 on 7/11/23.
//

#include <iostream>
#include <thread>

#include "grpc/key_value_service_client.h"
#include "snmp/SNMP_client.h"

void timer_start(std::function<void(void)> func, unsigned int interval) {
    std::thread([func, interval]() {
        while (true) {
            func();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}

void driver_generate(unsigned int rps, const std::string& ip) {
    const char* SERVER_ADDR = "127.0.0.1:1234";
    KeyValueService_client client(
            grpc::CreateChannel(SERVER_ADDR,
                                grpc::InsecureChannelCredentials()));
    unsigned int interval = 1000 / rps;
    SNMPClient(ip, "public").send_request();
    timer_start([&client]() -> void {
                    client.store_value("123");
                }
            , interval);
    while (true);
}

int main(int argc, char** argv) {

    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <requests per second>"
                  << " <remote_snmp_client_ip>" << std::endl;
        return 0;
    }
    long rps = std::stol(argv[1]);
    std::string ip = argv[2];
    driver_generate(rps, ip);

    return 0;
}

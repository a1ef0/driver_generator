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

void driver_generate(unsigned int rps, const std::string& ip,
                     const std::string& grpc_server_addr,
                     const std::string& sensor_name,
                     const std::string& oid = ".1.3.6.1.2.1.1.1.0") {
    const unsigned int interval = 1000 / rps;
    KeyValueService_client client(
            grpc::CreateChannel(grpc_server_addr,
                                grpc::InsecureChannelCredentials()));
    timer_start([&client, &ip, &sensor_name, &oid]() -> void {
        std::string key = "SNMP_" + sensor_name + "_" + ip;
        std::cout << "KEY: " << key << std::endl;
        std::vector<std::string> resp =
                SNMPClient(ip, "public").send_request(oid);
        for (auto& str : resp) {
            std::string val = oid + " = " + str;
            std::cout << "VALUE:" << val << std::endl;
            std::cout << client.store_value(ip, str) << std::endl;
        }} , interval);
    while (true);
}

int main(int argc, char** argv) {

    if (argc != 5) {
        std::cout << "Usage: " << argv[0] << " <sensor_id> " <<
        " <requests per second>" << " <remote_snmp_client_ip>"
        << " <remote_grpc_server_addr>" << std::endl;
        return 0;
    }
    std::string sensor_id = argv[1];
    long rps;
    try {
        rps = std::stol(argv[2]);
    } catch (std::invalid_argument& e) {
        std::cout << "Invalid parameter for rps!\n";
        return 1;
    } catch (std::out_of_range& e) {
        std::cout << "Invalid parameter for rps!\n";
        return 1;
    }
    std::string ip = argv[3];
    std::string grpc_server_addr = argv[4];

    driver_generate(rps, ip, grpc_server_addr, sensor_id);

    return 0;
}

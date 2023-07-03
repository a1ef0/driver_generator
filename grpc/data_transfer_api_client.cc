#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

#include <grpc++/grpc++.h>

#include "data_transfer_api.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

using data_transfer_api::KeyValueService;
using data_transfer_api::Value;
using data_transfer_api::StoreValueResponse;
using data_transfer_api::StoreValueRequest;

// link to storage https://github.com/catdog905/distributed_storage

class KeyValueService_client {
public:
    KeyValueService_client (std::shared_ptr<Channel> channel):
            stub_(KeyValueService::NewStub(channel)) {}

    std::string store_value(const std::string& key, const std::string& payload) {

        StoreValueRequest request;

        Value* val = new Value();
        val->set_payload(payload);

        request.set_allocated_value(val);
        request.set_key(key);

        ClientContext context;
        StoreValueResponse resp;

        Status status = stub_->StoreValue(&context, request, &resp);

        request.clear_value();
        request.clear_key();

        if (status.ok()) {
            return resp.message();
        } else {
            std::cout << status.error_code() << ": " <<
                status.error_message() << std::endl;
            return "gRPC Failed";
        }
    }

private:
    std::unique_ptr<KeyValueService::Stub> stub_;
};


void timer_start(std::function<void(void)> func, unsigned int interval) {
std::thread([func, interval]() {
        while (true)
        {
            func();
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        }
    }).detach();
}

void driver_generate(long rps) {
    const char* SERVER_ADDR = "127.0.0.1:1234";
    KeyValueService_client client(grpc::CreateChannel(SERVER_ADDR,
                                grpc::InsecureChannelCredentials()));
    double interval = 1000 / rps;
    timer_start([&client](){
        client.store_value("123", "payload");
    }
    , interval);
    while (true);
}

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <requests per second>"
                  << std::endl;
        return 0;
    }
    long rps = std::stol(argv[1]);
    driver_generate(rps);

    return 0;
}

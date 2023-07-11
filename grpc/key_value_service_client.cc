//
// Created by gemini on 7/11/23.
//
#include "key_value_service_client.h"

KeyValueService_client::KeyValueService_client (std::shared_ptr<Channel> channel):
stub_(KeyValueService::NewStub(channel)) {
    auto now = std::chrono::system_clock::now();
    this->initial_timestamp = now.time_since_epoch().count();
}

std::string KeyValueService_client::store_value(const std::string& key) {
    StoreValueRequest request;

    Value* val = new Value();
    auto now = std::chrono::system_clock::now();
    double timestamp = now.time_since_epoch().count() -
                       this->initial_timestamp;
    std::cout << timestamp << std::endl;
    double sin_value = sin(timestamp);
    val->set_payload(std::to_string(sin_value));

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
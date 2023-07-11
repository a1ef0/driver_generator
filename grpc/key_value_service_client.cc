//
// Created by gemini on 7/11/23.
//
#include "key_value_service_client.h"

KeyValueService_client::KeyValueService_client
(std::shared_ptr<Channel> channel): stub_(KeyValueService::NewStub(channel)) {}

std::string KeyValueService_client::store_value(const std::string& key,
                                                const std::string& payload) {
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
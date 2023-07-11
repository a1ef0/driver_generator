#include <iostream>

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
    KeyValueService_client (std::shared_ptr<Channel>);

    std::string store_value(const std::string&, const std::string&);

private:
    double initial_timestamp;
    std::unique_ptr<KeyValueService::Stub> stub_;
};
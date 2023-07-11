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

/**
 * Initializes grpc client and sends rpc function calls to store the data.
 */
class KeyValueService_client {
public:
    KeyValueService_client (std::shared_ptr<Channel>);

    /**
     * Store a value with the given key into the remote storage. RPC call.
     * @return string status message
     */
    std::string store_value(const std::string&, const std::string&);

private:
    std::unique_ptr<KeyValueService::Stub> stub_;
};
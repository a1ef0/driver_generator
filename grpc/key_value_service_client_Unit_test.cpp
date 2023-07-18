//
// Created by anastasia on 12.07.23.
//
#include "key_value_service_client.h"
#include "data_transfer_api.grpc.pb.h"
#include "data_transfer_api.pb.h"

#include <gtest/gtest.h>
#include <iostream>
#include <gmock/gmock.h>
//#include <gmock/gmock.h>
//#include <string.h>


using ::testing::_;
using ::testing::Invoke;
using ::testing::Return;


class MockKeyValueService : public data_transfer_api::KeyValueService::Service {
public:
    MOCK_METHOD(grpc::Status, StoreValue, (grpc::ServerContext* context,
            const data_transfer_api::StoreValueRequest* request,
            data_transfer_api::StoreValueResponse* response), (override));
    // Add mock methods for other service functions if needed
};

class MockTest : public ::testing::Test {
protected:
    MockTest() {
        int port = 8000;
        server_address_ << "localhost:" << port;
        // Setup server
        grpc::ServerBuilder builder;
        builder.AddListeningPort(server_address_.str(),
                                 grpc::InsecureServerCredentials());
        service_ = std::make_shared<MockKeyValueService>();
        builder.RegisterService(service_.get());
        server_ = builder.BuildAndStart();

        std::shared_ptr<::grpc::Channel> channel = server_->InProcessChannel(::grpc::ChannelArguments());
        client_ = std::make_unique<KeyValueService_client>(channel);

    }
    void TearDown() override { server_->Shutdown(); }
    std::unique_ptr<KeyValueService_client> client_;
    std::shared_ptr<MockKeyValueService> service_;
    std::ostringstream server_address_;
    std::unique_ptr<grpc::Server> server_;
};

TEST_F(MockTest, StoreValueTest){
    std::string key = "SomeKey";
    std::string payload = "FL=";
    EXPECT_CALL(*service_, StoreValue(_,_,_))
    .WillOnce([](grpc::ServerContext*, const data_transfer_api::StoreValueRequest* request, data_transfer_api::StoreValueResponse* response) {
        // Perform any necessary checks on the request
        EXPECT_EQ(request->key(), "SomeKey");
        EXPECT_EQ(request->value().payload(), "FL=");

        // Set response values if needed
        response->set_message("Item has been successfully stored");
        return grpc::Status::OK;
    });
    std::string result = client_->store_value(key, payload);

    EXPECT_EQ(result, "Item has been successfully stored");
}

// Additional test cases can be added for other methods or scenarios

// Entry point for running the tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

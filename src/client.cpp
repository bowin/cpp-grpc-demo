/*
 *
 * Copyright 2015 gRPC authors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <iostream>
#include <memory>
#include <string>
#include <cstdint>

#include <grpcpp/grpcpp.h>
#include <grpc/support/log.h>

#ifdef BAZEL_BUILD
#include "examples/protos/helloworld.grpc.pb.h"
#else

#include "helloworld.grpc.pb.h"

#endif

using google::protobuf::Descriptor;
using grpc::Channel;
using grpc::ClientAsyncResponseReader;
using grpc::ClientContext;
using grpc::CompletionQueue;
using grpc::Status;
using helloworld::HelloRequest;
using helloworld::HelloReply;
using helloworld::Greeter;
using helloworld::Test;

class GreeterClient {
public:
    explicit GreeterClient(std::shared_ptr<Channel> channel)
            : stub_(Greeter::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string SayHello(const std::string &user) {
        // Data we are sending to the server.
        HelloRequest request;
        request.set_name(user);

        // Container for the data we expect from the server.
        HelloReply reply;

        // Context for the client. It could be used to convey extra information to
        // the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The producer-consumer queue we use to communicate asynchronously with the
        // gRPC runtime.
        CompletionQueue cq;

        // Storage for the status of the RPC upon completion.
        Status status;

        // stub_->PrepareAsyncSayHello() creates an RPC object, returning
        // an instance to store in "call" but does not actually start the RPC
        // Because we are using the asynchronous API, we need to hold on to
        // the "call" instance in order to get updates on the ongoing RPC.
        std::unique_ptr<ClientAsyncResponseReader<HelloReply> > rpc(
                stub_->PrepareAsyncSayHello(&context, request, &cq));

        // StartCall initiates the RPC call
        rpc->StartCall();

        // Request that, upon completion of the RPC, "reply" be updated with the
        // server's response; "status" with the indication of whether the operation
        // was successful. Tag the request with the integer 1.
        rpc->Finish(&reply, &status, (void *) 1);
        void *got_tag;
        bool ok = false;
        // Block until the next result is available in the completion queue "cq".
        // The return value of Next should always be checked. This return value
        // tells us whether there is any kind of event or the cq_ is shutting down.
        GPR_ASSERT(cq.Next(&got_tag, &ok));

        // Verify that the result from "cq" corresponds, by its tag, our previous
        // request.
        GPR_ASSERT(got_tag == (void *) 1);
        // ... and that the request was completed successfully. Note that "ok"
        // corresponds solely to the request for updates introduced by Finish().
        GPR_ASSERT(ok);

        // Act upon the status of the actual RPC.
        if (status.ok()) {
            return reply.message();
        } else {
            return "RPC failed";
        }
    }

private:
    // Out of the passed in Channel comes the stub, stored here, our view of the
    // server's exposed services.
    std::unique_ptr<Greeter::Stub> stub_;
};

#define max(a, b) ( (a) > (b) ? (a) : (b))

#define mf(v, f) (v.f)

//
template<typename T>
void println(std::string &&s, T a) {
    std::cout << s << ": " << a << std::endl;
}

int main(int argc, char **argv) {
    // Instantiate the client. It requires a channel, out of which the actual RPCs
    // are created. This channel models a connection to an endpoint (in this case,
    // localhost at port 50051). We indicate that the channel isn't authenticated
    // (use of InsecureChannelCredentials()).
//    GreeterClient greeter(grpc::CreateChannel(
//            "localhost:50051", grpc::InsecureChannelCredentials()));
//    std::string user("world");
//    std::string reply = greeter.SayHello(user);  // The actual RPC call!
//    std::cout << "Greeter received: " << reply << std::endl;

    // pb ser & deser
    helloworld::Test t;
    t.set_name("ss");
    t.set_age(-11);
    std::string out;
    t.SerializeToString(&out);
    std::cout << out << std::endl;
    Test t2;
    t2.ParseFromString(out);

    {
        // cast example
        char *chs = const_cast<char *> (reinterpret_cast<const char *>(out.data()));
        auto *d2 = new uint8_t[out.size()];
        ::memcpy(d2, chs, out.size());
        println("d2", d2);
        println("chs", chs);
    }

    {
        // macro
        auto xx = mf(t2, age)();
        decltype(mf(t2, name)()) x = mf(t2, name)();
        auto match = std::is_same<int32_t, decltype(mf(t2, age)())>::value == true;
        println("xx", match);
    }


    {
        // lambda invoke
        auto f = [](int a, int b) { return a + b; };
        auto v = std::invoke(f, 1, 2);
        std::cout << v << std::endl;
    }
    {
        char a = 12;
        char b = -26;
        uint8_t c = b;
        char d = c;
        printf("%d, %d, %d, %d, ", a, b, c, d);
    }

    return 0;
}

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "collector.pb.h"
#include "collector.grpc.pb.h"

class YuzuClient
{
    std::unique_ptr<YuzuCollector::Stub> _stub;
    std::string _app_name;

public:
    YuzuClient()
    {
        _app_name = std::getenv("APP_NAME");

        std::shared_ptr<grpc::Channel> channel =
            grpc::CreateChannel(std::getenv("MASTER_ADDR"),
                                grpc::InsecureChannelCredentials());

        _stub = YuzuCollector::NewStub(channel);
    }

    void send(int step, double time, TimerType type)
    {
        grpc::ClientContext context;

        TimerTelemetry telemetry;
        telemetry.mutable_common()->set_app_name(_app_name);
        telemetry.mutable_common()->set_step(step);
        telemetry.set_duration(time);
        telemetry.set_timer_type(type);

        TelemetryReply reply;

        grpc::Status status = _stub->ReportTimer(&context, telemetry, &reply);

        if (!status.ok()) {
            std::cerr << "gRPC request failed: " << status.error_message() << std::endl;
        }
    }
};


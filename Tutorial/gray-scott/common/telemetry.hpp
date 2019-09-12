#include <iostream>

#include <nlohmann/json.hpp>
#include <zmq.hpp>

class Telemetry
{
    zmq::context_t _ctx;
    zmq::socket_t _sock(ctx, zma::socket_type::rep);

    Telemetry()
    {
        _sock.connect("tcp://*:5555");
    }

    ~Telemetry()
    {
        _sock.destroy();
        _ctx.destroy();
    }

    void send(const std::string &name, double time)
    {
        zmq::message_t req;
    }
};

#include <cstdlib>
#include <iostream>

#include <nlohmann/json.hpp>
#include <zmq.hpp>

class Telemetry
{
protected:
    std::string _app_name;
    zmq::context_t _ctx;
    zmq::socket_t _sock;

public:
    Telemetry(const std::string &app_name)
        : _sock(_ctx, zmq::socket_type::req), _app_name(app_name)
    {
        const char *addr = std::getenv("CONTROLLER_ADDR");

        _sock.connect(addr);
    }

    void send(const std::string &type, int step, double time)
    {
        nlohmann::json j;
        j["app"] = _app_name;
        j["type"] = type;
        j["time"] = time;
        j["step"] = step;

        const std::string s(j.dump());

        zmq::message_t req(s.begin(), s.end());
        _sock.send(req);

        zmq::message_t resp;
        _sock.recv(resp);
    }
};

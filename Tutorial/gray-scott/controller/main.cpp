#include <iostream>

#include <mpi.h>
#include <nlohmann/json.hpp>
#include <zmq.hpp>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);
    int rank, procs, wrank;

    MPI_Comm_rank(MPI_COMM_WORLD, &wrank);

    const unsigned int color = 999;
    MPI_Comm comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, wrank, &comm);

    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &procs);

    zmq::context_t ctx;
    zmq::socket_t sock(ctx, zmq::socket_type::rep);

    sock.bind("tcp://*:5555");

    std::cout << "Bind successful." << std::endl;

    while (true) {
        zmq::message_t request;

        sock.recv(&request);

        const std::string msg(request.data<char>(), request.size());

        auto j = nlohmann::json::parse(msg);
        std::cout << j << std::endl;

        const std::string s("ack");
        zmq::message_t reply(s.begin(), s.end());


        sock.send(reply);
    }

    MPI_Finalize();

    return 0;
}

#include "client.h"

BlockingTcpClient::BlockingTcpClient(std::string host, uint16_t port)
    : resolver_(io_context_), socket_(io_context_)
{
    ConnectTo(host, port);
}

void BlockingTcpClient::Send(const std::string& data)
{
    Send(data.c_str(), data.size());
}

void BlockingTcpClient::Send(const char* buf, size_t bytes)
{
    boost::asio::write(socket_, boost::asio::buffer(buf, bytes));
}

std::string BlockingTcpClient::Recv(size_t bytes)
{
    std::string replay;
    replay.resize(bytes);
    auto bytes_read = Recv(replay.data(), replay.size());
    replay.resize(bytes_read);
    return replay;
}

size_t BlockingTcpClient::Recv(char* buf, size_t limit)
{
    return boost::asio::read(socket_, boost::asio::buffer(buf, limit));
}

void BlockingTcpClient::ConnectTo(const std::string& host, uint16_t port)
{
    auto addr = resolver_.resolve(host, std::to_string(port));
    boost::asio::connect(socket_, addr);
}

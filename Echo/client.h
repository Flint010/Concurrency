#pragma once

#include <cstdlib>
#include <string>
#include <utility>

#include <boost/asio.hpp>

class BlockingTcpClient
{
public:
	BlockingTcpClient(std::string host, uint16_t port);

	void Send(const std::string& data);
	void Send(const char* buf, size_t bytes);

	std::string Recv(size_t bytes);
	size_t Recv(char* buf, size_t limit);

private:
	void ConnectTo(const std::string& host, uint16_t port);

private:
	boost::asio::io_context io_context_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;
};


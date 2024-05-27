#pragma once

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
private:
	static const size_t kBufCapacity = 1024;

public:
	explicit TcpConnection(tcp::socket socket);
	~TcpConnection();

	void Start();

private:
	void ReadChunk();

	void WriteChunk(size_t bytes);

private:
	tcp::socket socket_;
	char buf_[kBufCapacity];
};

class EchoServer {
public:
	EchoServer(boost::asio::io_context& io_context, uint16_t port);
	~EchoServer();

private:
	void AcceptClient();

private:
	tcp::acceptor acceptor_;
};
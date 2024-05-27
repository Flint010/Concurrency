#include "server.h"
#include <iostream>

TcpConnection::TcpConnection(tcp::socket socket)
	: socket_(std::move(socket))
{
	std::cout << "TcpConnection(): " << std::this_thread::get_id() << std::endl;
}

TcpConnection::~TcpConnection()
{
	std::cout << "~TcpConnection(): " << std::this_thread::get_id() << std::endl;
}

void TcpConnection::Start()
{
	std::cout << "Start(): " << std::this_thread::get_id() << std::endl;
	ReadChunk();
}

void TcpConnection::ReadChunk()
{
	std::cout << "ReadChunk(): " << std::this_thread::get_id() << std::endl;
	auto self = shared_from_this();
	socket_.async_read_some(
		boost::asio::buffer(buf_, kBufCapacity),
		[this, self](std::error_code error_code, size_t bytes_read) {
			std::cout << "async_read_some(): " << std::this_thread::get_id() << std::endl;
			if (!error_code) {
				WriteChunk(bytes_read);
			}
		}
	);
}

void TcpConnection::WriteChunk(size_t bytes)
{
	std::cout << "WriteChunk(): " << std::this_thread::get_id() << std::endl;
	auto self = shared_from_this();
	boost::asio::async_write(socket_, boost::asio::buffer(buf_, kBufCapacity),
		[this, self](std::error_code error_code, size_t bytes_write) {
			std::cout << "async_write(): " << std::this_thread::get_id() << std::endl;
			if (!error_code) {
				ReadChunk();
			}			
		}
	);
}

EchoServer::EchoServer(boost::asio::io_context& io_context, uint16_t port)
	: acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
{
	std::cout << "EchoServer(): " << std::this_thread::get_id() << std::endl;
	AcceptClient();
}

EchoServer::~EchoServer()
{
	std::cout << "~EchoServer(): " << std::this_thread::get_id() << std::endl;
}

void EchoServer::AcceptClient()
{
	std::cout << "AcceptClient(): " << std::this_thread::get_id() << std::endl;

	acceptor_.async_accept(
		[this](std::error_code error_code, tcp::socket client_socket) {
			std::cout << "async_accept(): " << std::this_thread::get_id() << std::endl;

			if (!error_code) {
				std::make_shared<TcpConnection>(std::move(client_socket))->Start();
			}
			AcceptClient();
		}
	);
}

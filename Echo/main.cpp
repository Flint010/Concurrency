#include <iostream>
#include <boost/asio.hpp>
#include <thread>
#include <exception>

#include "client.h"
#include "server.h"

static const std::string kServerHost = "localhost";
static const uint16_t kServerPort = 54674;

void LaunchEchoServer() {
	auto running = []() {
		boost::asio::io_context io_context;
		uint16_t port = kServerPort;
		EchoServer server(io_context, port);
		io_context.run();
		};
	std::thread t1(running);
	t1.detach();
}

std::string Echo(BlockingTcpClient& client, const std::string& data) {
	client.Send(data);
	return client.Recv(data.length());
}

int main() {
	using std::cout;
	using std::endl;


	std::thread t1(LaunchEchoServer);
	t1.detach();
	
	BlockingTcpClient client(kServerHost, kServerPort);
	std::cout << "Hello" << std::endl;

	std::string str1 = Echo(client, "Hello");
	std::string str2 = Echo(client, "Hi");
	std::string str3 = Echo(client, "Hello World");


	/*try {
		cout << Echo(client, "Hello") << endl;
		cout << Echo(client, "Hi") << endl;
		cout << Echo(client, "Hello World") << endl;
	}
	catch (const std::exception& e) {
		cout << "Exception: " << e.what() << endl;
	}*/
	
	return 0;
}
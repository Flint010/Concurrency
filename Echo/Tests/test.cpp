#include "gtest/gtest.h"
#include <chrono>
#include "server.h"
//#include "server.cpp"
#include "client.h"
//#include "client.cpp"



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

TEST(EchoServerTest, ServerLaunched) {
	EXPECT_TRUE(true);
}

//TEST(EchoServerTest, Hello) {
//
//	BlockingTcpClient client(kServerHost, kServerPort);
//	EXPECT_EQ(Echo(client, "Hello"), "Hello");
//	EXPECT_EQ(Echo(client, "Good morning"), "Good morning");
//}

//TEST(EchoServerTest, HelloWorld) {
//    BlockingTcpClient client(kServerHost, kServerPort);
//
//    client.Send("Hello");
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    client.Send(", World");
//    EXPECT_EQ(client.Recv(12), "Hello, World");
//
//    // No extra data from server
//    EXPECT_EQ(Echo(client, "!"), "!");
//}

TEST(EchoServerTest, TwoClients) {
    BlockingTcpClient finn(kServerHost, kServerPort);
    BlockingTcpClient jake(kServerHost, kServerPort);

    finn.Send("Hi, Jake!");
    jake.Send("Hi, Finn!");

    std::this_thread::sleep_for(std::chrono::seconds(1));

    EXPECT_EQ(finn.Recv(9), "Hi, Jake!");
    EXPECT_EQ(jake.Recv(9), "Hi, Finn!");
}
//
TEST(EchoServerTest, ConcurrentClients) {
    static const size_t kClients = 17;

    std::list<BlockingTcpClient> clients;
    for (size_t i = 0; i < kClients; ++i) {
        clients.emplace_back(kServerHost, kServerPort);
    }

    static const std::string kMessage = "Merry Cristmas!";

    for (auto& client : clients) {
        client.Send(kMessage);
    }

    for (auto& client : clients) {
        EXPECT_EQ(client.Recv(kMessage.length()), kMessage);
    }
}


int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);

	std::cout << "main thread: " << std::this_thread::get_id() << std::endl;

	LaunchEchoServer();

	RUN_ALL_TESTS();
}
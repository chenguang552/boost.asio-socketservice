#include <boost/asio.hpp>
#include <iostream>
int main()
{
    try {
        std::cout << "server start." << std::endl;
    // asio程序必须的io_service对象
        boost::asio::io_service ios;
    // 具体的服务器地址与端口
        boost::asio::ip::tcp::endpoint endpotion(boost::asio::ip::tcp::v4(), 13695);
    // 创建acceptor对象，当前的IPV4作为服务器地址(127.0.0.1 || 0.0.0.0)，接受端口13695的消息.
        boost::asio::ip::tcp::acceptor acceptor(ios, endpotion);
    // 打印当前服务器地址
        std::cout << "addr: " << acceptor.local_endpoint().address() << std::endl;
    // 打印当前服务器端口
        std::cout << "port: " << acceptor.local_endpoint().port() << std::endl;
    // 循环执行服务
    // 一个临时的socket对象
        boost::asio::ip::tcp::socket socket(ios);
    // 阻塞等待客户端连接，连接成功后返回socket, accept这个函数使用引用来调取socket.
        acceptor.accept(socket);
        while (true) {
        // 打印与本机服务器取得连接的客户端IP地址
        //    std::cout << "client: " << socket.remote_endpoint().address() << std::endl;
        //////////////////////////////处理/////////////////////////////////
            std::string msg;
        // 阻塞发送作者名称到客户端
        //    socket.write_some(boost::asio::buffer("hello CSND_Ayo"));
        // 阻塞接收客户端发来的数据
            socket.read_some(boost::asio::buffer(msg));
        // 打印客户端发来的数据
            std::cout << "client reply: " << msg.c_str() << std::endl;
        }
    }
    catch (std::exception& _e) {
        std::cout << "server exceptional." << std::endl;
        std::cout << _e.what() << std::endl;
    }
    std::cout << "server end." << std::endl;
    getchar();
    return 0;
}

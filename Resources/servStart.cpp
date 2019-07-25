#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "service.h"

int main(void)
{
    try
    {
        std::cout << "server start." << std::endl;
        // 建造服务对象
        boost::asio::io_service ios;
        // 具体的服务器地址与端口
        boost::asio::ip::tcp::endpoint endpotion(ip::address_v4::from_string("192.168.22.160"), 10086);
        // 构建Server实例
        CService server(ios, endpotion);
        // 启动异步调用事件处理循环
        server.run();
    }
    catch (std::exception& _e)
    {
        std::cout << _e.what() << std::endl;
    }
    std::cout << "server end." << std::endl;
    return 0;
}

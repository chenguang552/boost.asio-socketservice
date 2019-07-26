#include <iostream>
#include <boost/asio.hpp>
#include "client.h"

int main(void)
{
    try
    {
        std::cout << "client start." << std::endl;
        // ����������
        boost::asio::io_service iClient;
        // ����ķ�������ַ��˿�
        boost::asio::ip::tcp::endpoint endpotion(boost::asio::ip::address_v4::from_string("192.168.22.160"), 10086);
        // ����clientʵ��
        CClient client(iClient, endpotion);
    }
    catch (std::exception& _e)
    {
        std::cout << _e.what() << std::endl;
    }
    std::cout << "client end." << std::endl;
    return 0;
}

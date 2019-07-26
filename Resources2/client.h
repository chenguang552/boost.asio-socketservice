#ifndef CLIENT
#define CLIENT

#include <boost/asio.hpp>
#include <iostream>
class CClient {

private:
    void start();

public:
    CClient(boost::asio::io_service& ioClient, boost::asio::ip::tcp::endpoint& endPoint);
    ~CClient();

private:
    boost::asio::io_context& mIoContext;
    boost::asio::ip::tcp::socket mSocket;

public:
};

#endif //!CLIENT

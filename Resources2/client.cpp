#include "client.h"

CClient::CClient(boost::asio::io_service& ioService, boost::asio::ip::tcp::endpoint& endPoint)
{
    std::shared_ptr<boost::asio::ip::tcp::socket> sock(
        new boost::asio::ip::tcp::socket(ioc, ep.protocol()));
}

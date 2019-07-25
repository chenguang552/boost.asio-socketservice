#ifndef CLIENT
#define CLIENT

#include <boost/asio.hpp>

class CClient {

private:
public:
    CClient(boost::asio::io_service& ioService, boost::asio::ip::tcp::endpoint& endPoint);
    ~CClient();

private:
    boost::asio::io_context mIoContext;

public:
};

#endif //!CLIENT

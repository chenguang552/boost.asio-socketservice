#include "client.h"

CClient::CClient(boost::asio::io_service& ioClient, boost::asio::ip::tcp::endpoint& endPoint)
    : mIoContext(ioClient)
    , mSocket(mIoContext)
{
    mSocket.connect(endPoint);
    start();
}

CClient::~CClient()
{
}

void CClient::start()
{
    std::string strIn;

    char inBuf[512] = { 0 };
    while (1) {
        strIn.clear();
        std::cin >> strIn;
        memcpy(inBuf, strIn.c_str(), strIn.length());
        boost::asio::write(mSocket, boost::asio::buffer(inBuf));
    }
}

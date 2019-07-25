#ifndef SERVICE
#define SERVICE
#include <iostream>
#include <string.h>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "session2.h"
using namespace boost;
using namespace boost::asio;

class CService {

private:
    // 会话 - 智能指针
    typedef boost::shared_ptr<CSession2> session_ptr;

public:
    CService(boost::asio::io_service& ioService, boost::asio::ip::tcp::endpoint& endPoint);

    virtual ~CService(void);

    // 监听
    void start(void);

    // 异步
    void run(void);

private:
    // 数据导出接口
    void callback_session(std::string _fromIp, std::string _info);

    // 会话启动
    void accept_handler(session_ptr chatSession, const boost::system::error_code& errorCode);

private:
    io_service& ioService;

    ip::tcp::acceptor tcpAcceptor;
};
#endif //!SERVICE

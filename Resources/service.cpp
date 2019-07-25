#include "service.h"

CService::CService(boost::asio::io_service& ioService, boost::asio::ip::tcp::endpoint& endPoint)
    : ioService(ioService)
    , tcpAcceptor(ioService, endPoint)
{
    start();
}

CService::~CService(void)
{
}

void CService::start(void)
{
    session_ptr new_chat_session(new CSession2(ioService));

    // 传入会话socket句柄以及连接后回调函数的绑定，bind参数为(回调函数、上下文、参数1....n)
    tcpAcceptor.async_accept(new_chat_session->socket(),
                             boost::bind(&CService::accept_handler, this, new_chat_session, boost::asio::placeholders::error));
}

void CService::run(void)
{
    ioService.run();
}

void CService::callback_session(std::string /*_fromIp*/, std::string /*_info*/)
{
    return;
}

// 会话处理回调函数
void CService::accept_handler(session_ptr chatSession, const boost::system::error_code& errorCode)
{
    if (!errorCode && chatSession) {
        try
        {
            chatSession->start();
            start();
        }
        catch (...)
        {
            return;
        }
    }
}

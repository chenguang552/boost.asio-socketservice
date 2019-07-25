#ifndef SESSION2
#define SESSION2
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#define REPLY_SIZE (32)

// 会话类 传递一个指向自身的share_ptr给异步函数，这样在异步函数执行期间share_ptr所管理的对象就不会析构
class CSession2 : public boost::enable_shared_from_this<CSession2> {

public:
    typedef void pSessionCallback(std::string, std::string);

public:
    CSession2(boost::asio::io_service& ioService);
    virtual ~CSession2(void);

    void start(void);

    void setCallback(pSessionCallback* callBack)
    {
        mCallBack = callBack;
    }

    // socket 实例
    boost::asio::ip::tcp::socket& socket(void);

private:
	
    // 读取成功后触发的函数
    void read_handler(const boost::system::error_code& errorCode);

private:
	
    // 临时信息缓冲区
    char cData[512];
    
    std::string strMsg;
    
    boost::asio::ip::tcp::socket mSocket;
    	
    // 回调
    pSessionCallback* mCallBack;
};
#endif //!SESSION2
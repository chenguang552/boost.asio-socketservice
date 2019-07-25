#ifndef SESSION
#define SESSION
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#define REPLY_SIZE (32)

// 会话类 传递一个指向自身的share_ptr给异步函数，这样在异步函数执行期间share_ptr所管理的对象就不会析构
class CSession : public boost::enable_shared_from_this<CSession> {

public:
    typedef void pSessionCallback(std::string, std::string);

public:
    CSession(boost::asio::io_service& ioService);
    virtual ~CSession(void);

    void start(void);

    void setCallback(pSessionCallback* callBack)
    {
        mCallBack = callBack;
    }

    // socket 实例
    boost::asio::ip::tcp::socket& socket(void);

private:
    // 第一个协议包
    void init_handler(const boost::system::error_code& errorCode);

    // 解析协议包
    void analyse_handler(const boost::system::error_code& errorCode);

    // 完成数据传输后触发的收尾工作
    void done_handler(const boost::system::error_code& errorCode);

    // 读取成功后触发的函数
    void read_handler(const boost::system::error_code& errorCode, size_t stReadSize);

    // 写入完成后触发的函数
    void write_handler(const boost::system::error_code& errorCode);

		void done_toStart(const boost::system::error_code& errorCode);

    int wsHandshake(std::string &request, std::string &response);

private:
    // 临时信息缓冲区
    //char msg[4];
    char cData[512];
    std::string strCurrentMsg;

    // 数据总数量
    int iSumSize;
//    // 单个数据包大小
//    unsigned int uiMaxSize;
//    // socket句柄
    boost::asio::ip::tcp::socket mSocket;
    // 回调
    pSessionCallback* mCallBack;
};
#endif //!SESSION

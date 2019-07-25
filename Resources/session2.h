#ifndef SESSION2
#define SESSION2
#include <iostream>
#include <string>

#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#define REPLY_SIZE (32)

// �Ự�� ����һ��ָ�������share_ptr���첽�������������첽����ִ���ڼ�share_ptr������Ķ���Ͳ�������
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

    // socket ʵ��
    boost::asio::ip::tcp::socket& socket(void);

private:
	
    // ��ȡ�ɹ��󴥷��ĺ���
    void read_handler(const boost::system::error_code& errorCode);

private:
	
    // ��ʱ��Ϣ������
    char cData[512];
    
    std::string strMsg;
    
    boost::asio::ip::tcp::socket mSocket;
    	
    // �ص�
    pSessionCallback* mCallBack;
};
#endif //!SESSION2
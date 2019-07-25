#include "session.h"

CSession::CSession(boost::asio::io_service& ioService)
    : mSocket(ioService)
{
    memset(cData, 0, sizeof(cData));
}

CSession::~CSession(void)
{
}

void CSession::start(void)
{
    // 告诉链接成功的客户端数据发送格式。
    char cMsg[32] = "packet count:";
    iSumSize = 0;
    boost::asio::async_write(mSocket, boost::asio::buffer(cMsg, strlen(cMsg)),
                             boost::bind(&CSession::init_handler, shared_from_this(),
                                         boost::asio::placeholders::error));
}

boost::asio::ip::tcp::socket& CSession::socket(void)
{
    return mSocket;
}

// 第一个协议包
void CSession::init_handler(const boost::system::error_code& errorCode)
{
    if (errorCode) {
        return;
    }
    // 获取头部信息，确定数据包总数
    boost::asio::async_read(mSocket, boost::asio::buffer(cData,4),
                            boost::bind(&CSession::analyse_handler, shared_from_this(),
                                        boost::asio::placeholders::error));
}

void CSession::analyse_handler(const boost::system::error_code& errorCode)
{
    if (errorCode) {
        return;
    }
    // 分析协议包格式，根据不同的数据头确定其所用协议
    //switch（msg）{case xxx:   ;break;   default:break;}
    
    // 暂定无属性头，截取的头部为数据包总数
    bool bflag = true;

    // do something.
    if (!bflag) {
        start();
        return;
    }

    // 格式化保存协议包数据
    //std::stringstream io(msg);
    std::stringstream io(cData);
//    io >> uiMaxSize;
    io >> iSumSize;


    // 发送接收请求信息
    char cMsg[REPLY_SIZE];
    sprintf(cMsg, "确认数据包数量为：%d\n", iSumSize);
    //sprintf(cMsg, response.c_str(),response.length());
    std::cout<<"收到数据发送请求，数据包总数："<<iSumSize<<std::endl;
    boost::asio::async_write(mSocket, boost::asio::buffer(cMsg, REPLY_SIZE),
                             boost::bind(&CSession::write_handler, shared_from_this(),
                                         boost::asio::placeholders::error));
}

// 完成数据传输
void CSession::done_handler(const boost::system::error_code& errorCode)
{
    if (errorCode) {
        return;
    }
    strCurrentMsg += cData;
    // 发送信息到回调
//    if (!strCurrentMsg.empty() && mCallBack != NULL) {
//        mCallBack(mSocket.remote_endpoint().address().to_string(), strCurrentMsg);
//        strCurrentMsg.clear();
//    }

		std::cout<<"接收到数据：\n"<<strCurrentMsg<<std::endl;
    memset(cData, 0, sizeof(cData));

    char cMsg[32] = "message recv done.\n";
    boost::asio::async_write(mSocket, boost::asio::buffer(cMsg, REPLY_SIZE),
                             boost::bind(&CSession::done_toStart, shared_from_this(),
                                         boost::asio::placeholders::error));
}

void CSession::done_toStart(const boost::system::error_code& errorCode)
{
    if (errorCode) {
        return;
    }
    start();
}

void CSession::read_handler(const boost::system::error_code& errorCode, size_t stReadSize)
{
    if (errorCode) {
        return;
    }
    // 数据处理
//    strCurrentMsg += msg;
//    if (strCurrentMsg.size() > 1024 * 512) {
//        // 发送信息到回调
//        if (mCallBack != NULL) {
//            mCallBack(mSocket.remote_endpoint().address().to_string(), strCurrentMsg);
//            strCurrentMsg.clear();
//        }
//    }
//    memset(msg, 0, sizeof(msg));

		strCurrentMsg += cData;
	  memset(cData, 0, sizeof(cData));
    // 计算当前剩余数据数量
//    iSumSize -= stReadSize;

    // 接收完成
//    if (0 > iSumSize) {
		--iSumSize;
		if (iSumSize <= 0) {
        done_handler(errorCode);
    }
    // 继续接收
    else {
        char cMsg[REPLY_SIZE];
        sprintf(cMsg, "还剩余数据包:%d\n", iSumSize);
        boost::asio::async_write(mSocket, boost::asio::buffer(cMsg, REPLY_SIZE),
                                 boost::bind(&CSession::write_handler, shared_from_this(),
                                             boost::asio::placeholders::error));

        std::cout << "还剩余数据包:" << iSumSize << std::endl;
    }
}
void CSession::write_handler(const boost::system::error_code& errorCode)
{
    if (errorCode) {
        return;
    }

//    boost::asio::async_read(mSocket, boost::asio::buffer(msg, 512),
    	boost::asio::async_read(mSocket, boost::asio::buffer(cData,512),
                            boost::bind(&CSession::read_handler, shared_from_this(),
                                        boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

int wsHandshake(std::string &request, std::string &response)
{
    // 解析http请求头信息
    int ret = 0;/*
    std::istringstream stream(request.c_str());
    std::string reqType;
    std::getline(stream, reqType);
    if (reqType.substr(0, 4) != "GET ")
    {
        return ret;
    }

    std::string header;
    std::string::size_type pos = 0;
    std::string websocketKey;
    while (std::getline(stream, header) && header != "\r")
    {
        header.erase(header.end() - 1);
        pos = header.find(": ", 0);
        if (pos != std::string::npos)
        {
            std::string key = header.substr(0, pos);
            std::string value = header.substr(pos + 2);
            if (key == "Sec-WebSocket-Key")
            {
                ret = WS_STATUS_CONNECT;
                websocketKey = value;
                break;
            }
        }
    }

    if (ret != WS_STATUS_CONNECT)
    {
        return ret;
    }

    // 填充http响应头信息
    response = "HTTP/1.1 101 Switching Protocols\r\n";
    response += "Upgrade: websocket\r\n";
    response += "Connection: upgrade\r\n";
    response += "Sec-WebSocket-Accept: ";

    const std::string magicKey("258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
    std::string serverKey = websocketKey + magicKey;

    char shaHash[32];
    memset(shaHash, 0, sizeof(shaHash));
    sha1::calc(serverKey.c_str(), serverKey.size(), (unsigned char *) shaHash);
    serverKey = base64::base64_encode(std::string(shaHash)) + "\r\n\r\n";
    string strtmp(serverKey.c_str());
    response += strtmp;
*/
    return ret;
}
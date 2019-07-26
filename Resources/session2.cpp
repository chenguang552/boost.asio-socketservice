#include "session2.h"

CSession2::CSession2(boost::asio::io_service& ioService)
    : mSocket(ioService)
{
    memset(cData, 0, sizeof(cData));
    strMsg = "";
}

CSession2::~CSession2(void)
{
}

void CSession2::start(void)
{
    memset(cData, 0, sizeof(cData));
    mSocket.async_read_some(boost::asio::buffer(cData),
                            boost::bind(&CSession2::read_handler, shared_from_this(),
                                        boost::asio::placeholders::error));
}

boost::asio::ip::tcp::socket& CSession2::socket(void)
{
    return mSocket;
}

void CSession2::read_handler(const boost::system::error_code& errorCode)
{
    if (errorCode) {
        return;
    }
    std::string strData(cData);
    if (strData.find("{[>over<]}") != std::string::npos) {
        // 结束
        std::cout << "over  >>  " << strData.find("{[>over<]}") << std::endl;
        strMsg += strData.substr(0, strData.find("{[>over<]}"));
        std::cout << "DATA: \n" << strMsg << std::endl;
        mSocket.close();
    } else {
        if (strData.substr(0, 3) == "GET") {
            // do get
            std::cout << "get" << std::endl;
            std::cout << "DATA: \n" << cData << std::endl;
            parseHttpGet(errorCode, "GET", strData);
            start();
            return;
        }

        if (strData.substr(0, 4) == "POST") {
            // do post
            std::cout << "post" << std::endl;
            std::cout << "DATA: \n" << cData << std::endl;
            parseHttpPost(errorCode, "POST", strData);
            return;
        }

        strMsg += strData.substr(0, strData.length() - 2);
        std::cout << "DATA: \n" << strMsg << std::endl;
        start();
    }
}

void CSession2::parseHttpGet(const boost::system::error_code& errorCode, const char* reqType, std::string& strData)
{
    if (errorCode) {
        return;
    }
    // 获取get过来的url  暂时只能解析无参请求
    std::string httpUrl(strData.substr(strData.find("T ") + 2, strData.find(" HTTP/") - strlen(reqType) - 1));
    std::cout << httpUrl << std::endl;
    std::ifstream inf;
    std::string finalUrl("html");
    finalUrl += httpUrl;
    std::cout << finalUrl << std::endl;

    // 读取文件
    int length = 0;
    inf.open(finalUrl.c_str());
    std::string buf = "";
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n";
    if (inf.is_open()) {
        std::cout << "file is open" << std::endl;

        // 返回响应
        while (std::getline(inf, buf)) {
            response += buf;
            response += "\r\n";
            length += buf.length();
        }

    } else {
        if (httpUrl.find(".do") != std::string::npos || httpUrl.find(".jsp") != std::string::npos || httpUrl.find(".asp") != std::string::npos || httpUrl.find(".php") != std::string::npos || httpUrl.find(".html") != std::string::npos || httpUrl.find(".htm") != std::string::npos || httpUrl.find(".aspx") != std::string::npos) {
            response = "HTTP/1.1 404 OK\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n \
        <html> \
        <title>404</title> \
        <body> \
        <h1>404</h1> \
        <h2> Page Not Found<h2> \
        </body> \
        </html>";
        }
    }

    // std::cout << "response:\n" << response << std::endl;
    mSocket.async_write_some(boost::asio::buffer(response),
                             boost::bind(&CSession2::finishResponse, shared_from_this()));
}

void CSession2::parseHttpPost(const boost::system::error_code& errorCode, const char* reqType, std::string& strData)
{
    if (errorCode) {
        return;
    }
    // 获取用于处理请求的函数名
    std::string httpUrl(strData.substr(strData.find("T ") + 3, strData.find(" HTTP/") - strlen(reqType) - 1));

    if (httpUrl.find(".do") != std::string::npos || httpUrl.find(".jsp") != std::string::npos || httpUrl.find(".asp") != std::string::npos || httpUrl.find(".php") != std::string::npos || httpUrl.find(".html") != std::string::npos || httpUrl.find(".htm") != std::string::npos || httpUrl.find(".aspx") != std::string::npos || httpUrl.find(".") == std::string::npos) {

        // 解析post参数
        std::string parameters(strData.substr(strData.find_last_of("\r\n\r\n") + 1));

        // 交给dopost类解决
        CDoPost doPost(httpUrl, parameters);

        std::string response = doPost.getResponse();

        std::cout << "response:\n" << response << std::endl;
        mSocket.async_write_some(boost::asio::buffer(response),
                                 boost::bind(&CSession2::finishResponse, shared_from_this()));
    } else {
    }
}

void CSession2::finishResponse()
{
    mSocket.close();
    std::cout << "disconnect" << std::endl;
}

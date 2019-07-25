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
    std::string isOver(cData);
    if(isOver.find("{[>over<]}") != std::string::npos)
    {
    	// ½áÊø
    	strMsg += isOver.substr(0,isOver.find("{[>over<]}")-1);
    	std::cout<<"over"<<std::endl;		
    	std::cout<<"DATA: \n"<<strMsg<<std::endl;
    	mSocket.close();
    }else{
    	strMsg += cData;
    	std::cout<<"DATA: \n"<<strMsg<<std::endl;
    	start();
    }
}
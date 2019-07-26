#include "dopost.h"

CDoPost::CDoPost(std::string functionName, std::string parameters)
{
    std::cout << "CDoPost" << std::endl;
    parseParameters(parameters);
    if (functionName.find("test") != std::string::npos) {
        test();
        return;
    }

    strResponse = "HTTP/1.1 404 OK\r\nContent-Type:text/html; charset=UTF-8\r\n\r\n \
                <html> \
                    <title>404</title> \
                    <body> \
                        <h1>404</h1> \
                        <h2> Page Not Found<h2> \
                    </body> \
                </html>";
}

CDoPost::~CDoPost()
{
    // ~CDoPost
    std::cout << "~CDoPost" << std::endl;
}

void CDoPost::test()
{
    std::cout << "test success" << std::endl;
}

std::string CDoPost::getResponse()
{
    return strResponse;
}

void CDoPost::parseParameters(std::string parameters)
{
    std::istringstream parStr(parameters.c_str());
    std::string line;
    while (getline(parStr, line, '&')) {
        std::string postKey = "", postValue = "";
        postKey = line.substr(0, line.find('='));
        postValue = line.substr(line.find('=') + 1);
        parametersMap[postKey] = postValue;
    }
}

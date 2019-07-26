#ifndef DOPOST
#define DOPOST

#include <iostream>
#include <sstream>
#include <map>
class CDoPost {

public:
    CDoPost(std::string, std::string);
    ~CDoPost();

    std::string getResponse();

private:
    void test();
    void parseParameters(std::string parameters);

private:
    std::string strResponse;
    std::map<std::string, std::string> parametersMap;
};

#endif //!DOPOST

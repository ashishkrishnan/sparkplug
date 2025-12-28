#ifndef WEBSERVICE_H
#define WEBSERVICE_H
#include <WebServer.h>

class WebService {
private:
    WebServer server;

public:
    WebService();

    void setupWebAPI();

    void handleWebAPILoop();
};
#endif

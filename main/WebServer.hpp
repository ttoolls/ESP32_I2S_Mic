#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <esp_http_server.h>

class WebServer
{
public:
    WebServer();
    ~WebServer();

    void start();
    void stop();

private:
    static esp_err_t wavGetHandler(httpd_req_t *req);
    httpd_handle_t server;
};

#endif // WEBSERVER_H

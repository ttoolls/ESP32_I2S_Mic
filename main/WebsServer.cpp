#include "WebServer.hpp"
#include <esp_log.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <nvs_flash.h>
// #include <protocol_examples_common.h>

static const char *TAG = "WebServer";

// This is where you put the C array of your .wav file
const uint8_t wavFile[] = {
    // Your C array data here
};

// Length of the .wav file
const size_t wavFileLength = sizeof(wavFile) / sizeof(wavFile[0]);

WebServer::WebServer() : server(NULL) {}

WebServer::~WebServer()
{
    stop();
}

void WebServer::start()
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_uri_t wavUri = {
            .uri = "/audio",
            .method = HTTP_GET,
            .handler = wavGetHandler,
            .user_ctx = NULL};
        httpd_register_uri_handler(server, &wavUri);
    }
    else
    {
        ESP_LOGE(TAG, "Error starting server!");
    }
}

void WebServer::stop()
{
    if (server)
    {
        httpd_stop(server);
        server = NULL;
    }
}

esp_err_t WebServer::wavGetHandler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "audio/wav");
    httpd_resp_set_hdr(req, "Content-Disposition", "inline; filename=\"sample.wav\"");
    httpd_resp_send(req, (const char *)wavFile, wavFileLength);

    httpd_resp_sendstr_chunk(req, "<audio controls preload=\"none\"><source src=\"");
    httpd_resp_sendstr_chunk(req, req->uri);
    httpd_resp_sendstr_chunk(req, entry->d_name);
    if (entry->d_type == DT_DIR)
    {
        httpd_resp_sendstr_chunk(req, "/");
    }
    httpd_resp_sendstr_chunk(req, "\" type=\"audio/wav\">");
    httpd_resp_sendstr_chunk(req, "</audio>");

    return ESP_OK;
}

#pragma once

#include "../core/InvestigationEngine.hpp"
#include <string>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <map>

namespace Culprit {

struct HttpRequest {
    std::string method;
    std::string path;
    std::string rawQuery;
    std::map<std::string, std::string> queryParams;
    std::map<std::string, std::string> headers;
    std::string body;
};

struct HttpResponse {
    int statusCode = 200;
    std::string statusMessage = "OK";
    std::string contentType = "application/json";
    std::string body;
    std::map<std::string, std::string> headers;
};

class HttpServer {
private:
    int port;
    std::string staticDir;
    std::shared_ptr<InvestigationEngine> engine;
    std::atomic<bool> isRunning;
    int serverFd = -1;

    void handleClient(int clientSocket);
    HttpResponse routeRequest(const HttpRequest& req);
    std::string getMimeType(const std::string& path);
    bool serveStaticFile(const std::string& filePath, HttpResponse& resp);

public:
    HttpServer(int port, const std::string& staticDir, std::shared_ptr<InvestigationEngine> engine);
    ~HttpServer();

    void start();
    void stop();
};

} // namespace Culprit

#include "HttpServer.hpp"
#include "JsonHelper.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

namespace Culprit {

static std::string urlDecode(const std::string& in) {
    std::string out;
    out.reserve(in.size());
    for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 2 < in.size()) {
                int hexVal;
                std::istringstream hexStream(in.substr(i + 1, 2));
                if (hexStream >> std::hex >> hexVal) {
                    out += static_cast<char>(hexVal);
                    i += 2;
                } else {
                    out += '%';
                }
            } else {
                out += '%';
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }
    return out;
}

HttpServer::HttpServer(int port, const std::string& staticDir, std::shared_ptr<InvestigationEngine> engine)
    : port(port), staticDir(staticDir), engine(engine), isRunning(false) {}

HttpServer::~HttpServer() {
    stop();
}

void HttpServer::stop() {
    if (isRunning) {
        isRunning = false;
        if (serverFd >= 0) {
            close(serverFd);
            serverFd = -1;
        }
    }
}

std::string HttpServer::getMimeType(const std::string& path) {
    if (path.rfind(".html") != std::string::npos || path == "/") return "text/html; charset=utf-8";
    if (path.rfind(".css") != std::string::npos) return "text/css; charset=utf-8";
    if (path.rfind(".js") != std::string::npos) return "application/javascript; charset=utf-8";
    if (path.rfind(".json") != std::string::npos) return "application/json; charset=utf-8";
    if (path.rfind(".png") != std::string::npos) return "image/png";
    if (path.rfind(".jpg") != std::string::npos || path.rfind(".jpeg") != std::string::npos) return "image/jpeg";
    if (path.rfind(".svg") != std::string::npos) return "image/svg+xml";
    if (path.rfind(".ico") != std::string::npos) return "image/x-icon";
    return "text/plain; charset=utf-8";
}

bool HttpServer::serveStaticFile(const std::string& relativePath, HttpResponse& resp) {
    std::string safePath = relativePath;
    if (safePath == "/" || safePath.empty()) safePath = "/index.html";

    // Clean path to prevent directory traversal
    if (safePath.find("..") != std::string::npos) {
        resp.statusCode = 403;
        resp.statusMessage = "Forbidden";
        resp.body = "{\"error\":\"Access Denied\"}";
        return false;
    }

    std::string fullPath = staticDir + safePath;
    std::ifstream file(fullPath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    resp.body = ss.str();
    resp.statusCode = 200;
    resp.statusMessage = "OK";
    resp.contentType = getMimeType(safePath);
    return true;
}

HttpResponse HttpServer::routeRequest(const HttpRequest& req) {
    HttpResponse resp;
    resp.headers["Access-Control-Allow-Origin"] = "*";
    resp.headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, OPTIONS";
    resp.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization";

    if (req.method == "OPTIONS") {
        resp.statusCode = 204;
        resp.statusMessage = "No Content";
        resp.body = "";
        return resp;
    }

    // Health check
    if (req.path == "/api/health") {
        resp.body = "{\"status\":\"ONLINE\",\"system\":\"WHO IS THE CULPRIT? Forensic Engine\",\"version\":\"2.0-FORENSIC-C++17\"}";
        return resp;
    }

    // GET /api/cpp-code (Serves source snippets for teacher inspection)
    if (req.path == "/api/cpp-code") {
        std::ostringstream oss;
        oss << "{"
            << "\"merge_sort\":" << Json::quote(
                "// C++17 Divide-and-Conquer Merge Sort for Suspect Prioritization (O(N log N))\n"
                "void mergeSort(std::vector<Suspect>& arr, int left, int right) {\n"
                "    if (left >= right) return;\n"
                "    int mid = left + (right - left) / 2;\n"
                "    mergeSort(arr, left, mid);\n"
                "    mergeSort(arr, mid + 1, right);\n"
                "    merge(arr, left, mid, right);\n"
                "}\n\n"
                "void merge(std::vector<Suspect>& arr, int left, int mid, int right) {\n"
                "    std::vector<Suspect> L(arr.begin() + left, arr.begin() + mid + 1);\n"
                "    std::vector<Suspect> R(arr.begin() + mid + 1, arr.begin() + right + 1);\n"
                "    int i = 0, j = 0, k = left;\n"
                "    while (i < L.size() && j < R.size()) {\n"
                "        // Descending order of composite evidence relevance score\n"
                "        if (L[i].evidenceScore >= R[j].evidenceScore) arr[k++] = L[i++];\n"
                "        else arr[k++] = R[j++];\n"
                "    }\n"
                "    while (i < L.size()) arr[k++] = L[i++];\n"
                "    while (j < R.size()) arr[k++] = R[j++];\n"
                "}"
            ) << ","
            << "\"backtracking\":" << Json::quote(
                "// C++17 Backtracking Constraint Satisfaction Elimination Engine\n"
                "void backtrackSolve(const vector<Suspect>& suspects, size_t sIdx,\n"
                "                    const vector<Clue>& clues, size_t cIdx,\n"
                "                    vector<Suspect>& solutions) {\n"
                "    if (sIdx >= suspects.size()) return;\n"
                "    const auto& cand = suspects[sIdx];\n"
                "    // Base Case: Candidate satisfies all empirical clues\n"
                "    if (cIdx >= clues.size()) {\n"
                "        solutions.push_back(cand);\n"
                "        backtrackSolve(suspects, sIdx + 1, clues, 0, solutions);\n"
                "        return;\n"
                "    }\n"
                "    // Evaluate Constraint at cIdx\n"
                "    if (evaluateConstraint(cand, clues[cIdx])) {\n"
                "        // Recurse to next clue\n"
                "        backtrackSolve(suspects, sIdx, clues, cIdx + 1, solutions);\n"
                "    } else {\n"
                "        // PRUNE BRANCH: Constraint failed, skip all sub-trees\n"
                "        logBranchPruned(cand, clues[cIdx]);\n"
                "        backtrackSolve(suspects, sIdx + 1, clues, 0, solutions);\n"
                "    }\n"
                "}"
            ) << ","
            << "\"bfs\":" << Json::quote(
                "// C++17 Adjacency List Breadth-First Search (O(V + E))\n"
                "BFSResult runBFS(const std::string& startId) {\n"
                "    std::queue<std::string> q;\n"
                "    std::unordered_set<std::string> visited;\n"
                "    q.push(startId); visited.insert(startId);\n"
                "    distance[startId] = 0;\n"
                "    while (!q.empty()) {\n"
                "        std::string u = q.front(); q.pop();\n"
                "        for (const auto& edge : adjList[u]) {\n"
                "            std::string v = edge.targetId;\n"
                "            if (visited.find(v) == visited.end()) {\n"
                "                visited.insert(v);\n"
                "                distance[v] = distance[u] + 1;\n"
                "                parent[v] = u;\n"
                "                q.push(v);\n"
                "            }\n"
                "        }\n"
                "    }\n"
                "    return result;\n"
                "}"
            ) << ","
            << "\"kmp\":" << Json::quote(
                "// C++17 Knuth-Morris-Pratt (KMP) Linear-Time String Matching (O(N + M))\n"
                "std::vector<int> computeLps(const std::string& pat) {\n"
                "    int m = pat.length(), len = 0, i = 1;\n"
                "    std::vector<int> lps(m, 0);\n"
                "    while (i < m) {\n"
                "        if (pat[i] == pat[len]) { len++; lps[i++] = len; }\n"
                "        else if (len != 0) len = lps[len - 1];\n"
                "        else lps[i++] = 0;\n"
                "    }\n"
                "    return lps;\n"
                "}\n\n"
                "void kmpSearch(const std::string& txt, const std::string& pat) {\n"
                "    auto lps = computeLps(pat);\n"
                "    int i = 0, j = 0;\n"
                "    while (i < txt.length()) {\n"
                "        if (pat[j] == txt[i]) { i++; j++; }\n"
                "        if (j == pat.length()) {\n"
                "            recordMatch(i - j, i - 1);\n"
                "            j = lps[j - 1]; // Shift without text rollback\n"
                "        } else if (i < txt.length() && pat[j] != txt[i]) {\n"
                "            if (j != 0) j = lps[j - 1];\n"
                "            else i++;\n"
                "        }\n"
                "    }\n"
                "}"
            )
            << "}";
        resp.body = oss.str();
        return resp;
    }

    // GET /api/cases
    if (req.path == "/api/cases" && req.method == "GET") {
        const auto& all = engine->getAllCases();
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < all.size(); ++i) {
            oss << Json::serializeCaseSummary(all[i]);
            if (i + 1 < all.size()) oss << ",";
        }
        oss << "]";
        resp.body = oss.str();
        return resp;
    }

    // Case specific routes: /api/cases/<caseId>/...
    if (req.path.rfind("/api/cases/", 0) == 0) {
        std::string remainder = req.path.substr(std::string("/api/cases/").length());
        size_t slashPos = remainder.find('/');
        std::string caseId = (slashPos == std::string::npos) ? remainder : remainder.substr(0, slashPos);
        std::string subRoute = (slashPos == std::string::npos) ? "" : remainder.substr(slashPos);

        auto* caseFile = engine->getCase(caseId);
        if (!caseFile && req.method != "POST") {
            resp.statusCode = 404;
            resp.statusMessage = "Not Found";
            resp.body = "{\"error\":\"Investigation case not found in repository: " + caseId + "\"}";
            return resp;
        }

        // GET /api/cases/<id> -> Full case
        if (subRoute.empty() && req.method == "GET") {
            resp.body = Json::serializeCaseFull(*caseFile);
            return resp;
        }

        // DELETE /api/cases/<id>
        if (subRoute.empty() && req.method == "DELETE") {
            bool ok = engine->deleteCase(caseId);
            resp.body = ok ? "{\"success\":true,\"message\":\"Case archived/deleted.\"}" : "{\"error\":\"Failed to delete case.\"}";
            return resp;
        }

        // GET /api/cases/<id>/graph
        if (subRoute == "/graph" && req.method == "GET") {
            auto graph = engine->getGraph(caseId);
            if (graph) {
                resp.body = Json::serializeGraph(*graph);
            } else {
                resp.body = "{\"nodes\":[],\"links\":[]}";
            }
            return resp;
        }

        // GET /api/cases/<id>/memory (Adjacency Matrix vs List comparison)
        if (subRoute == "/memory" && req.method == "GET") {
            auto ma = engine->getMemoryAnalysis(caseId);
            std::ostringstream oss;
            oss << "{"
                << "\"vertexCount\":" << ma.vertexCount << ","
                << "\"edgeCount\":" << ma.edgeCount << ","
                << "\"graphDensity\":" << std::fixed << std::setprecision(4) << ma.graphDensity << ","
                << "\"adjListBytes\":" << ma.adjListBytes << ","
                << "\"adjMatrixBytes\":" << ma.adjMatrixBytes << ","
                << "\"recommendation\":" << Json::quote(ma.recommendation)
                << "}";
            resp.body = oss.str();
            return resp;
        }

        // POST /api/cases/<id>/algorithms/shortest-path
        if (subRoute == "/algorithms/shortest-path" && req.method == "POST") {
            std::string fromId = Json::getField(req.body, "fromId");
            std::string toId = Json::getField(req.body, "toId");
            auto path = engine->getShortestPath(caseId, fromId, toId);
            std::ostringstream oss;
            oss << "{\"fromId\":" << Json::quote(fromId) << ",\"toId\":" << Json::quote(toId)
                << ",\"pathLength\":" << (path.empty() ? 0 : path.size() - 1)
                << ",\"path\":[";
            for (size_t i = 0; i < path.size(); ++i) {
                oss << Json::quote(path[i]);
                if (i + 1 < path.size()) oss << ",";
            }
            oss << "]}";
            resp.body = oss.str();
            return resp;
        }

        // POST /api/cases/<id>/algorithms/search
        if (subRoute == "/algorithms/search" && req.method == "POST") {
            std::string target = Json::getField(req.body, "target"); // "suspects" or "evidence"
            std::string field = Json::getField(req.body, "field");   // "name", "id", "all", etc.
            std::string query = Json::getField(req.body, "query");

            if (target == "evidence") {
                auto res = engine->searchEvidence(caseId, field, query);
                resp.body = Json::serializeLinearSearchEvidence(res);
            } else {
                auto res = engine->searchSuspects(caseId, field, query);
                resp.body = Json::serializeLinearSearchSuspects(res);
            }
            return resp;
        }

        // POST /api/cases/<id>/algorithms/sort
        if (subRoute == "/algorithms/sort" && req.method == "POST") {
            auto res = engine->rankSuspects(caseId);
            resp.body = Json::serializeMergeSort(res);
            return resp;
        }

        // POST /api/cases/<id>/algorithms/bfs
        if (subRoute == "/algorithms/bfs" && req.method == "POST") {
            std::string startNode = Json::getField(req.body, "startNode");
            if (startNode.empty()) startNode = "VICTIM_" + caseId;
            auto res = engine->runBFS(caseId, startNode);
            resp.body = Json::serializeBFS(res);
            return resp;
        }

        // POST /api/cases/<id>/algorithms/dfs
        if (subRoute == "/algorithms/dfs" && req.method == "POST") {
            std::string startNode = Json::getField(req.body, "startNode");
            if (startNode.empty()) startNode = "VICTIM_" + caseId;
            auto res = engine->runDFS(caseId, startNode);
            resp.body = Json::serializeDFS(res);
            return resp;
        }

        // POST /api/cases/<id>/algorithms/backtracking
        if (subRoute == "/algorithms/backtracking" && req.method == "POST") {
            auto res = engine->solveClues(caseId);
            resp.body = Json::serializeBacktracking(res);
            return resp;
        }

        // POST /api/cases/<id>/algorithms/kmp
        if (subRoute == "/algorithms/kmp" && req.method == "POST") {
            std::string statementId = Json::getField(req.body, "statementId");
            std::string pattern = Json::getField(req.body, "pattern");
            auto res = engine->searchWitnessStatement(caseId, statementId, pattern);
            resp.body = Json::serializeKMP(res);
            return resp;
        }

        // POST /api/cases/<id>/solve (Full forensic synthesis)
        if (subRoute == "/solve" && req.method == "POST") {
            auto res = engine->runFullAnalysis(caseId);
            resp.body = Json::serializeFullAnalysis(res);
            return resp;
        }

        // POST /api/cases/<id>/suspects
        if (subRoute == "/suspects" && req.method == "POST") {
            Suspect s;
            s.id = "SUSP-" + std::to_string(caseFile->suspects.size() + 10);
            s.fullName = Json::getField(req.body, "fullName");
            std::string ageStr = Json::getField(req.body, "age");
            s.age = ageStr.empty() ? 30 : std::stoi(ageStr);
            s.occupation = Json::getField(req.body, "occupation");
            s.address = Json::getField(req.body, "address");
            s.relationshipToVictim = Json::getField(req.body, "relationshipToVictim");
            s.lastKnownLocation = Json::getField(req.body, "lastKnownLocation");
            s.alibi = Json::getField(req.body, "alibi");
            s.alibiVerified = (Json::getField(req.body, "alibiVerified") == "true");
            s.motive = Json::getField(req.body, "motive");
            s.opportunity = Json::getField(req.body, "opportunity");
            s.investigationStatus = "Person of Interest";
            s.evidenceRelevanceScore = 50.0;
            s.presenceInTimeWindow = (Json::getField(req.body, "presenceInTimeWindow") == "true");
            s.accessToMeansOrWeapon = (Json::getField(req.body, "accessToMeansOrWeapon") == "true");
            s.forensicTraceMatched = (Json::getField(req.body, "forensicTraceMatched") == "true");
            s.alibiInvalidated = !s.alibiVerified;
            s.substantiatedMotive = !s.motive.empty();
            s.directSceneTrace = false;

            engine->addSuspectToCase(caseId, s);
            resp.statusCode = 201;
            resp.body = "{\"success\":true,\"suspect\":" + Json::serializeSuspect(s) + "}";
            return resp;
        }

        // POST /api/cases/<id>/evidence
        if (subRoute == "/evidence" && req.method == "POST") {
            Evidence ev;
            ev.id = "EVD-" + std::to_string(caseFile->evidenceRecords.size() + 10);
            ev.name = Json::getField(req.body, "name");
            std::string catStr = Json::getField(req.body, "category");
            if (catStr == "Physical") ev.category = EvidenceCategory::PHYSICAL;
            else if (catStr == "Digital") ev.category = EvidenceCategory::DIGITAL;
            else if (catStr == "Witness") ev.category = EvidenceCategory::WITNESS;
            else if (catStr == "Documentary") ev.category = EvidenceCategory::DOCUMENTARY;
            else ev.category = EvidenceCategory::CIRCUMSTANTIAL;

            ev.description = Json::getField(req.body, "description");
            ev.collectedLocation = Json::getField(req.body, "collectedLocation");
            ev.collectionDateTime = "2026-11-15 12:00 IST";
            ev.linkedSuspectId = Json::getField(req.body, "linkedSuspectId");
            ev.linkedSuspectName = Json::getField(req.body, "linkedSuspectName");
            std::string relStr = Json::getField(req.body, "reliabilityScore");
            ev.reliabilityScore = relStr.empty() ? 80.0 : std::stod(relStr);
            ev.status = "In Custody";
            ev.chainOfCustody = "Investigating Team Registered";
            ev.forensicNotes = Json::getField(req.body, "forensicNotes");

            engine->addEvidenceToCase(caseId, ev);
            resp.statusCode = 201;
            resp.body = "{\"success\":true,\"evidence\":" + Json::serializeEvidence(ev) + "}";
            return resp;
        }

        // POST /api/cases/<id>/witnesses
        if (subRoute == "/witnesses" && req.method == "POST") {
            WitnessStatement w;
            w.id = "WIT-" + std::to_string(caseFile->witnessStatements.size() + 10);
            w.witnessName = Json::getField(req.body, "witnessName");
            w.occupation = Json::getField(req.body, "occupation");
            w.relationshipToCase = Json::getField(req.body, "relationshipToCase");
            w.statementDateTime = "2026-11-15 14:00 IST";
            w.statementText = Json::getField(req.body, "statementText");
            w.connectedSuspectId = Json::getField(req.body, "connectedSuspectId");
            std::string credStr = Json::getField(req.body, "credibilityRating");
            w.credibilityRating = credStr.empty() ? 85.0 : std::stod(credStr);
            w.interviewerBadge = "Badge #7412";

            engine->addWitnessStatementToCase(caseId, w);
            resp.statusCode = 201;
            resp.body = "{\"success\":true,\"witness\":" + Json::serializeWitness(w) + "}";
            return resp;
        }
    }

    // Try serving static files
    if (serveStaticFile(req.path, resp)) {
        return resp;
    }

    resp.statusCode = 404;
    resp.statusMessage = "Not Found";
    resp.body = "{\"error\":\"Endpoint not found: " + req.path + "\"}";
    return resp;
}

void HttpServer::handleClient(int clientSocket) {
    char buffer[16384];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead <= 0) {
        close(clientSocket);
        return;
    }

    std::string rawRequest(buffer, bytesRead);
    std::istringstream reqStream(rawRequest);

    HttpRequest req;
    reqStream >> req.method >> req.path;

    // Parse query string if present
    size_t qPos = req.path.find('?');
    if (qPos != std::string::npos) {
        req.rawQuery = req.path.substr(qPos + 1);
        req.path = req.path.substr(0, qPos);

        std::istringstream qStream(req.rawQuery);
        std::string pair;
        while (std::getline(qStream, pair, '&')) {
            size_t eqPos = pair.find('=');
            if (eqPos != std::string::npos) {
                std::string k = urlDecode(pair.substr(0, eqPos));
                std::string v = urlDecode(pair.substr(eqPos + 1));
                req.queryParams[k] = v;
            }
        }
    }

    // Parse headers
    std::string line;
    std::getline(reqStream, line); // finish request line
    while (std::getline(reqStream, line) && line != "\r" && !line.empty()) {
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string headerName = line.substr(0, colon);
            std::string headerVal = line.substr(colon + 1);
            while (!headerVal.empty() && (headerVal.front() == ' ' || headerVal.front() == '\t')) headerVal.erase(headerVal.begin());
            while (!headerVal.empty() && (headerVal.back() == '\r' || headerVal.back() == ' ')) headerVal.pop_back();
            req.headers[headerName] = headerVal;
        }
    }

    // Extract body
    size_t bodyPos = rawRequest.find("\r\n\r\n");
    if (bodyPos != std::string::npos) {
        req.body = rawRequest.substr(bodyPos + 4);
    }

    HttpResponse resp = routeRequest(req);

    // Build raw HTTP response
    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 " << resp.statusCode << " " << resp.statusMessage << "\r\n";
    responseStream << "Content-Type: " << resp.contentType << "\r\n";
    responseStream << "Content-Length: " << resp.body.length() << "\r\n";
    responseStream << "Connection: close\r\n";
    for (const auto& h : resp.headers) {
        responseStream << h.first << ": " << h.second << "\r\n";
    }
    responseStream << "\r\n";
    responseStream << resp.body;

    std::string rawResponse = responseStream.str();
    send(clientSocket, rawResponse.c_str(), rawResponse.length(), 0);
    close(clientSocket);
}

void HttpServer::start() {
    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd < 0) {
        std::cerr << "[ERROR] Failed to create socket." << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(serverFd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "[ERROR] Failed to bind to port " << port << std::endl;
        close(serverFd);
        serverFd = -1;
        return;
    }

    if (listen(serverFd, 30) < 0) {
        std::cerr << "[ERROR] Failed to listen on socket." << std::endl;
        close(serverFd);
        serverFd = -1;
        return;
    }

    isRunning = true;
    std::cout << "[CULPRIT-ENGINE] HTTP Server actively listening on http://127.0.0.1:" << port << std::endl;
    std::cout << "[CULPRIT-ENGINE] Forensic web console root: " << staticDir << std::endl;

    while (isRunning) {
        sockaddr_in clientAddr{};
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverFd, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket < 0) {
            if (!isRunning) break;
            continue;
        }

        // Spawn detached worker thread for concurrent request processing
        std::thread([this, clientSocket]() {
            this->handleClient(clientSocket);
        }).detach();
    }
}

} // namespace Culprit

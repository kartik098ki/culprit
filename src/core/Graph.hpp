#pragma once

#include "Models.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <sstream>
#include <chrono>
#include <algorithm>

namespace Culprit {

struct TraversalStep {
    int stepNumber;
    std::string action;       // "ENQUEUE", "DEQUEUE", "VISIT", "DISCOVER_NEIGHBOR", "ALREADY_VISITED", "PUSH_STACK", "POP_STACK", "BACKTRACK"
    std::string currentNodeId;
    std::string currentNodeLabel;
    std::string targetNodeId;
    std::string edgeRelation;
    int currentDistance; // Level in BFS, or recursion depth in DFS
    std::vector<std::string> activeQueueOrStack;
    std::vector<std::string> visitedList;
    std::string notes;
};

struct BFSResult {
    std::string algorithmName = "Breadth-First Search (BFS)";
    std::string startNodeId;
    std::string startNodeLabel;
    std::string timeComplexity = "O(V + E)";
    std::string spaceComplexity = "O(V)";
    int verticesVisited = 0;
    int edgesTraversed = 0;
    long long executionTimeMicros = 0;
    std::unordered_map<std::string, int> distanceMap;
    std::unordered_map<std::string, std::string> parentMap;
    std::vector<std::string> traversalOrder;
    std::vector<TraversalStep> traceLogs;
    std::string summary;
};

struct DFSResult {
    std::string algorithmName = "Depth-First Search (DFS)";
    std::string startNodeId;
    std::string startNodeLabel;
    std::string timeComplexity = "O(V + E)";
    std::string spaceComplexity = "O(V)";
    int verticesVisited = 0;
    int edgesTraversed = 0;
    int maxRecursionDepth = 0;
    bool hasCycles = false;
    long long executionTimeMicros = 0;
    std::vector<std::string> traversalOrder;
    std::vector<TraversalStep> traceLogs;
    std::vector<std::vector<std::string>> detectedChains;
    std::string summary;
};

class InvestigationGraph {
private:
    std::unordered_map<std::string, GraphNode> nodes;
    std::unordered_map<std::string, std::vector<GraphEdge>> adjList;

public:
    void addNode(const GraphNode& node) {
        nodes[node.id] = node;
    }

    void addEdge(const GraphEdge& edge, bool bidirectional = true) {
        adjList[edge.sourceId].push_back(edge);
        if (bidirectional) {
            GraphEdge rev = edge;
            rev.sourceId = edge.targetId;
            rev.targetId = edge.sourceId;
            adjList[edge.targetId].push_back(rev);
        }
    }

    const std::unordered_map<std::string, GraphNode>& getNodes() const {
        return nodes;
    }

    const std::unordered_map<std::string, std::vector<GraphEdge>>& getAdjList() const {
        return adjList;
    }

    bool hasNode(const std::string& id) const {
        return nodes.find(id) != nodes.end();
    }

    GraphNode getNode(const std::string& id) const {
        auto it = nodes.find(id);
        if (it != nodes.end()) return it->second;
        return GraphNode{id, id, "UNKNOWN", "", 0.0};
    }

    void clear() {
        nodes.clear();
        adjList.clear();
    }

    // Build graph from complete CaseFile
    void buildFromCase(const CaseFile& caseFile) {
        clear();

        // 1. Add Victim Node
        addNode(GraphNode{
            "VICTIM_" + caseFile.caseId,
            caseFile.victimName.empty() ? "Victim" : caseFile.victimName,
            "VICTIM",
            caseFile.victimDetails + " | Crime Scene: " + caseFile.crimeLocation,
            100.0
        });

        // 2. Add Crime Location Node
        std::string locId = "LOC_" + caseFile.caseId;
        addNode(GraphNode{
            locId,
            caseFile.crimeLocation,
            "LOCATION",
            "Primary Crime Scene (" + caseFile.dateTime + ")",
            50.0
        });

        // Connect Victim to Location
        addEdge(GraphEdge{"VICTIM_" + caseFile.caseId, locId, "CRIME_SCENE_LOCATION", 1.0, "Discovered at location"});

        // 3. Add Suspects
        for (const auto& s : caseFile.suspects) {
            addNode(GraphNode{
                s.id,
                s.fullName,
                "SUSPECT",
                s.occupation + " (" + std::to_string(s.age) + " yo) - Alibi: " + s.alibi,
                s.evidenceRelevanceScore
            });

            // Connect Suspect to Victim with relationship
            addEdge(GraphEdge{
                s.id,
                "VICTIM_" + caseFile.caseId,
                "RELATIONSHIP",
                0.8,
                s.relationshipToVictim
            });

            // Connect Suspect to Location if present
            if (s.presenceInTimeWindow) {
                addEdge(GraphEdge{
                    s.id,
                    locId,
                    "PROXIMITY_AT_TIME",
                    0.9,
                    "Geographic proximity or confirmed presence during critical interval"
                });
            }
        }

        // 4. Add Evidence Records
        for (const auto& e : caseFile.evidenceRecords) {
            addNode(GraphNode{
                e.id,
                e.name,
                "EVIDENCE",
                evidenceCategoryToString(e.category) + " - Reliability: " + std::to_string(static_cast<int>(e.reliabilityScore)) + "%",
                e.reliabilityScore
            });

            // Connect Evidence to Location
            addEdge(GraphEdge{
                e.id,
                locId,
                "RECOVERED_AT",
                0.85,
                "Forensically collected at " + e.collectedLocation
            });

            // Connect Evidence to linked Suspect
            if (!e.linkedSuspectId.empty() && hasNode(e.linkedSuspectId)) {
                addEdge(GraphEdge{
                    e.id,
                    e.linkedSuspectId,
                    "FORENSIC_TIE",
                    e.reliabilityScore / 100.0,
                    e.description
                });
            }
        }

        // 5. Add Witness Statements
        for (const auto& w : caseFile.witnessStatements) {
            addNode(GraphNode{
                w.id,
                w.witnessName + " (Witness)",
                "WITNESS",
                w.occupation + " - Credibility: " + std::to_string(static_cast<int>(w.credibilityRating)) + "%",
                w.credibilityRating
            });

            // Connect Witness to Victim / Case
            addEdge(GraphEdge{
                w.id,
                "VICTIM_" + caseFile.caseId,
                "TESTIFIED_ON_CASE",
                0.75,
                w.relationshipToCase
            });

            // Connect Witness to connected Suspect if mentioned
            if (!w.connectedSuspectId.empty() && hasNode(w.connectedSuspectId)) {
                addEdge(GraphEdge{
                    w.id,
                    w.connectedSuspectId,
                    "DEPOSITION_STATEMENT",
                    0.80,
                    "Witness testimony specifically implicates or notes activity of suspect"
                });
            }
        }

        // 6. Add Custom Relationships defined in CaseFile
        for (const auto& rel : caseFile.relationships) {
            addEdge(rel);
        }
    }

    // BREADTH-FIRST SEARCH (BFS)
    BFSResult runBFS(const std::string& startId) {
        auto startTime = std::chrono::high_resolution_clock::now();
        BFSResult result;
        result.startNodeId = startId;
        result.startNodeLabel = hasNode(startId) ? getNode(startId).label : startId;

        if (!hasNode(startId)) {
            result.summary = "Start node " + startId + " not found in investigation network.";
            return result;
        }

        std::queue<std::string> q;
        std::unordered_set<std::string> visited;
        std::vector<std::string> visitedOrder;
        int step = 0;

        q.push(startId);
        visited.insert(startId);
        result.distanceMap[startId] = 0;
        result.parentMap[startId] = "";

        step++;
        TraversalStep initStep;
        initStep.stepNumber = step;
        initStep.action = "INITIALIZE_BFS_ROOT";
        initStep.currentNodeId = startId;
        initStep.currentNodeLabel = getNode(startId).label;
        initStep.currentDistance = 0;
        initStep.activeQueueOrStack = {startId};
        initStep.visitedList = {startId};
        initStep.notes = "Initialized BFS queue with root investigation entity: " + getNode(startId).label;
        result.traceLogs.push_back(initStep);

        while (!q.empty()) {
            std::string curr = q.front();
            q.pop();
            result.verticesVisited++;
            result.traversalOrder.push_back(curr);

            std::vector<std::string> queueSnapshot;
            std::queue<std::string> tempQ = q;
            while (!tempQ.empty()) {
                queueSnapshot.push_back(tempQ.front());
                tempQ.pop();
            }

            step++;
            TraversalStep deqStep;
            deqStep.stepNumber = step;
            deqStep.action = "DEQUEUE_FOR_EXPANSION";
            deqStep.currentNodeId = curr;
            deqStep.currentNodeLabel = getNode(curr).label;
            deqStep.currentDistance = result.distanceMap[curr];
            deqStep.activeQueueOrStack = queueSnapshot;
            visitedOrder.push_back(curr);
            deqStep.visitedList = visitedOrder;
            deqStep.notes = "Dequeued node '" + getNode(curr).label + "' at distance level " + 
                            std::to_string(result.distanceMap[curr]) + ". Scanning adjacent edges.";
            result.traceLogs.push_back(deqStep);

            if (adjList.find(curr) != adjList.end()) {
                for (const auto& edge : adjList[curr]) {
                    result.edgesTraversed++;
                    std::string neighbor = edge.targetId;

                    if (visited.find(neighbor) == visited.end()) {
                        visited.insert(neighbor);
                        result.distanceMap[neighbor] = result.distanceMap[curr] + 1;
                        result.parentMap[neighbor] = curr;
                        q.push(neighbor);

                        std::vector<std::string> curQ;
                        std::queue<std::string> tQ = q;
                        while (!tQ.empty()) {
                            curQ.push_back(tQ.front());
                            tQ.pop();
                        }

                        step++;
                        TraversalStep discStep;
                        discStep.stepNumber = step;
                        discStep.action = "DISCOVER_NEIGHBOR";
                        discStep.currentNodeId = curr;
                        discStep.currentNodeLabel = getNode(curr).label;
                        discStep.targetNodeId = neighbor;
                        discStep.edgeRelation = edge.relationType + " (" + edge.description + ")";
                        discStep.currentDistance = result.distanceMap[neighbor];
                        discStep.activeQueueOrStack = curQ;
                        discStep.visitedList = visitedOrder;
                        discStep.notes = "Found unvisited relation '" + edge.relationType + "' -> '" + 
                                         getNode(neighbor).label + "'. Enqueued at distance " + 
                                         std::to_string(result.distanceMap[neighbor]) + ".";
                        result.traceLogs.push_back(discStep);
                    }
                }
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        result.executionTimeMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::ostringstream oss;
        oss << "BFS traversal complete from root '" << result.startNodeLabel << "'. "
            << "Explored " << result.verticesVisited << " entities and " 
            << result.edgesTraversed << " relational connections. "
            << "Maximum degrees of separation observed: " 
            << (result.distanceMap.empty() ? 0 : std::max_element(result.distanceMap.begin(), result.distanceMap.end(),
                   [](const auto& a, const auto& b) { return a.second < b.second; })->second)
            << " hops. Algorithmic complexity: O(V + E).";
        result.summary = oss.str();

        return result;
    }

    // DEPTH-FIRST SEARCH (DFS)
    DFSResult runDFS(const std::string& startId) {
        auto startTime = std::chrono::high_resolution_clock::now();
        DFSResult result;
        result.startNodeId = startId;
        result.startNodeLabel = hasNode(startId) ? getNode(startId).label : startId;

        if (!hasNode(startId)) {
            result.summary = "Start node " + startId + " not found.";
            return result;
        }

        std::unordered_set<std::string> visited;
        std::vector<std::string> currentPath;
        std::vector<std::string> visitedOrder;
        int step = 0;

        std::function<void(const std::string&, int)> dfsVisit = [&](const std::string& u, int depth) {
            visited.insert(u);
            visitedOrder.push_back(u);
            currentPath.push_back(u);
            result.verticesVisited++;
            result.traversalOrder.push_back(u);
            if (depth > result.maxRecursionDepth) {
                result.maxRecursionDepth = depth;
            }

            step++;
            TraversalStep pushStep;
            pushStep.stepNumber = step;
            pushStep.action = "DFS_PUSH_CALL_STACK";
            pushStep.currentNodeId = u;
            pushStep.currentNodeLabel = getNode(u).label;
            pushStep.currentDistance = depth;
            pushStep.activeQueueOrStack = currentPath;
            pushStep.visitedList = visitedOrder;
            pushStep.notes = "Traversing into node '" + getNode(u).label + "' at call depth " + 
                             std::to_string(depth) + ". Probing deeper relation chains.";
            result.traceLogs.push_back(pushStep);

            if (adjList.find(u) != adjList.end()) {
                for (const auto& edge : adjList[u]) {
                    result.edgesTraversed++;
                    std::string v = edge.targetId;

                    if (visited.find(v) == visited.end()) {
                        step++;
                        TraversalStep edgeStep;
                        edgeStep.stepNumber = step;
                        edgeStep.action = "FOLLOW_RELATIONSHIP_CHAIN";
                        edgeStep.currentNodeId = u;
                        edgeStep.currentNodeLabel = getNode(u).label;
                        edgeStep.targetNodeId = v;
                        edgeStep.edgeRelation = edge.relationType;
                        edgeStep.currentDistance = depth + 1;
                        edgeStep.activeQueueOrStack = currentPath;
                        edgeStep.visitedList = visitedOrder;
                        edgeStep.notes = "Branching down edge [" + edge.relationType + "] to '" + getNode(v).label + "'.";
                        result.traceLogs.push_back(edgeStep);

                        dfsVisit(v, depth + 1);
                    }
                }
            }

            // Save completed investigative chain if significant length
            if (currentPath.size() >= 3) {
                result.detectedChains.push_back(currentPath);
            }

            // Backtrack
            step++;
            TraversalStep popStep;
            popStep.stepNumber = step;
            popStep.action = "DFS_BACKTRACK_UNWIND";
            popStep.currentNodeId = u;
            popStep.currentNodeLabel = getNode(u).label;
            popStep.currentDistance = depth;
            currentPath.pop_back();
            popStep.activeQueueOrStack = currentPath;
            popStep.visitedList = visitedOrder;
            popStep.notes = "Completed exhaustive branch probe for '" + getNode(u).label + 
                             "'. Unwinding stack to explore alternate connections.";
            result.traceLogs.push_back(popStep);
        };

        dfsVisit(startId, 1);

        auto endTime = std::chrono::high_resolution_clock::now();
        result.executionTimeMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::ostringstream oss;
        oss << "DFS deep relationship exploration completed. Visited " 
            << result.verticesVisited << " entities with max recursion depth " 
            << result.maxRecursionDepth << ". Discovered " << result.detectedChains.size() 
            << " multi-hop evidentiary paths. Complexity: O(V + E).";
        result.summary = oss.str();

        return result;
    }

    // Shortest path between two nodes using BFS parent reconstruct
    std::vector<std::string> getShortestPath(const std::string& fromId, const std::string& toId) {
        BFSResult bfs = runBFS(fromId);
        std::vector<std::string> path;

        if (bfs.distanceMap.find(toId) == bfs.distanceMap.end()) {
            return path; // No path
        }

        std::string curr = toId;
        while (!curr.empty()) {
            path.push_back(curr);
            curr = bfs.parentMap[curr];
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    // Memory footprint analysis comparing Adjacency List vs Adjacency Matrix
    struct MemoryAnalysis {
        size_t vertexCount = 0;
        size_t edgeCount = 0;
        double graphDensity = 0.0;
        size_t adjListBytes = 0;
        size_t adjMatrixBytes = 0;
        std::string recommendation;
    };

    MemoryAnalysis getMemoryAnalysis() const {
        MemoryAnalysis ma;
        ma.vertexCount = nodes.size();
        size_t totalEdges = 0;
        for (const auto& p : adjList) {
            totalEdges += p.second.size();
        }
        ma.edgeCount = totalEdges / 2; // Undirected pair count

        if (ma.vertexCount > 1) {
            double maxEdges = static_cast<double>(ma.vertexCount * (ma.vertexCount - 1)) / 2.0;
            ma.graphDensity = (maxEdges > 0) ? (static_cast<double>(ma.edgeCount) / maxEdges) : 0.0;
        }

        // Adjacency List memory: unordered_map overhead + node entries + edge structs
        ma.adjListBytes = (nodes.size() * (sizeof(GraphNode) + 32)) + (totalEdges * sizeof(GraphEdge));

        // Adjacency Matrix memory: V * V * sizeof(float/double) + vector overhead
        ma.adjMatrixBytes = (ma.vertexCount * ma.vertexCount * sizeof(double)) + (ma.vertexCount * sizeof(void*));

        std::ostringstream rec;
        rec << "Graph density is " << std::fixed << std::setprecision(2) << (ma.graphDensity * 100.0) 
            << "% (Sparse Network). Adjacency List is optimal: saves " 
            << (ma.adjMatrixBytes > ma.adjListBytes ? (ma.adjMatrixBytes - ma.adjListBytes) : 0) 
            << " bytes over dense matrix representation.";
        ma.recommendation = rec.str();

        return ma;
    }
};

} // namespace Culprit

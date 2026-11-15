#include "core/InvestigationEngine.hpp"
#include "server/HttpServer.hpp"
#include "tests/TestRunner.cpp"
#include <iostream>
#include <memory>
#include <string>
#include <csignal>

static std::shared_ptr<Culprit::HttpServer> g_server;

void signalHandler(int signum) {
    std::cout << "\n[SIGNAL] Received signal " << signum << ". Shutting down server gracefully..." << std::endl;
    if (g_server) {
        g_server->stop();
    }
    exit(0);
}

void printBanner() {
    std::cout << "\033[1;36m";
    std::cout << R"(
  ██╗    ██╗██╗  ██╗ ██████╗     ██╗███████╗    ████████╗██╗  ██╗███████╗
  ██║    ██║██║  ██║██╔═══██╗    ██║██╔════╝    ╚══██╔══╝██║  ██║██╔════╝
  ██║ █╗ ██║███████║██║   ██║    ██║███████╗       ██║   ███████║█████╗  
  ██║███╗██║██╔══██║██║   ██║    ██║╚════██║       ██║   ██╔══██║██╔══╝  
  ╚███╔███╔╝██║  ██║╚██████╔╝    ██║███████║       ██║   ██║  ██║███████╗
   ╚══╝╚══╝ ╚═╝  ╚═╝ ╚═════╝     ╚═╝╚══════╝       ╚═╝   ╚═╝  ╚═╝╚══════╝
                                                                          
   ██████╗██╗   ██╗██╗     ██████╗ ██████╗ ██╗████████╗                 
  ██╔════╝██║   ██║██║     ██╔══██╗██╔══██╗██║╚══██╔══╝                 
  ██║     ██║   ██║██║     ██████╔╝██████╔╝██║   ██║                    
  ██║     ██║   ██║██║     ██╔═══╝ ██╔══██╗██║   ██║                    
  ╚██████╗╚██████╔╝███████╗██║     ██║  ██║██║   ██║                    
   ╚═════╝ ╚═════╝ ╚══════╝╚═╝     ╚═╝  ╚═╝╚═╝   ╚═╝                    
    )" << "\033[0m" << std::endl;
    std::cout << "\033[1;37m  [CRIME INVESTIGATION & FORENSIC EVIDENCE ANALYSIS PLATFORM]\033[0m" << std::endl;
    std::cout << "\033[1;30m  C++17 High Performance Engine | Graph Network | KMP | CSP Backtracking\033[0m\n" << std::endl;
}

void printCliCaseReport(Culprit::InvestigationEngine& engine, const std::string& caseId) {
    auto* cf = engine.getCase(caseId);
    if (!cf) {
        std::cerr << "Case ID not found: " << caseId << std::endl;
        return;
    }

    auto analysis = engine.runFullAnalysis(caseId);

    std::cout << "\n========================================================================" << std::endl;
    std::cout << "           POLICE FORENSIC DOSSIER & ALGORITHMIC REPORT                " << std::endl;
    std::cout << "========================================================================" << std::endl;
    std::cout << "CASE FILE:      " << cf->caseTitle << " (" << cf->caseId << ")" << std::endl;
    std::cout << "CRIME TYPE:     " << Culprit::crimeTypeToString(cf->crimeType) << std::endl;
    std::cout << "DATE & TIME:    " << cf->dateTime << std::endl;
    std::cout << "LOCATION:       " << cf->crimeLocation << std::endl;
    std::cout << "INVESTIGATOR:   " << cf->assignedInvestigator << std::endl;
    std::cout << "VICTIM:         " << cf->victimName << " (" << cf->victimDetails << ")" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    std::cout << "CASE SUMMARY:" << std::endl;
    std::cout << cf->caseDescription << std::endl;

    std::cout << "\n------------------------------------------------------------------------" << std::endl;
    std::cout << "ALGORITHM 1: MERGE SORT - SUSPECT EVIDENCE RANKING (O(N log N))" << std::endl;
    std::cout << "Total Comparisons: " << analysis.rankingResult.totalComparisons << std::endl;
    std::cout << "Total Recursive Splits: " << analysis.rankingResult.totalRecursiveCalls << std::endl;
    for (size_t i = 0; i < analysis.rankingResult.sortedSuspects.size(); ++i) {
        const auto& s = analysis.rankingResult.sortedSuspects[i];
        std::cout << "  #" << (i + 1) << " " << s.fullName << " | Score: " 
                  << s.evidenceRelevanceScore << "% | Status: " << s.investigationStatus 
                  << " | Alibi: " << (s.alibiVerified ? "VERIFIED" : "BROKEN/UNVERIFIED") << std::endl;
    }

    std::cout << "\n------------------------------------------------------------------------" << std::endl;
    std::cout << "ALGORITHM 2: BACKTRACKING / CSP CLUE ELIMINATION ENGINE" << std::endl;
    std::cout << "Candidates Tested: " << analysis.clueMatchingResult.totalCandidatesTested << std::endl;
    std::cout << "Constraints Evaluated: " << analysis.clueMatchingResult.totalConstraintsEvaluated << std::endl;
    std::cout << "Branches Pruned: " << analysis.clueMatchingResult.totalBranchesPruned << std::endl;
    std::cout << "Backtrack Events: " << analysis.clueMatchingResult.totalBacktrackEvents << std::endl;
    std::cout << "\nCANDIDATES SATISFYING ALL " << cf->clues.size() << " INVESTIGATION CLUES:" << std::endl;
    if (analysis.clueMatchingResult.matchingSuspects.empty()) {
        std::cout << "  [None found matching 100% of clues]" << std::endl;
    } else {
        for (const auto& s : analysis.clueMatchingResult.matchingSuspects) {
            std::cout << "  >> [PRIMARY PERSON OF INTEREST] " << s.fullName 
                      << " (Age: " << s.age << ", Occupation: " << s.occupation << ")" << std::endl;
            std::cout << "     Motive: " << s.motive << std::endl;
            std::cout << "     Opportunity: " << s.opportunity << std::endl;
        }
    }

    std::cout << "\n------------------------------------------------------------------------" << std::endl;
    std::cout << "ALGORITHM 3: GRAPH ADJACENCY LIST & BREADTH-FIRST SEARCH (BFS)" << std::endl;
    std::cout << "Root Entity: " << analysis.primaryNetworkBfs.startNodeLabel << std::endl;
    std::cout << "Vertices Explored: " << analysis.primaryNetworkBfs.verticesVisited << std::endl;
    std::cout << "Edges Traversed: " << analysis.primaryNetworkBfs.edgesTraversed << std::endl;

    std::cout << "\n------------------------------------------------------------------------" << std::endl;
    std::cout << "EXECUTIVE FORENSIC SYNTHESIS:" << std::endl;
    std::cout << analysis.executiveForensicSummary << std::endl;
    std::cout << "\nLEGAL NOTICE:" << std::endl;
    std::cout << analysis.legalDisclaimer << std::endl;
    std::cout << "========================================================================\n" << std::endl;
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    printBanner();

    auto engine = std::make_shared<Culprit::InvestigationEngine>();

    int port = 8080;
    std::string webDir = "web";

    // Parse CLI arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--test" || arg == "-t") {
            bool success = Culprit::TestRunner::runAllTests();
            return success ? 0 : 1;
        } else if (arg == "--case" || arg == "-c") {
            std::string caseId = (i + 1 < argc) ? argv[++i] : "CASE-DEL-2026-089";
            printCliCaseReport(*engine, caseId);
            return 0;
        } else if (arg == "--port" || arg == "-p") {
            if (i + 1 < argc) port = std::stoi(argv[++i]);
        } else if (arg == "--web" || arg == "-w") {
            if (i + 1 < argc) webDir = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --serve [port]       Run HTTP API server and UI (default: 8080)" << std::endl;
            std::cout << "  --port <port>        Specify HTTP port" << std::endl;
            std::cout << "  --web <dir>          Specify web frontend directory (default: web)" << std::endl;
            std::cout << "  --test               Execute complete C++ DSA automated test suite" << std::endl;
            std::cout << "  --case <caseId>      Execute forensic analysis on case and print CLI dossier" << std::endl;
            std::cout << "  --help               Show this help message" << std::endl;
            return 0;
        }
    }

    std::cout << "[SYSTEM] Initialized " << engine->getAllCases().size() << " preloaded investigation cases." << std::endl;
    std::cout << "[SYSTEM] Primary Case: 'THE MIDNIGHT MURDER' (Bungalow 14-B, Amrita Shergill Marg, New Delhi)" << std::endl;
    std::cout << "[SYSTEM] Starting forensic HTTP server on port " << port << "..." << std::endl;

    g_server = std::make_shared<Culprit::HttpServer>(port, webDir, engine);
    g_server->start();

    return 0;
}

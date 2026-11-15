#include "../core/InvestigationEngine.hpp"
#include <iostream>
#include <cassert>

namespace Culprit {

class TestRunner {
public:
    static bool runAllTests() {
        std::cout << "\n========================================================" << std::endl;
        std::cout << "  RUNNING AUTOMATED DSA ENGINE TEST SUITE FOR CULPRIT   " << std::endl;
        std::cout << "========================================================\n" << std::endl;

        int passed = 0;
        int failed = 0;

        auto runTest = [&](const std::string& name, auto func) {
            std::cout << "[TEST] " << name << " ... ";
            try {
                func();
                std::cout << "\033[32mPASSED\033[0m" << std::endl;
                passed++;
            } catch (const std::exception& e) {
                std::cout << "\033[31mFAILED\033[0m: " << e.what() << std::endl;
                failed++;
            }
        };

        // 1. Test Linear Search
        runTest("Linear Search: Suspect ID & Name Query", []() {
            auto cases = CaseRepository::getPreloadedCases();
            const auto& suspects = cases[0].suspects;

            auto res = LinearSearchEngine::searchSuspects(suspects, "name", "Dev");
            if (res.matches.empty()) throw std::runtime_error("Failed to find Dev Singhania");
            if (res.matches[0].fullName != "Dev Singhania") throw std::runtime_error("Name mismatch in match 0");
            if (res.totalComparisons != static_cast<int>(suspects.size())) throw std::runtime_error("Total comparisons must equal N for full scan");
            if (res.traceLogs.empty()) throw std::runtime_error("Trace logs must not be empty");
        });

        // 2. Test Merge Sort
        runTest("Merge Sort: Suspect Evidence Score Ranking", []() {
            auto cases = CaseRepository::getPreloadedCases();
            auto suspects = cases[0].suspects;

            auto sortRes = MergeSortEngine::sortSuspects(suspects);
            if (sortRes.sortedSuspects.size() != suspects.size()) throw std::runtime_error("Size mismatch after sort");

            for (size_t i = 1; i < sortRes.sortedSuspects.size(); ++i) {
                if (sortRes.sortedSuspects[i-1].evidenceRelevanceScore < sortRes.sortedSuspects[i].evidenceRelevanceScore) {
                    throw std::runtime_error("Array not in descending order of score!");
                }
            }
            if (sortRes.totalComparisons <= 0) throw std::runtime_error("Zero comparisons recorded in Merge Sort");
        });

        // 3. Test Graph Adjacency List & BFS
        runTest("Graph & BFS: Level-by-level Network Discovery", []() {
            auto cases = CaseRepository::getPreloadedCases();
            InvestigationGraph g;
            g.buildFromCase(cases[0]);

            std::string root = "VICTIM_" + cases[0].caseId;
            auto bfs = g.runBFS(root);

            if (bfs.verticesVisited < 5) throw std::runtime_error("BFS visited too few vertices");
            if (bfs.distanceMap[root] != 0) throw std::runtime_error("Root distance must be 0");
            if (bfs.traceLogs.empty()) throw std::runtime_error("BFS trace logs must be populated");
        });

        // 4. Test DFS: Deep Relationship Chain Exploration
        runTest("Graph & DFS: Deep Path Exploration & Unwind", []() {
            auto cases = CaseRepository::getPreloadedCases();
            InvestigationGraph g;
            g.buildFromCase(cases[0]);

            std::string root = "VICTIM_" + cases[0].caseId;
            auto dfs = g.runDFS(root);

            if (dfs.verticesVisited < 5) throw std::runtime_error("DFS visited too few vertices");
            if (dfs.maxRecursionDepth < 2) throw std::runtime_error("DFS max depth must be >= 2");
        });

        // 5. Test Backtracking Clue Constraint Engine
        runTest("Backtracking: Clue Elimination & Pruning Tree", []() {
            auto cases = CaseRepository::getPreloadedCases();
            const auto& suspects = cases[0].suspects;
            const auto& clues = cases[0].clues;

            auto bt = BacktrackingClueEngine::runClueMatching(suspects, clues);

            if (bt.totalCandidatesTested != static_cast<int>(suspects.size())) {
                throw std::runtime_error("Backtracking must test all candidates");
            }
            if (bt.totalBranchesPruned <= 0) {
                throw std::runtime_error("Branch pruning counter must be > 0");
            }
            if (bt.matchingSuspects.empty()) {
                throw std::runtime_error("At least one suspect must match all clues in Case 1");
            }
            if (bt.matchingSuspects[0].fullName != "Dev Singhania") {
                throw std::runtime_error("Expected Dev Singhania to match all empirical clues in primary case");
            }
        });

        // 6. Test KMP String Matching
        runTest("KMP: Pi Table Computation & Pattern Search", []() {
            auto cases = CaseRepository::getPreloadedCases();
            const auto& stmt = cases[0].witnessStatements[0]; // Harish Chandra

            auto kmp = KmpSearchEngine::searchInStatement(stmt, "dark grey sedan");

            if (kmp.occurrences.empty()) throw std::runtime_error("KMP failed to locate 'dark grey sedan'");
            if (kmp.lpsTable.empty()) throw std::runtime_error("LPS table cannot be empty");
            if (kmp.totalCharacterComparisons <= 0) throw std::runtime_error("Character comparisons must be logged");
            if (kmp.occurrences[0].matchedSnippet != "dark grey sedan") {
                throw std::runtime_error("Matched snippet mismatch");
            }
        });

        // 7. Full Investigation Engine Synthesis
        runTest("Investigation Engine: Full Multi-Case Initialization", []() {
            InvestigationEngine engine;
            if (engine.getAllCases().size() < 3) throw std::runtime_error("Must have at least 3 preloaded cases");

            auto analysis = engine.runFullAnalysis("CASE-DEL-2026-089");
            if (analysis.primaryPersonsOfInterest.empty()) throw std::runtime_error("Primary persons of interest missing");
            if (analysis.executiveForensicSummary.empty()) throw std::runtime_error("Executive summary missing");
        });

        std::cout << "\n--------------------------------------------------------" << std::endl;
        std::cout << "TEST SUMMARY: " << passed << " PASSED, " << failed << " FAILED." << std::endl;
        std::cout << "--------------------------------------------------------\n" << std::endl;

        return (failed == 0);
    }
};

} // namespace Culprit

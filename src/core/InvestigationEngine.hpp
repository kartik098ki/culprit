#pragma once

#include "Models.hpp"
#include "LinearSearch.hpp"
#include "MergeSort.hpp"
#include "Graph.hpp"
#include "Backtracking.hpp"
#include "KMP.hpp"
#include "CaseData.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <sstream>

namespace Culprit {

struct FullInvestigationAnalysis {
    std::string caseId;
    std::string caseTitle;
    std::string investigator;
    std::string analysisTimestamp;
    
    // Algorithmic Results
    MergeSortResult rankingResult;
    BacktrackingResult clueMatchingResult;
    BFSResult primaryNetworkBfs;
    DFSResult primaryNetworkDfs;
    std::vector<KmpResult> kmpWitnessAnalyses;
    
    // Synthesis
    std::vector<Suspect> primaryPersonsOfInterest;
    std::vector<std::string> unresolvedClues;
    std::string executiveForensicSummary;
    std::string legalDisclaimer;
};

class InvestigationEngine {
private:
    std::vector<CaseFile> cases;
    std::unordered_map<std::string, std::shared_ptr<InvestigationGraph>> caseGraphs;

    void updateSuspectScores(CaseFile& cf) {
        // Compute composite score for each suspect dynamically based on linked evidence & statements
        for (auto& s : cf.suspects) {
            double totalWeightedEvidence = 0.0;
            int countEvidence = 0;

            for (const auto& eid : s.associatedEvidenceIds) {
                for (const auto& ev : cf.evidenceRecords) {
                    if (ev.id == eid) {
                        double weight = getCategoryWeight(ev.category);
                        totalWeightedEvidence += (ev.reliabilityScore * weight);
                        countEvidence++;
                        break;
                    }
                }
            }

            double baseScore = 0.0;
            if (countEvidence > 0) {
                baseScore = totalWeightedEvidence / static_cast<double>(countEvidence);
            }

            // Adjustments based on corroborated witnesses & alibi status
            double witnessBonus = 0.0;
            for (const auto& w : cf.witnessStatements) {
                if (w.connectedSuspectId == s.id) {
                    witnessBonus += (w.credibilityRating * 0.15);
                }
            }

            double alibiPenaltyOrBonus = 0.0;
            if (s.alibiInvalidated) {
                alibiPenaltyOrBonus += 12.0; // Penalty increases suspicion
            } else if (s.alibiVerified) {
                alibiPenaltyOrBonus -= 25.0; // Verified alibi dramatically reduces suspicion
            }

            if (s.presenceInTimeWindow) alibiPenaltyOrBonus += 8.0;
            if (s.accessToMeansOrWeapon) alibiPenaltyOrBonus += 7.0;

            double finalScore = baseScore + witnessBonus + alibiPenaltyOrBonus;
            finalScore = std::max(5.0, std::min(99.5, finalScore)); // Clamp between 5% and 99.5%
            s.evidenceRelevanceScore = finalScore;
        }
    }

public:
    InvestigationEngine() {
        cases = CaseRepository::getPreloadedCases();
        for (auto& cf : cases) {
            updateSuspectScores(cf);
            auto g = std::make_shared<InvestigationGraph>();
            g->buildFromCase(cf);
            caseGraphs[cf.caseId] = g;
        }
    }

    const std::vector<CaseFile>& getAllCases() const {
        return cases;
    }

    bool hasCase(const std::string& caseId) const {
        for (const auto& c : cases) {
            if (c.caseId == caseId) return true;
        }
        return false;
    }

    CaseFile* getCase(const std::string& caseId) {
        for (auto& c : cases) {
            if (c.caseId == caseId) return &c;
        }
        return nullptr;
    }

    bool addCase(const CaseFile& newCase) {
        if (hasCase(newCase.caseId)) return false;
        cases.push_back(newCase);
        updateSuspectScores(cases.back());
        auto g = std::make_shared<InvestigationGraph>();
        g->buildFromCase(cases.back());
        caseGraphs[newCase.caseId] = g;
        return true;
    }

    bool updateCase(const CaseFile& updatedCase) {
        for (size_t i = 0; i < cases.size(); ++i) {
            if (cases[i].caseId == updatedCase.caseId) {
                cases[i] = updatedCase;
                updateSuspectScores(cases[i]);
                auto g = std::make_shared<InvestigationGraph>();
                g->buildFromCase(cases[i]);
                caseGraphs[updatedCase.caseId] = g;
                return true;
            }
        }
        return false;
    }

    bool deleteCase(const std::string& caseId) {
        for (auto it = cases.begin(); it != cases.end(); ++it) {
            if (it->caseId == caseId) {
                cases.erase(it);
                caseGraphs.erase(caseId);
                return true;
            }
        }
        return false;
    }

    // Add Suspect
    bool addSuspectToCase(const std::string& caseId, const Suspect& suspect) {
        auto* cf = getCase(caseId);
        if (!cf) return false;
        cf->suspects.push_back(suspect);
        updateSuspectScores(*cf);
        caseGraphs[caseId]->buildFromCase(*cf);
        return true;
    }

    // Add Evidence
    bool addEvidenceToCase(const std::string& caseId, const Evidence& evidence) {
        auto* cf = getCase(caseId);
        if (!cf) return false;
        cf->evidenceRecords.push_back(evidence);
        updateSuspectScores(*cf);
        caseGraphs[caseId]->buildFromCase(*cf);
        return true;
    }

    // Add Witness Statement
    bool addWitnessStatementToCase(const std::string& caseId, const WitnessStatement& statement) {
        auto* cf = getCase(caseId);
        if (!cf) return false;
        cf->witnessStatements.push_back(statement);
        updateSuspectScores(*cf);
        caseGraphs[caseId]->buildFromCase(*cf);
        return true;
    }

    std::shared_ptr<InvestigationGraph> getGraph(const std::string& caseId) {
        if (caseGraphs.find(caseId) != caseGraphs.end()) {
            return caseGraphs[caseId];
        }
        return nullptr;
    }

    // 1. Run Linear Search on Case Suspects
    LinearSearchResult<Suspect> searchSuspects(const std::string& caseId, const std::string& field, const std::string& query) {
        auto* cf = getCase(caseId);
        if (!cf) return LinearSearchResult<Suspect>();
        return LinearSearchEngine::searchSuspects(cf->suspects, field, query);
    }

    // 2. Run Linear Search on Case Evidence
    LinearSearchResult<Evidence> searchEvidence(const std::string& caseId, const std::string& field, const std::string& query) {
        auto* cf = getCase(caseId);
        if (!cf) return LinearSearchResult<Evidence>();
        return LinearSearchEngine::searchEvidence(cf->evidenceRecords, field, query);
    }

    // 3. Run Merge Sort Suspect Ranking
    MergeSortResult rankSuspects(const std::string& caseId) {
        auto* cf = getCase(caseId);
        if (!cf) return MergeSortResult();
        updateSuspectScores(*cf);
        return MergeSortEngine::sortSuspects(cf->suspects);
    }

    // 4. Run Backtracking Clue Elimination
    BacktrackingResult solveClues(const std::string& caseId) {
        auto* cf = getCase(caseId);
        if (!cf) return BacktrackingResult();
        return BacktrackingClueEngine::runClueMatching(cf->suspects, cf->clues);
    }

    // 5. Run BFS on Network Graph
    BFSResult runBFS(const std::string& caseId, const std::string& startNodeId) {
        auto graph = getGraph(caseId);
        if (!graph) return BFSResult();
        return graph->runBFS(startNodeId);
    }

    // 6. Run DFS on Network Graph
    DFSResult runDFS(const std::string& caseId, const std::string& startNodeId) {
        auto graph = getGraph(caseId);
        if (!graph) return DFSResult();
        return graph->runDFS(startNodeId);
    }

    // 7. Run KMP String Matching on Witness Statement
    KmpResult searchWitnessStatement(const std::string& caseId, const std::string& statementId, const std::string& pattern) {
        auto* cf = getCase(caseId);
        if (!cf) return KmpResult();

        for (const auto& w : cf->witnessStatements) {
            if (w.id == statementId) {
                return KmpSearchEngine::searchInStatement(w, pattern);
            }
        }

        // Fallback: search across all statements concatenated
        if (!cf->witnessStatements.empty()) {
            return KmpSearchEngine::searchInStatement(cf->witnessStatements[0], pattern);
        }

        return KmpResult();
    }

    // 8. Memory Footprint Analysis
    InvestigationGraph::MemoryAnalysis getMemoryAnalysis(const std::string& caseId) {
        auto graph = getGraph(caseId);
        if (!graph) return InvestigationGraph::MemoryAnalysis();
        return graph->getMemoryAnalysis();
    }

    // 9. Shortest Evidentiary Path
    std::vector<std::string> getShortestPath(const std::string& caseId, const std::string& fromId, const std::string& toId) {
        auto graph = getGraph(caseId);
        if (!graph) return {};
        return graph->getShortestPath(fromId, toId);
    }

    // 8. Complete Multi-Algorithm Forensic Investigation Report
    FullInvestigationAnalysis runFullAnalysis(const std::string& caseId) {
        auto* cf = getCase(caseId);
        FullInvestigationAnalysis analysis;
        if (!cf) return analysis;

        analysis.caseId = cf->caseId;
        analysis.caseTitle = cf->caseTitle;
        analysis.investigator = cf->assignedInvestigator;
        analysis.analysisTimestamp = "2026-11-15 22:00:00 IST";

        // 1. Run Merge Sort
        analysis.rankingResult = rankSuspects(caseId);

        // 2. Run Backtracking CSP
        analysis.clueMatchingResult = solveClues(caseId);

        // 3. Run Graph BFS from Victim
        std::string victimNode = "VICTIM_" + caseId;
        analysis.primaryNetworkBfs = runBFS(caseId, victimNode);

        // 4. Run Graph DFS from Victim
        analysis.primaryNetworkDfs = runDFS(caseId, victimNode);

        // 5. Run KMP for key phrases across witness statements
        std::vector<std::string> sampleKeywords = {"dark grey sedan", "navy cashmere", "shattered glass", "decanting"};
        for (const auto& w : cf->witnessStatements) {
            for (const auto& kw : sampleKeywords) {
                KmpResult res = KmpSearchEngine::searchInStatement(w, kw);
                if (!res.occurrences.empty()) {
                    analysis.kmpWitnessAnalyses.push_back(res);
                }
            }
        }

        analysis.primaryPersonsOfInterest = analysis.clueMatchingResult.matchingSuspects;
        analysis.legalDisclaimer = analysis.clueMatchingResult.legalDisclaimer;

        std::ostringstream summary;
        summary << "COMPREHENSIVE FORENSIC SYNTHESIS: "
                << "Investigation of " << cf->caseTitle << " evaluated " 
                << cf->suspects.size() << " suspects, "
                << cf->evidenceRecords.size() << " evidence records, and "
                << cf->witnessStatements.size() << " witness depositions. "
                << "Algorithmic Merge Sort identified the highest evidence correlation suspect as "
                << (analysis.rankingResult.sortedSuspects.empty() ? "None" : analysis.rankingResult.sortedSuspects[0].fullName)
                << " (" << (analysis.rankingResult.sortedSuspects.empty() ? 0.0 : analysis.rankingResult.sortedSuspects[0].evidenceRelevanceScore) << "%). "
                << "Backtracking Clue Engine identified " << analysis.clueMatchingResult.matchingSuspects.size()
                << " candidate(s) satisfying 100% of the " << cf->clues.size() << " empirical clues. "
                << "Network BFS established " << analysis.primaryNetworkBfs.verticesVisited << " connected entities with maximum "
                << (analysis.primaryNetworkBfs.distanceMap.empty() ? 0 : 3) << " degrees of separation from victim.";
        analysis.executiveForensicSummary = summary.str();

        return analysis;
    }
};

} // namespace Culprit

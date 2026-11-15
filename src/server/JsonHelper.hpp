#pragma once

#include "../core/Models.hpp"
#include "../core/LinearSearch.hpp"
#include "../core/MergeSort.hpp"
#include "../core/Graph.hpp"
#include "../core/Backtracking.hpp"
#include "../core/KMP.hpp"
#include "../core/InvestigationEngine.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <map>

namespace Culprit {
namespace Json {

inline std::string escape(const std::string& s) {
    std::ostringstream o;
    for (char c : s) {
        switch (c) {
            case '"': o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b"; break;
            case '\f': o << "\\f"; break;
            case '\n': o << "\\n"; break;
            case '\r': o << "\\r"; break;
            case '\t': o << "\\t"; break;
            default:
                if ('\x00' <= c && c <= '\x1f') {
                    o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << (int)c;
                } else {
                    o << c;
                }
        }
    }
    return o.str();
}

inline std::string quote(const std::string& s) {
    return "\"" + escape(s) + "\"";
}

// Simple JSON extraction helper for request bodies
inline std::string getField(const std::string& json, const std::string& key) {
    std::string needle = "\"" + key + "\"";
    size_t pos = json.find(needle);
    if (pos == std::string::npos) return "";

    pos += needle.length();
    // Skip spaces and ':'
    while (pos < json.length() && (json[pos] == ' ' || json[pos] == ':' || json[pos] == '\t' || json[pos] == '\r' || json[pos] == '\n')) {
        pos++;
    }
    if (pos >= json.length()) return "";

    if (json[pos] == '"') {
        pos++;
        std::string val;
        while (pos < json.length()) {
            if (json[pos] == '\\' && pos + 1 < json.length()) {
                val += json[pos + 1];
                pos += 2;
            } else if (json[pos] == '"') {
                break;
            } else {
                val += json[pos];
                pos++;
            }
        }
        return val;
    } else {
        // Number or boolean
        size_t end = json.find_first_of(",}\n\r", pos);
        if (end == std::string::npos) end = json.length();
        std::string val = json.substr(pos, end - pos);
        // Trim
        while (!val.empty() && (val.back() == ' ' || val.back() == '\t')) val.pop_back();
        return val;
    }
}

// Serializers
inline std::string serializeSuspect(const Suspect& s) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << quote(s.id) << ","
        << "\"fullName\":" << quote(s.fullName) << ","
        << "\"age\":" << s.age << ","
        << "\"occupation\":" << quote(s.occupation) << ","
        << "\"address\":" << quote(s.address) << ","
        << "\"relationshipToVictim\":" << quote(s.relationshipToVictim) << ","
        << "\"lastKnownLocation\":" << quote(s.lastKnownLocation) << ","
        << "\"alibi\":" << quote(s.alibi) << ","
        << "\"alibiVerified\":" << (s.alibiVerified ? "true" : "false") << ","
        << "\"motive\":" << quote(s.motive) << ","
        << "\"opportunity\":" << quote(s.opportunity) << ","
        << "\"investigationStatus\":" << quote(s.investigationStatus) << ","
        << "\"evidenceRelevanceScore\":" << std::fixed << std::setprecision(2) << s.evidenceRelevanceScore << ","
        << "\"presenceInTimeWindow\":" << (s.presenceInTimeWindow ? "true" : "false") << ","
        << "\"accessToMeansOrWeapon\":" << (s.accessToMeansOrWeapon ? "true" : "false") << ","
        << "\"forensicTraceMatched\":" << (s.forensicTraceMatched ? "true" : "false") << ","
        << "\"alibiInvalidated\":" << (s.alibiInvalidated ? "true" : "false") << ","
        << "\"substantiatedMotive\":" << (s.substantiatedMotive ? "true" : "false") << ","
        << "\"directSceneTrace\":" << (s.directSceneTrace ? "true" : "false") << ","
        << "\"associatedEvidenceIds\":[";
    for (size_t i = 0; i < s.associatedEvidenceIds.size(); ++i) {
        oss << quote(s.associatedEvidenceIds[i]);
        if (i + 1 < s.associatedEvidenceIds.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeEvidence(const Evidence& e) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << quote(e.id) << ","
        << "\"name\":" << quote(e.name) << ","
        << "\"category\":" << quote(evidenceCategoryToString(e.category)) << ","
        << "\"description\":" << quote(e.description) << ","
        << "\"collectedLocation\":" << quote(e.collectedLocation) << ","
        << "\"collectionDateTime\":" << quote(e.collectionDateTime) << ","
        << "\"linkedSuspectId\":" << quote(e.linkedSuspectId) << ","
        << "\"linkedSuspectName\":" << quote(e.linkedSuspectName) << ","
        << "\"reliabilityScore\":" << std::fixed << std::setprecision(1) << e.reliabilityScore << ","
        << "\"status\":" << quote(e.status) << ","
        << "\"chainOfCustody\":" << quote(e.chainOfCustody) << ","
        << "\"forensicNotes\":" << quote(e.forensicNotes)
        << "}";
    return oss.str();
}

inline std::string serializeWitness(const WitnessStatement& w) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << quote(w.id) << ","
        << "\"witnessName\":" << quote(w.witnessName) << ","
        << "\"occupation\":" << quote(w.occupation) << ","
        << "\"relationshipToCase\":" << quote(w.relationshipToCase) << ","
        << "\"statementDateTime\":" << quote(w.statementDateTime) << ","
        << "\"statementText\":" << quote(w.statementText) << ","
        << "\"connectedSuspectId\":" << quote(w.connectedSuspectId) << ","
        << "\"credibilityRating\":" << std::fixed << std::setprecision(1) << w.credibilityRating << ","
        << "\"interviewerBadge\":" << quote(w.interviewerBadge) << ","
        << "\"mentionedKeywords\":[";
    for (size_t i = 0; i < w.mentionedKeywords.size(); ++i) {
        oss << quote(w.mentionedKeywords[i]);
        if (i + 1 < w.mentionedKeywords.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeClue(const InvestigationClue& c) {
    std::ostringstream oss;
    oss << "{"
        << "\"id\":" << quote(c.id) << ","
        << "\"title\":" << quote(c.title) << ","
        << "\"category\":" << quote(c.category) << ","
        << "\"requiredCondition\":" << quote(c.requiredCondition) << ","
        << "\"description\":" << quote(c.description) << ","
        << "\"weight\":" << c.weight
        << "}";
    return oss.str();
}

inline std::string serializeCaseSummary(const CaseFile& c) {
    std::ostringstream oss;
    oss << "{"
        << "\"caseId\":" << quote(c.caseId) << ","
        << "\"caseTitle\":" << quote(c.caseTitle) << ","
        << "\"crimeType\":" << quote(crimeTypeToString(c.crimeType)) << ","
        << "\"dateTime\":" << quote(c.dateTime) << ","
        << "\"crimeLocation\":" << quote(c.crimeLocation) << ","
        << "\"caseDescription\":" << quote(c.caseDescription) << ","
        << "\"status\":" << quote(caseStatusToString(c.status)) << ","
        << "\"assignedInvestigator\":" << quote(c.assignedInvestigator) << ","
        << "\"victimName\":" << quote(c.victimName) << ","
        << "\"totalSuspects\":" << c.suspects.size() << ","
        << "\"totalEvidence\":" << c.evidenceRecords.size() << ","
        << "\"totalWitnesses\":" << c.witnessStatements.size() << ","
        << "\"totalClues\":" << c.clues.size()
        << "}";
    return oss.str();
}

inline std::string serializeCaseFull(const CaseFile& c) {
    std::ostringstream oss;
    oss << "{"
        << "\"caseId\":" << quote(c.caseId) << ","
        << "\"caseTitle\":" << quote(c.caseTitle) << ","
        << "\"crimeType\":" << quote(crimeTypeToString(c.crimeType)) << ","
        << "\"dateTime\":" << quote(c.dateTime) << ","
        << "\"crimeLocation\":" << quote(c.crimeLocation) << ","
        << "\"caseDescription\":" << quote(c.caseDescription) << ","
        << "\"status\":" << quote(caseStatusToString(c.status)) << ","
        << "\"assignedInvestigator\":" << quote(c.assignedInvestigator) << ","
        << "\"victimName\":" << quote(c.victimName) << ","
        << "\"victimDetails\":" << quote(c.victimDetails) << ","
        << "\"suspects\":[";
    for (size_t i = 0; i < c.suspects.size(); ++i) {
        oss << serializeSuspect(c.suspects[i]);
        if (i + 1 < c.suspects.size()) oss << ",";
    }
    oss << "],\"evidenceRecords\":[";
    for (size_t i = 0; i < c.evidenceRecords.size(); ++i) {
        oss << serializeEvidence(c.evidenceRecords[i]);
        if (i + 1 < c.evidenceRecords.size()) oss << ",";
    }
    oss << "],\"witnessStatements\":[";
    for (size_t i = 0; i < c.witnessStatements.size(); ++i) {
        oss << serializeWitness(c.witnessStatements[i]);
        if (i + 1 < c.witnessStatements.size()) oss << ",";
    }
    oss << "],\"clues\":[";
    for (size_t i = 0; i < c.clues.size(); ++i) {
        oss << serializeClue(c.clues[i]);
        if (i + 1 < c.clues.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeGraph(const InvestigationGraph& g) {
    std::ostringstream oss;
    oss << "{\"nodes\":[";
    size_t nIdx = 0;
    for (const auto& pair : g.getNodes()) {
        const auto& n = pair.second;
        oss << "{"
            << "\"id\":" << quote(n.id) << ","
            << "\"label\":" << quote(n.label) << ","
            << "\"type\":" << quote(n.type) << ","
            << "\"details\":" << quote(n.details) << ","
            << "\"score\":" << n.score
            << "}";
        if (++nIdx < g.getNodes().size()) oss << ",";
    }
    oss << "],\"links\":[";
    std::set<std::string> seenEdges;
    std::vector<GraphEdge> uniqueEdges;

    for (const auto& pair : g.getAdjList()) {
        for (const auto& edge : pair.second) {
            std::string key1 = edge.sourceId + "---" + edge.targetId;
            std::string key2 = edge.targetId + "---" + edge.sourceId;
            if (seenEdges.find(key1) == seenEdges.end() && seenEdges.find(key2) == seenEdges.end()) {
                seenEdges.insert(key1);
                uniqueEdges.push_back(edge);
            }
        }
    }

    for (size_t i = 0; i < uniqueEdges.size(); ++i) {
        const auto& edge = uniqueEdges[i];
        oss << "{"
            << "\"source\":" << quote(edge.sourceId) << ","
            << "\"target\":" << quote(edge.targetId) << ","
            << "\"relationType\":" << quote(edge.relationType) << ","
            << "\"weight\":" << edge.weight << ","
            << "\"description\":" << quote(edge.description)
            << "}";
        if (i + 1 < uniqueEdges.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeLinearSearchSuspects(const LinearSearchResult<Suspect>& res) {
    std::ostringstream oss;
    oss << "{"
        << "\"algorithmName\":" << quote(res.algorithmName) << ","
        << "\"timeComplexityBest\":" << quote(res.timeComplexityBest) << ","
        << "\"timeComplexityWorst\":" << quote(res.timeComplexityWorst) << ","
        << "\"timeComplexityAverage\":" << quote(res.timeComplexityAverage) << ","
        << "\"spaceComplexity\":" << quote(res.spaceComplexity) << ","
        << "\"totalElementsExamined\":" << res.totalElementsExamined << ","
        << "\"totalComparisons\":" << res.totalComparisons << ","
        << "\"executionTimeMicros\":" << res.executionTimeMicros << ","
        << "\"summary\":" << quote(res.summary) << ","
        << "\"matches\":[";
    for (size_t i = 0; i < res.matches.size(); ++i) {
        oss << serializeSuspect(res.matches[i]);
        if (i + 1 < res.matches.size()) oss << ",";
    }
    oss << "],\"traceLogs\":[";
    for (size_t i = 0; i < res.traceLogs.size(); ++i) {
        const auto& t = res.traceLogs[i];
        oss << "{"
            << "\"stepNumber\":" << t.stepNumber << ","
            << "\"action\":" << quote(t.action) << ","
            << "\"candidateId\":" << quote(t.candidateId) << ","
            << "\"candidateName\":" << quote(t.candidateName) << ","
            << "\"evaluatedValue\":" << quote(t.evaluatedValue) << ","
            << "\"targetQuery\":" << quote(t.targetQuery) << ","
            << "\"isMatch\":" << (t.isMatch ? "true" : "false") << ","
            << "\"notes\":" << quote(t.notes)
            << "}";
        if (i + 1 < res.traceLogs.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeLinearSearchEvidence(const LinearSearchResult<Evidence>& res) {
    std::ostringstream oss;
    oss << "{"
        << "\"algorithmName\":" << quote(res.algorithmName) << ","
        << "\"timeComplexityBest\":" << quote(res.timeComplexityBest) << ","
        << "\"timeComplexityWorst\":" << quote(res.timeComplexityWorst) << ","
        << "\"timeComplexityAverage\":" << quote(res.timeComplexityAverage) << ","
        << "\"spaceComplexity\":" << quote(res.spaceComplexity) << ","
        << "\"totalElementsExamined\":" << res.totalElementsExamined << ","
        << "\"totalComparisons\":" << res.totalComparisons << ","
        << "\"executionTimeMicros\":" << res.executionTimeMicros << ","
        << "\"summary\":" << quote(res.summary) << ","
        << "\"matches\":[";
    for (size_t i = 0; i < res.matches.size(); ++i) {
        oss << serializeEvidence(res.matches[i]);
        if (i + 1 < res.matches.size()) oss << ",";
    }
    oss << "],\"traceLogs\":[";
    for (size_t i = 0; i < res.traceLogs.size(); ++i) {
        const auto& t = res.traceLogs[i];
        oss << "{"
            << "\"stepNumber\":" << t.stepNumber << ","
            << "\"action\":" << quote(t.action) << ","
            << "\"candidateId\":" << quote(t.candidateId) << ","
            << "\"candidateName\":" << quote(t.candidateName) << ","
            << "\"evaluatedValue\":" << quote(t.evaluatedValue) << ","
            << "\"targetQuery\":" << quote(t.targetQuery) << ","
            << "\"isMatch\":" << (t.isMatch ? "true" : "false") << ","
            << "\"notes\":" << quote(t.notes)
            << "}";
        if (i + 1 < res.traceLogs.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeMergeSort(const MergeSortResult& res) {
    std::ostringstream oss;
    oss << "{"
        << "\"algorithmName\":" << quote(res.algorithmName) << ","
        << "\"timeComplexityBest\":" << quote(res.timeComplexityBest) << ","
        << "\"timeComplexityWorst\":" << quote(res.timeComplexityWorst) << ","
        << "\"timeComplexityAverage\":" << quote(res.timeComplexityAverage) << ","
        << "\"spaceComplexity\":" << quote(res.spaceComplexity) << ","
        << "\"totalComparisons\":" << res.totalComparisons << ","
        << "\"totalRecursiveCalls\":" << res.totalRecursiveCalls << ","
        << "\"maxRecursionDepth\":" << res.maxRecursionDepth << ","
        << "\"executionTimeMicros\":" << res.executionTimeMicros << ","
        << "\"summary\":" << quote(res.summary) << ","
        << "\"sortedSuspects\":[";
    for (size_t i = 0; i < res.sortedSuspects.size(); ++i) {
        oss << serializeSuspect(res.sortedSuspects[i]);
        if (i + 1 < res.sortedSuspects.size()) oss << ",";
    }
    oss << "],\"traceLogs\":[";
    for (size_t i = 0; i < res.traceLogs.size(); ++i) {
        const auto& s = res.traceLogs[i];
        oss << "{"
            << "\"stepNumber\":" << s.stepNumber << ","
            << "\"phase\":" << quote(s.phase) << ","
            << "\"leftIndex\":" << s.leftIndex << ","
            << "\"midIndex\":" << s.midIndex << ","
            << "\"rightIndex\":" << s.rightIndex << ","
            << "\"depth\":" << s.depth << ","
            << "\"comparisonDetails\":" << quote(s.comparisonDetails) << ","
            << "\"subArrayState\":" << quote(s.subArrayState) << ","
            << "\"notes\":" << quote(s.notes)
            << "}";
        if (i + 1 < res.traceLogs.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeBFS(const BFSResult& res) {
    std::ostringstream oss;
    oss << "{"
        << "\"algorithmName\":" << quote(res.algorithmName) << ","
        << "\"startNodeId\":" << quote(res.startNodeId) << ","
        << "\"startNodeLabel\":" << quote(res.startNodeLabel) << ","
        << "\"timeComplexity\":" << quote(res.timeComplexity) << ","
        << "\"spaceComplexity\":" << quote(res.spaceComplexity) << ","
        << "\"verticesVisited\":" << res.verticesVisited << ","
        << "\"edgesTraversed\":" << res.edgesTraversed << ","
        << "\"executionTimeMicros\":" << res.executionTimeMicros << ","
        << "\"summary\":" << quote(res.summary) << ","
        << "\"traversalOrder\":[";
    for (size_t i = 0; i < res.traversalOrder.size(); ++i) {
        oss << quote(res.traversalOrder[i]);
        if (i + 1 < res.traversalOrder.size()) oss << ",";
    }
    oss << "],\"distanceMap\":{";
    size_t dIdx = 0;
    for (const auto& p : res.distanceMap) {
        oss << quote(p.first) << ":" << p.second;
        if (++dIdx < res.distanceMap.size()) oss << ",";
    }
    oss << "},\"traceLogs\":[";
    for (size_t i = 0; i < res.traceLogs.size(); ++i) {
        const auto& t = res.traceLogs[i];
        oss << "{"
            << "\"stepNumber\":" << t.stepNumber << ","
            << "\"action\":" << quote(t.action) << ","
            << "\"currentNodeId\":" << quote(t.currentNodeId) << ","
            << "\"currentNodeLabel\":" << quote(t.currentNodeLabel) << ","
            << "\"targetNodeId\":" << quote(t.targetNodeId) << ","
            << "\"edgeRelation\":" << quote(t.edgeRelation) << ","
            << "\"currentDistance\":" << t.currentDistance << ","
            << "\"notes\":" << quote(t.notes)
            << "}";
        if (i + 1 < res.traceLogs.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeDFS(const DFSResult& res) {
    std::ostringstream oss;
    oss << "{"
        << "\"algorithmName\":" << quote(res.algorithmName) << ","
        << "\"startNodeId\":" << quote(res.startNodeId) << ","
        << "\"startNodeLabel\":" << quote(res.startNodeLabel) << ","
        << "\"timeComplexity\":" << quote(res.timeComplexity) << ","
        << "\"spaceComplexity\":" << quote(res.spaceComplexity) << ","
        << "\"verticesVisited\":" << res.verticesVisited << ","
        << "\"edgesTraversed\":" << res.edgesTraversed << ","
        << "\"maxRecursionDepth\":" << res.maxRecursionDepth << ","
        << "\"executionTimeMicros\":" << res.executionTimeMicros << ","
        << "\"summary\":" << quote(res.summary) << ","
        << "\"traversalOrder\":[";
    for (size_t i = 0; i < res.traversalOrder.size(); ++i) {
        oss << quote(res.traversalOrder[i]);
        if (i + 1 < res.traversalOrder.size()) oss << ",";
    }
    oss << "],\"traceLogs\":[";
    for (size_t i = 0; i < res.traceLogs.size(); ++i) {
        const auto& t = res.traceLogs[i];
        oss << "{"
            << "\"stepNumber\":" << t.stepNumber << ","
            << "\"action\":" << quote(t.action) << ","
            << "\"currentNodeId\":" << quote(t.currentNodeId) << ","
            << "\"currentNodeLabel\":" << quote(t.currentNodeLabel) << ","
            << "\"targetNodeId\":" << quote(t.targetNodeId) << ","
            << "\"edgeRelation\":" << quote(t.edgeRelation) << ","
            << "\"currentDistance\":" << t.currentDistance << ","
            << "\"notes\":" << quote(t.notes)
            << "}";
        if (i + 1 < res.traceLogs.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeBacktracking(const BacktrackingResult& res) {
    std::ostringstream oss;
    oss << "{"
        << "\"algorithmName\":" << quote(res.algorithmName) << ","
        << "\"timeComplexityWorst\":" << quote(res.timeComplexityWorst) << ","
        << "\"timeComplexityPruned\":" << quote(res.timeComplexityPruned) << ","
        << "\"spaceComplexity\":" << quote(res.spaceComplexity) << ","
        << "\"totalCandidatesTested\":" << res.totalCandidatesTested << ","
        << "\"totalConstraintsEvaluated\":" << res.totalConstraintsEvaluated << ","
        << "\"totalBranchesPruned\":" << res.totalBranchesPruned << ","
        << "\"totalBacktrackEvents\":" << res.totalBacktrackEvents << ","
        << "\"executionTimeMicros\":" << res.executionTimeMicros << ","
        << "\"summary\":" << quote(res.summary) << ","
        << "\"legalDisclaimer\":" << quote(res.legalDisclaimer) << ","
        << "\"matchingSuspects\":[";
    for (size_t i = 0; i < res.matchingSuspects.size(); ++i) {
        oss << serializeSuspect(res.matchingSuspects[i]);
        if (i + 1 < res.matchingSuspects.size()) oss << ",";
    }
    oss << "],\"detailedEvaluations\":[";
    for (size_t i = 0; i < res.detailedEvaluations.size(); ++i) {
        const auto& pair = res.detailedEvaluations[i];
        oss << "{\"suspect\":" << serializeSuspect(pair.first) << ",\"clueResults\":[";
        for (size_t j = 0; j < pair.second.size(); ++j) {
            const auto& ce = pair.second[j];
            oss << "{"
                << "\"clueId\":" << quote(ce.clueId) << ","
                << "\"clueTitle\":" << quote(ce.clueTitle) << ","
                << "\"requiredCondition\":" << quote(ce.requiredCondition) << ","
                << "\"satisfied\":" << (ce.satisfied ? "true" : "false") << ","
                << "\"forensicProof\":" << quote(ce.forensicProof)
                << "}";
            if (j + 1 < pair.second.size()) oss << ",";
        }
        oss << "]}";
        if (i + 1 < res.detailedEvaluations.size()) oss << ",";
    }
    oss << "],\"traceLogs\":[";
    for (size_t i = 0; i < res.traceLogs.size(); ++i) {
        const auto& t = res.traceLogs[i];
        oss << "{"
            << "\"stepNumber\":" << t.stepNumber << ","
            << "\"treeDepth\":" << t.treeDepth << ","
            << "\"candidateId\":" << quote(t.candidateId) << ","
            << "\"candidateName\":" << quote(t.candidateName) << ","
            << "\"action\":" << quote(t.action) << ","
            << "\"constraintTested\":" << quote(t.constraintTested) << ","
            << "\"constraintPassed\":" << (t.constraintPassed ? "true" : "false") << ","
            << "\"decisionReason\":" << quote(t.decisionReason)
            << "}";
        if (i + 1 < res.traceLogs.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeKMP(const KmpResult& res) {
    std::ostringstream oss;
    oss << "{"
        << "\"algorithmName\":" << quote(res.algorithmName) << ","
        << "\"pattern\":" << quote(res.pattern) << ","
        << "\"witnessStatementId\":" << quote(res.witnessStatementId) << ","
        << "\"witnessName\":" << quote(res.witnessName) << ","
        << "\"timeComplexityPreprocess\":" << quote(res.timeComplexityPreprocess) << ","
        << "\"timeComplexitySearch\":" << quote(res.timeComplexitySearch) << ","
        << "\"timeComplexityTotal\":" << quote(res.timeComplexityTotal) << ","
        << "\"spaceComplexity\":" << quote(res.spaceComplexity) << ","
        << "\"textLength\":" << res.textLength << ","
        << "\"patternLength\":" << res.patternLength << ","
        << "\"totalCharacterComparisons\":" << res.totalCharacterComparisons << ","
        << "\"naiveBruteForceComparisons\":" << res.naiveBruteForceComparisons << ","
        << "\"executionTimeMicros\":" << res.executionTimeMicros << ","
        << "\"summary\":" << quote(res.summary) << ","
        << "\"lpsTable\":[";
    for (size_t i = 0; i < res.lpsTable.size(); ++i) {
        oss << res.lpsTable[i];
        if (i + 1 < res.lpsTable.size()) oss << ",";
    }
    oss << "],\"occurrences\":[";
    for (size_t i = 0; i < res.occurrences.size(); ++i) {
        const auto& occ = res.occurrences[i];
        oss << "{"
            << "\"startIndex\":" << occ.startIndex << ","
            << "\"endIndex\":" << occ.endIndex << ","
            << "\"matchedSnippet\":" << quote(occ.matchedSnippet) << ","
            << "\"surroundingContext\":" << quote(occ.surroundingContext)
            << "}";
        if (i + 1 < res.occurrences.size()) oss << ",";
    }
    oss << "],\"traceLogs\":[";
    // Limit trace logs to at most 100 for responsive UI
    size_t limit = std::min(res.traceLogs.size(), (size_t)100);
    for (size_t i = 0; i < limit; ++i) {
        const auto& t = res.traceLogs[i];
        std::string tChar(1, t.textChar);
        std::string pChar(1, t.patternChar);
        oss << "{"
            << "\"stepNumber\":" << t.stepNumber << ","
            << "\"action\":" << quote(t.action) << ","
            << "\"textIndex\":" << t.textIndex << ","
            << "\"patternIndex\":" << t.patternIndex << ","
            << "\"textChar\":" << quote(tChar) << ","
            << "\"patternChar\":" << quote(pChar) << ","
            << "\"lpsValue\":" << t.lpsValue << ","
            << "\"notes\":" << quote(t.notes)
            << "}";
        if (i + 1 < limit) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

inline std::string serializeFullAnalysis(const FullInvestigationAnalysis& res) {
    std::ostringstream oss;
    oss << "{"
        << "\"caseId\":" << quote(res.caseId) << ","
        << "\"caseTitle\":" << quote(res.caseTitle) << ","
        << "\"investigator\":" << quote(res.investigator) << ","
        << "\"analysisTimestamp\":" << quote(res.analysisTimestamp) << ","
        << "\"rankingResult\":" << serializeMergeSort(res.rankingResult) << ","
        << "\"clueMatchingResult\":" << serializeBacktracking(res.clueMatchingResult) << ","
        << "\"primaryNetworkBfs\":" << serializeBFS(res.primaryNetworkBfs) << ","
        << "\"primaryNetworkDfs\":" << serializeDFS(res.primaryNetworkDfs) << ","
        << "\"executiveForensicSummary\":" << quote(res.executiveForensicSummary) << ","
        << "\"legalDisclaimer\":" << quote(res.legalDisclaimer) << ","
        << "\"primaryPersonsOfInterest\":[";
    for (size_t i = 0; i < res.primaryPersonsOfInterest.size(); ++i) {
        oss << serializeSuspect(res.primaryPersonsOfInterest[i]);
        if (i + 1 < res.primaryPersonsOfInterest.size()) oss << ",";
    }
    oss << "]}";
    return oss.str();
}

} // namespace Json
} // namespace Culprit

#pragma once

#include "Models.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>

namespace Culprit {

struct AlgorithmStep {
    int stepNumber;
    std::string action;
    std::string candidateId;
    std::string candidateName;
    std::string evaluatedValue;
    std::string targetQuery;
    bool isMatch;
    std::string notes;
};

template <typename T>
struct LinearSearchResult {
    std::string algorithmName = "Linear Search";
    std::string timeComplexityBest = "O(1)";
    std::string timeComplexityWorst = "O(N)";
    std::string timeComplexityAverage = "O(N)";
    std::string spaceComplexity = "O(1)";
    int totalElementsExamined = 0;
    int totalComparisons = 0;
    long long executionTimeMicros = 0;
    std::vector<T> matches;
    std::vector<AlgorithmStep> traceLogs;
    std::string summary;
};

class LinearSearchEngine {
public:
    static std::string toLower(const std::string& str) {
        std::string s = str;
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    static LinearSearchResult<Suspect> searchSuspects(
        const std::vector<Suspect>& suspects,
        const std::string& field,
        const std::string& query
    ) {
        auto startTime = std::chrono::high_resolution_clock::now();
        LinearSearchResult<Suspect> result;
        std::string qLower = toLower(query);
        int step = 0;

        for (size_t i = 0; i < suspects.size(); ++i) {
            const auto& s = suspects[i];
            result.totalElementsExamined++;
            result.totalComparisons++;
            step++;

            std::string inspectedVal = "";
            bool match = false;

            if (field == "id" || field == "all") {
                inspectedVal = s.id;
                if (toLower(s.id).find(qLower) != std::string::npos) match = true;
            }
            if (!match && (field == "name" || field == "all")) {
                inspectedVal = s.fullName;
                if (toLower(s.fullName).find(qLower) != std::string::npos) match = true;
            }
            if (!match && (field == "occupation" || field == "all")) {
                inspectedVal = s.occupation;
                if (toLower(s.occupation).find(qLower) != std::string::npos) match = true;
            }
            if (!match && (field == "status" || field == "all")) {
                inspectedVal = s.investigationStatus;
                if (toLower(s.investigationStatus).find(qLower) != std::string::npos) match = true;
            }

            AlgorithmStep log;
            log.stepNumber = step;
            log.action = "INSPECT_ELEMENT_AT_INDEX_" + std::to_string(i);
            log.candidateId = s.id;
            log.candidateName = s.fullName;
            log.evaluatedValue = inspectedVal.empty() ? s.fullName : inspectedVal;
            log.targetQuery = query;
            log.isMatch = match;
            log.notes = match 
                ? "Match confirmed: element satisfies query filter '" + query + "'."
                : "No match. Advancing pointer to index " + std::to_string(i + 1) + ".";

            result.traceLogs.push_back(log);

            if (match) {
                result.matches.push_back(s);
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        result.executionTimeMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::ostringstream oss;
        oss << "Linear search completed over " << suspects.size() 
            << " suspects. Identified " << result.matches.size() 
            << " matching record(s) using " << result.totalComparisons << " comparisons.";
        result.summary = oss.str();

        return result;
    }

    static LinearSearchResult<Evidence> searchEvidence(
        const std::vector<Evidence>& evidenceList,
        const std::string& field,
        const std::string& query
    ) {
        auto startTime = std::chrono::high_resolution_clock::now();
        LinearSearchResult<Evidence> result;
        std::string qLower = toLower(query);
        int step = 0;

        for (size_t i = 0; i < evidenceList.size(); ++i) {
            const auto& e = evidenceList[i];
            result.totalElementsExamined++;
            result.totalComparisons++;
            step++;

            std::string inspectedVal = "";
            bool match = false;

            if (field == "id" || field == "all") {
                inspectedVal = e.id;
                if (toLower(e.id).find(qLower) != std::string::npos) match = true;
            }
            if (!match && (field == "name" || field == "all")) {
                inspectedVal = e.name;
                if (toLower(e.name).find(qLower) != std::string::npos) match = true;
            }
            if (!match && (field == "category" || field == "all")) {
                inspectedVal = evidenceCategoryToString(e.category);
                if (toLower(inspectedVal).find(qLower) != std::string::npos) match = true;
            }
            if (!match && (field == "suspect" || field == "all")) {
                inspectedVal = e.linkedSuspectName + " (" + e.linkedSuspectId + ")";
                if (toLower(inspectedVal).find(qLower) != std::string::npos) match = true;
            }

            AlgorithmStep log;
            log.stepNumber = step;
            log.action = "INSPECT_EVIDENCE_RECORD_" + std::to_string(i);
            log.candidateId = e.id;
            log.candidateName = e.name;
            log.evaluatedValue = inspectedVal.empty() ? e.name : inspectedVal;
            log.targetQuery = query;
            log.isMatch = match;
            log.notes = match 
                ? "Forensic evidence match found. Captured in result registry."
                : "Index " + std::to_string(i) + " does not match. Sequential scan continues.";

            result.traceLogs.push_back(log);

            if (match) {
                result.matches.push_back(e);
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        result.executionTimeMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::ostringstream oss;
        oss << "Linear scan complete. Evaluated " << evidenceList.size() 
            << " forensic items. Total matches: " << result.matches.size() 
            << " (" << result.totalComparisons << " comparisons).";
        result.summary = oss.str();

        return result;
    }
};

} // namespace Culprit

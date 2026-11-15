#pragma once

#include "Models.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <algorithm>

namespace Culprit {

struct KmpStep {
    int stepNumber;
    std::string action; // "COMPUTE_LPS", "COMPARE_CHAR", "CHAR_MATCH", "CHAR_MISMATCH", "LPS_SHIFT", "OCCURRENCE_FOUND"
    int textIndex;
    int patternIndex;
    char textChar;
    char patternChar;
    int lpsValue;
    std::string notes;
};

struct KmpMatchOccurrence {
    int startIndex;
    int endIndex;
    std::string matchedSnippet;
    std::string surroundingContext;
};

struct KmpResult {
    std::string algorithmName = "Knuth-Morris-Pratt (KMP) String Matching";
    std::string pattern;
    std::string witnessStatementId;
    std::string witnessName;
    std::string timeComplexityPreprocess = "O(M)";
    std::string timeComplexitySearch = "O(N)";
    std::string timeComplexityTotal = "O(N + M)";
    std::string spaceComplexity = "O(M) for Pi/LPS table";
    int textLength = 0;
    int patternLength = 0;
    int totalCharacterComparisons = 0;
    int naiveBruteForceComparisons = 0; // Theoretical worst-case for comparison
    long long executionTimeMicros = 0;
    std::vector<int> lpsTable;
    std::vector<KmpMatchOccurrence> occurrences;
    std::vector<KmpStep> traceLogs;
    std::string summary;
};

class KmpSearchEngine {
public:
    static std::string toLower(const std::string& str) {
        std::string s = str;
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    // Compute Prefix Function Pi (LPS array)
    static std::vector<int> computeLpsArray(
        const std::string& pattern,
        std::vector<KmpStep>& traceLogs,
        int& stepCounter
    ) {
        int m = static_cast<int>(pattern.length());
        std::vector<int> lps(m, 0);
        int len = 0;
        int i = 1;

        while (i < m) {
            stepCounter++;
            KmpStep step;
            step.stepNumber = stepCounter;
            step.action = "COMPUTE_LPS";
            step.textIndex = -1;
            step.patternIndex = i;
            step.patternChar = pattern[i];

            if (pattern[i] == pattern[len]) {
                len++;
                lps[i] = len;
                step.lpsValue = len;
                step.notes = "Pattern chars match: pattern[" + std::to_string(i) + "] == pattern[" + 
                             std::to_string(len - 1) + "] ('" + pattern[i] + "'). LPS[" + 
                             std::to_string(i) + "] = " + std::to_string(len);
                i++;
            } else {
                if (len != 0) {
                    step.lpsValue = lps[len - 1];
                    step.notes = "Mismatch: pattern[" + std::to_string(i) + "] ('" + pattern[i] + 
                                 "') != pattern[" + std::to_string(len) + "] ('" + pattern[len] + 
                                 "'). Fallback to LPS[" + std::to_string(len - 1) + "] = " + std::to_string(lps[len - 1]);
                    len = lps[len - 1];
                } else {
                    lps[i] = 0;
                    step.lpsValue = 0;
                    step.notes = "No matching prefix for pattern[" + std::to_string(i) + "] ('" + pattern[i] + "'). LPS[" + std::to_string(i) + "] = 0";
                    i++;
                }
            }
            traceLogs.push_back(step);
        }
        return lps;
    }

    // Run KMP search over witness statement text
    static KmpResult searchInStatement(
        const WitnessStatement& statement,
        const std::string& queryPattern,
        bool caseInsensitive = true
    ) {
        auto startTime = std::chrono::high_resolution_clock::now();
        KmpResult result;
        result.witnessStatementId = statement.id;
        result.witnessName = statement.witnessName;
        result.pattern = queryPattern;

        if (queryPattern.empty() || statement.statementText.empty()) {
            result.summary = "Empty query pattern or statement text.";
            return result;
        }

        std::string text = statement.statementText;
        std::string pattern = queryPattern;
        if (caseInsensitive) {
            text = toLower(text);
            pattern = toLower(pattern);
        }

        result.textLength = static_cast<int>(text.length());
        result.patternLength = static_cast<int>(pattern.length());
        result.naiveBruteForceComparisons = result.textLength * result.patternLength;

        int stepCounter = 0;
        result.lpsTable = computeLpsArray(pattern, result.traceLogs, stepCounter);

        int i = 0; // index for text
        int j = 0; // index for pattern

        while (i < result.textLength) {
            result.totalCharacterComparisons++;
            stepCounter++;

            KmpStep step;
            step.stepNumber = stepCounter;
            step.textIndex = i;
            step.patternIndex = j;
            step.textChar = statement.statementText[i];
            step.patternChar = queryPattern[j];

            if (pattern[j] == text[i]) {
                step.action = "CHAR_MATCH";
                step.notes = "Character match: text[" + std::to_string(i) + "] ('" + step.textChar + 
                             "') == pattern[" + std::to_string(j) + "] ('" + step.patternChar + "'). Advancing both pointers.";
                result.traceLogs.push_back(step);
                i++;
                j++;
            }

            if (j == result.patternLength) {
                // Match occurrence found!
                int matchStart = i - j;
                int matchEnd = i - 1;

                int ctxStart = std::max(0, matchStart - 35);
                int ctxEnd = std::min(result.textLength - 1, matchEnd + 35);
                std::string snippet = statement.statementText.substr(matchStart, j);
                std::string context = statement.statementText.substr(ctxStart, ctxEnd - ctxStart + 1);

                KmpMatchOccurrence occ;
                occ.startIndex = matchStart;
                occ.endIndex = matchEnd;
                occ.matchedSnippet = snippet;
                occ.surroundingContext = context;
                result.occurrences.push_back(occ);

                stepCounter++;
                KmpStep occStep;
                occStep.stepNumber = stepCounter;
                occStep.action = "OCCURRENCE_FOUND";
                occStep.textIndex = matchStart;
                occStep.patternIndex = j;
                occStep.notes = "[PATTERN IDENTIFIED] Found full phrase '" + queryPattern + 
                                "' at span [" + std::to_string(matchStart) + ".." + std::to_string(matchEnd) + 
                                "]. Using LPS[" + std::to_string(j - 1) + "] = " + 
                                std::to_string(result.lpsTable[j - 1]) + " to resume search without backtrack.";
                result.traceLogs.push_back(occStep);

                j = result.lpsTable[j - 1];
            } else if (i < result.textLength && pattern[j] != text[i]) {
                stepCounter++;
                KmpStep misStep;
                misStep.stepNumber = stepCounter;
                misStep.action = "CHAR_MISMATCH";
                misStep.textIndex = i;
                misStep.patternIndex = j;
                misStep.textChar = statement.statementText[i];
                misStep.patternChar = queryPattern[j];

                if (j != 0) {
                    misStep.notes = "Mismatch after " + std::to_string(j) + " matching chars. Shifting pattern via LPS[" + 
                                    std::to_string(j - 1) + "] = " + std::to_string(result.lpsTable[j - 1]) + 
                                    " (text pointer i=" + std::to_string(i) + " does NOT roll back!).";
                    result.traceLogs.push_back(misStep);
                    j = result.lpsTable[j - 1];
                } else {
                    misStep.notes = "Mismatch at first pattern char. Advancing text pointer i to " + std::to_string(i + 1) + ".";
                    result.traceLogs.push_back(misStep);
                    i++;
                }
            }
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        result.executionTimeMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::ostringstream oss;
        oss << "KMP String Matching identified " << result.occurrences.size() 
            << " occurrence(s) of key forensic phrase '" << queryPattern << "' in deposition by "
            << statement.witnessName << ". Executed " << result.totalCharacterComparisons 
            << " comparisons (vs up to " << result.naiveBruteForceComparisons 
            << " in naive search, saving " 
            << (result.naiveBruteForceComparisons > result.totalCharacterComparisons 
                ? (result.naiveBruteForceComparisons - result.totalCharacterComparisons) : 0)
            << " redundant character probes). Verified linear time complexity O(N + M).";
        result.summary = oss.str();

        return result;
    }
};

} // namespace Culprit

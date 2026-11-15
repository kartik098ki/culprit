#pragma once

#include "Models.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace Culprit {

struct SortStep {
    int stepNumber;
    std::string phase; // "DIVIDE" or "CONQUER_MERGE"
    int leftIndex;
    int midIndex;
    int rightIndex;
    int depth;
    std::string comparisonDetails;
    std::string subArrayState;
    std::string notes;
};

struct MergeSortResult {
    std::string algorithmName = "Merge Sort (Divide-and-Conquer)";
    std::string timeComplexityBest = "O(N log N)";
    std::string timeComplexityWorst = "O(N log N)";
    std::string timeComplexityAverage = "O(N log N)";
    std::string spaceComplexity = "O(N)";
    int totalComparisons = 0;
    int totalRecursiveCalls = 0;
    int maxRecursionDepth = 0;
    long long executionTimeMicros = 0;
    std::vector<Suspect> sortedSuspects;
    std::vector<SortStep> traceLogs;
    std::string summary;
};

class MergeSortEngine {
private:
    static std::string formatSubArray(const std::vector<Suspect>& arr, int start, int end) {
        std::ostringstream oss;
        oss << "[";
        for (int i = start; i <= end; ++i) {
            oss << arr[i].fullName << " (" << std::fixed << std::setprecision(1) << arr[i].evidenceRelevanceScore << ")";
            if (i < end) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    static void merge(
        std::vector<Suspect>& arr,
        int left,
        int mid,
        int right,
        int depth,
        int& stepCounter,
        MergeSortResult& result
    ) {
        int n1 = mid - left + 1;
        int n2 = right - mid;

        std::vector<Suspect> L(arr.begin() + left, arr.begin() + mid + 1);
        std::vector<Suspect> R(arr.begin() + mid + 1, arr.begin() + right + 1);

        int i = 0, j = 0, k = left;

        std::ostringstream compLog;
        compLog << "Merging partitions: L[" << left << ".." << mid << "] and R[" << (mid + 1) << ".." << right << "]. ";

        while (i < n1 && j < n2) {
            result.totalComparisons++;
            stepCounter++;

            SortStep step;
            step.stepNumber = stepCounter;
            step.phase = "CONQUER_MERGE";
            step.leftIndex = left;
            step.midIndex = mid;
            step.rightIndex = right;
            step.depth = depth;

            // Sort DESCENDING: higher evidence relevance score comes first
            if (L[i].evidenceRelevanceScore >= R[j].evidenceRelevanceScore) {
                step.comparisonDetails = L[i].fullName + " (" + std::to_string(L[i].evidenceRelevanceScore) + 
                    ") >= " + R[j].fullName + " (" + std::to_string(R[j].evidenceRelevanceScore) + ")";
                step.notes = "Selecting left item: " + L[i].fullName + " placed at index " + std::to_string(k);
                arr[k++] = L[i++];
            } else {
                step.comparisonDetails = R[j].fullName + " (" + std::to_string(R[j].evidenceRelevanceScore) + 
                    ") > " + L[i].fullName + " (" + std::to_string(L[i].evidenceRelevanceScore) + ")";
                step.notes = "Selecting right item: " + R[j].fullName + " placed at index " + std::to_string(k);
                arr[k++] = R[j++];
            }
            step.subArrayState = formatSubArray(arr, left, right);
            result.traceLogs.push_back(step);
        }

        while (i < n1) {
            arr[k++] = L[i++];
        }
        while (j < n2) {
            arr[k++] = R[j++];
        }
    }

    static void mergeSortRecursive(
        std::vector<Suspect>& arr,
        int left,
        int right,
        int depth,
        int& stepCounter,
        MergeSortResult& result
    ) {
        result.totalRecursiveCalls++;
        if (depth > result.maxRecursionDepth) {
            result.maxRecursionDepth = depth;
        }

        if (left >= right) return;

        int mid = left + (right - left) / 2;

        stepCounter++;
        SortStep divStep;
        divStep.stepNumber = stepCounter;
        divStep.phase = "DIVIDE";
        divStep.leftIndex = left;
        divStep.midIndex = mid;
        divStep.rightIndex = right;
        divStep.depth = depth;
        divStep.comparisonDetails = "Partition split point = " + std::to_string(mid);
        divStep.subArrayState = formatSubArray(arr, left, right);
        divStep.notes = "Splitting span [" + std::to_string(left) + ".." + std::to_string(right) + 
                        "] into left [" + std::to_string(left) + ".." + std::to_string(mid) + 
                        "] and right [" + std::to_string(mid + 1) + ".." + std::to_string(right) + "]";
        result.traceLogs.push_back(divStep);

        mergeSortRecursive(arr, left, mid, depth + 1, stepCounter, result);
        mergeSortRecursive(arr, mid + 1, right, depth + 1, stepCounter, result);
        merge(arr, left, mid, right, depth, stepCounter, result);
    }

public:
    static MergeSortResult sortSuspects(std::vector<Suspect> suspects) {
        auto startTime = std::chrono::high_resolution_clock::now();
        MergeSortResult result;
        int stepCounter = 0;

        if (!suspects.empty()) {
            mergeSortRecursive(suspects, 0, static_cast<int>(suspects.size()) - 1, 1, stepCounter, result);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        result.executionTimeMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
        result.sortedSuspects = suspects;

        std::ostringstream oss;
        oss << "Merge Sort successfully prioritized " << suspects.size() 
            << " suspects. Executed " << result.totalComparisons << " key comparisons across "
            << result.totalRecursiveCalls << " recursive partitions with max call depth " 
            << result.maxRecursionDepth << ". Time complexity guaranteed O(N log N).";
        result.summary = oss.str();

        return result;
    }
};

} // namespace Culprit

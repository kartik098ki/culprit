#pragma once

#include "Models.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

namespace Culprit {

struct ConstraintEvaluation {
    std::string clueId;
    std::string clueTitle;
    std::string requiredCondition;
    bool satisfied;
    std::string forensicProof;
};

struct BacktrackingStep {
    int stepNumber;
    int treeDepth;
    std::string candidateId;
    std::string candidateName;
    std::string action; // "EVALUATE_CANDIDATE", "CHECK_CONSTRAINT", "BRANCH_PRUNED", "BACKTRACK", "SOLUTION_FOUND"
    std::string constraintTested;
    bool constraintPassed;
    std::string decisionReason;
    std::vector<std::string> currentSurvivingCandidates;
};

struct BacktrackingResult {
    std::string algorithmName = "Backtracking / Constraint Satisfaction Elimination Engine";
    std::string timeComplexityWorst = "O(N * 2^C) or O(N * C)";
    std::string timeComplexityPruned = "O(N * C_avg) with early branch cutoffs";
    std::string spaceComplexity = "O(C) recursion call stack";
    int totalCandidatesTested = 0;
    int totalConstraintsEvaluated = 0;
    int totalBranchesPruned = 0;
    int totalBacktrackEvents = 0;
    long long executionTimeMicros = 0;
    std::vector<Suspect> matchingSuspects;
    std::vector<std::pair<Suspect, std::vector<ConstraintEvaluation>>> detailedEvaluations;
    std::vector<BacktrackingStep> traceLogs;
    std::string summary;
    std::string legalDisclaimer = "EDUCATIONAL FORENSIC SIMULATION: Matching candidates represent persons of high investigative interest who satisfy all empirical constraints. In accordance with evidentiary standards, algorithmic match does not constitute judicial determination of criminal guilt.";
};

class BacktrackingClueEngine {
private:
    static bool evaluateSingleConstraint(
        const Suspect& s,
        const InvestigationClue& clue,
        std::string& proofNotes
    ) {
        const std::string& cond = clue.requiredCondition;

        if (cond == "TIME_WINDOW_OPPORTUNITY" || cond == "TIME_WINDOW_2330_0030" || clue.category == "TIMELINE") {
            if (s.presenceInTimeWindow) {
                proofNotes = "Geographic proximity confirmed. Suspect verified present at/near perimeter during window.";
                return true;
            } else {
                proofNotes = "Opportunity absent: Suspect telemetry or distance precludes physical presence in time window.";
                return false;
            }
        }
        else if (cond == "ACCESS_TO_WEAPON" || cond == "TECHNICAL_SECURITY_ACCESS" || clue.category == "WEAPON_MEANS") {
            if (s.accessToMeansOrWeapon) {
                proofNotes = "Means confirmed: Suspect possessed direct physical access to weapon/terminal.";
                return true;
            } else {
                proofNotes = "Lack of means: No documented keycard, physical capability, or tool access.";
                return false;
            }
        }
        else if (cond == "FORENSIC_TRACE_MATCH" || cond == "BIOLOGICAL_OR_DIGITAL_TRACE" || clue.category == "FORENSICS") {
            if (s.forensicTraceMatched) {
                proofNotes = "Forensic corroboration: Physical fibers, blood type O+, digital IP, or fingerprints linked to suspect.";
                return true;
            } else {
                proofNotes = "Forensic negative: No biological, chemical, or network fingerprint links suspect.";
                return false;
            }
        }
        else if (cond == "ALIBI_INVALIDATED" || cond == "UNVERIFIED_ALIBI" || clue.category == "ALIBI") {
            if (s.alibiInvalidated) {
                proofNotes = "Alibi collapsed: Claimed whereabouts contradicted by surveillance, phone logs, or witnesses.";
                return true;
            } else {
                proofNotes = "Solid alibi: Suspect alibi independently corroborated by external objective records.";
                return false;
            }
        }
        else if (cond == "SUBSTANTIATED_MOTIVE" || cond == "FINANCIAL_OR_PERSONAL_MOTIVE" || clue.category == "MOTIVE") {
            if (s.substantiatedMotive) {
                proofNotes = "Motive established: Documented inheritance dispute, imminent audit exposure, or debt.";
                return true;
            } else {
                proofNotes = "Motive insufficient: No clear financial, personal, or defensive incentive established.";
                return false;
            }
        }
        else if (cond == "DIRECT_SCENE_TRACE" || cond == "PHYSICAL_CONTACT" || clue.category == "DIRECT_CONTACT") {
            if (s.directSceneTrace) {
                proofNotes = "Direct trace validated: Audio capture, footwear impression, or personal item recovered on scene.";
                return true;
            } else {
                proofNotes = "Direct trace absent: No physical or digital artifacts establish intimate crime contact.";
                return false;
            }
        }

        // Generic fallback by checking associated evidence count or score
        if (s.evidenceRelevanceScore > 40.0) {
            proofNotes = "Heuristic threshold passed: High composite evidence correlation (" + std::to_string(s.evidenceRelevanceScore) + ").";
            return true;
        }

        proofNotes = "Generic constraint failed: Evidence relevance below requisite threshold.";
        return false;
    }

    static void backtrackSolve(
        const std::vector<Suspect>& suspects,
        size_t suspectIdx,
        const std::vector<InvestigationClue>& clues,
        size_t clueIdx,
        std::vector<ConstraintEvaluation>& currentEval,
        std::vector<std::string>& currentSurviving,
        int& stepCount,
        BacktrackingResult& result
    ) {
        if (suspectIdx >= suspects.size()) {
            return;
        }

        const auto& s = suspects[suspectIdx];

        // If we are at the start of evaluating this suspect
        if (clueIdx == 0) {
            result.totalCandidatesTested++;
            stepCount++;

            BacktrackingStep startStep;
            startStep.stepNumber = stepCount;
            startStep.treeDepth = 1;
            startStep.candidateId = s.id;
            startStep.candidateName = s.fullName;
            startStep.action = "EVALUATE_CANDIDATE";
            startStep.constraintTested = "INITIATE_CLUE_VERIFICATION";
            startStep.constraintPassed = true;
            startStep.decisionReason = "Examining candidate " + s.fullName + " against " + 
                                       std::to_string(clues.size()) + " empirical case clues.";
            startStep.currentSurvivingCandidates = currentSurviving;
            result.traceLogs.push_back(startStep);
        }

        // Base case: all clues checked for this suspect
        if (clueIdx >= clues.size()) {
            // ALL clues satisfied!
            result.matchingSuspects.push_back(s);
            result.detailedEvaluations.push_back({s, currentEval});
            currentSurviving.push_back(s.fullName);

            stepCount++;
            BacktrackingStep solStep;
            solStep.stepNumber = stepCount;
            solStep.treeDepth = static_cast<int>(clues.size());
            solStep.candidateId = s.id;
            solStep.candidateName = s.fullName;
            solStep.action = "SOLUTION_FOUND";
            solStep.constraintTested = "ALL_CLUES_PASSED";
            solStep.constraintPassed = true;
            solStep.decisionReason = "[MATCH CONFIRMED] Candidate " + s.fullName + 
                                     " strictly satisfies all " + std::to_string(clues.size()) + 
                                     " investigative clues! Retained in prime suspect cohort.";
            solStep.currentSurvivingCandidates = currentSurviving;
            result.traceLogs.push_back(solStep);

            // Move to next suspect
            std::vector<ConstraintEvaluation> nextEval;
            backtrackSolve(suspects, suspectIdx + 1, clues, 0, nextEval, currentSurviving, stepCount, result);
            return;
        }

        // Test clue at clueIdx
        const auto& clue = clues[clueIdx];
        result.totalConstraintsEvaluated++;

        std::string proofNotes;
        bool satisfied = evaluateSingleConstraint(s, clue, proofNotes);

        ConstraintEvaluation eval;
        eval.clueId = clue.id;
        eval.clueTitle = clue.title;
        eval.requiredCondition = clue.requiredCondition;
        eval.satisfied = satisfied;
        eval.forensicProof = proofNotes;
        currentEval.push_back(eval);

        stepCount++;
        BacktrackingStep checkStep;
        checkStep.stepNumber = stepCount;
        checkStep.treeDepth = static_cast<int>(clueIdx + 1);
        checkStep.candidateId = s.id;
        checkStep.candidateName = s.fullName;
        checkStep.action = "CHECK_CONSTRAINT";
        checkStep.constraintTested = clue.title + " [" + clue.requiredCondition + "]";
        checkStep.constraintPassed = satisfied;
        checkStep.decisionReason = proofNotes;
        checkStep.currentSurvivingCandidates = currentSurviving;
        result.traceLogs.push_back(checkStep);

        if (satisfied) {
            // Recurse to next clue
            backtrackSolve(suspects, suspectIdx, clues, clueIdx + 1, currentEval, currentSurviving, stepCount, result);
        } else {
            // CONSTRAINT VIOLATED -> PRUNE BRANCH!
            result.totalBranchesPruned++;
            result.totalBacktrackEvents++;

            stepCount++;
            BacktrackingStep pruneStep;
            pruneStep.stepNumber = stepCount;
            pruneStep.treeDepth = static_cast<int>(clueIdx + 1);
            pruneStep.candidateId = s.id;
            pruneStep.candidateName = s.fullName;
            pruneStep.action = "BRANCH_PRUNED";
            pruneStep.constraintTested = clue.title;
            pruneStep.constraintPassed = false;
            pruneStep.decisionReason = "[PRUNING DECISION] " + s.fullName + " failed Clue " + 
                                       std::to_string(clueIdx + 1) + " (" + clue.title + "). " +
                                       "Pruned " + std::to_string(clues.size() - clueIdx - 1) + 
                                       " subsequent sub-branches. Backtracking to next candidate.";
            pruneStep.currentSurvivingCandidates = currentSurviving;
            result.traceLogs.push_back(pruneStep);

            // Backtrack to next suspect
            std::vector<ConstraintEvaluation> nextEval;
            backtrackSolve(suspects, suspectIdx + 1, clues, 0, nextEval, currentSurviving, stepCount, result);
        }
    }

public:
    static BacktrackingResult runClueMatching(
        const std::vector<Suspect>& suspects,
        const std::vector<InvestigationClue>& clues
    ) {
        auto startTime = std::chrono::high_resolution_clock::now();
        BacktrackingResult result;
        int stepCount = 0;
        std::vector<ConstraintEvaluation> initialEval;
        std::vector<std::string> surviving;

        if (!suspects.empty() && !clues.empty()) {
            backtrackSolve(suspects, 0, clues, 0, initialEval, surviving, stepCount, result);
        }

        auto endTime = std::chrono::high_resolution_clock::now();
        result.executionTimeMicros = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

        std::ostringstream oss;
        oss << "Constraint Satisfaction Backtracking completed. Evaluated " 
            << result.totalCandidatesTested << " suspect candidates against " 
            << clues.size() << " empirical case clues. Executed " 
            << result.totalConstraintsEvaluated << " constraint checks. "
            << "Pruned " << result.totalBranchesPruned << " inconsistent hypotheses. "
            << "Found " << result.matchingSuspects.size() << " suspect(s) meeting 100% of required clues.";
        result.summary = oss.str();

        return result;
    }
};

} // namespace Culprit

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>

namespace Culprit {

enum class CrimeType {
    HOMICIDE,
    CYBER_HEIST,
    POISONING_CONSPIRACY,
    EXTORTION,
    KIDNAPPING
};

inline std::string crimeTypeToString(CrimeType type) {
    switch (type) {
        case CrimeType::HOMICIDE: return "Homicide";
        case CrimeType::CYBER_HEIST: return "Cyber Heist";
        case CrimeType::POISONING_CONSPIRACY: return "Poisoning Conspiracy";
        case CrimeType::EXTORTION: return "Extortion";
        case CrimeType::KIDNAPPING: return "Kidnapping";
        default: return "Unknown";
    }
}

enum class CaseStatus {
    ACTIVE_INVESTIGATION,
    EVIDENCE_ANALYSIS,
    FORENSIC_TRIAGE,
    SOLVED_PENDING_PROSECUTION,
    COLD_CASE
};

inline std::string caseStatusToString(CaseStatus status) {
    switch (status) {
        case CaseStatus::ACTIVE_INVESTIGATION: return "Active Investigation";
        case CaseStatus::EVIDENCE_ANALYSIS: return "Evidence Analysis";
        case CaseStatus::FORENSIC_TRIAGE: return "Forensic Triage";
        case CaseStatus::SOLVED_PENDING_PROSECUTION: return "Solved / Ready for Prosecution";
        case CaseStatus::COLD_CASE: return "Cold Case";
        default: return "Active Investigation";
    }
}

enum class EvidenceCategory {
    PHYSICAL,
    DIGITAL,
    WITNESS,
    DOCUMENTARY,
    CIRCUMSTANTIAL
};

inline std::string evidenceCategoryToString(EvidenceCategory cat) {
    switch (cat) {
        case EvidenceCategory::PHYSICAL: return "Physical Evidence";
        case EvidenceCategory::DIGITAL: return "Digital Evidence";
        case EvidenceCategory::WITNESS: return "Witness Evidence";
        case EvidenceCategory::DOCUMENTARY: return "Documentary Evidence";
        case EvidenceCategory::CIRCUMSTANTIAL: return "Circumstantial Evidence";
        default: return "General Evidence";
    }
}

inline double getCategoryWeight(EvidenceCategory cat) {
    switch (cat) {
        case EvidenceCategory::PHYSICAL: return 1.0;
        case EvidenceCategory::DIGITAL: return 0.90;
        case EvidenceCategory::DOCUMENTARY: return 0.85;
        case EvidenceCategory::WITNESS: return 0.75;
        case EvidenceCategory::CIRCUMSTANTIAL: return 0.50;
        default: return 0.60;
    }
}

struct Evidence {
    std::string id;
    std::string name;
    EvidenceCategory category;
    std::string description;
    std::string collectedLocation;
    std::string collectionDateTime;
    std::string linkedSuspectId;
    std::string linkedSuspectName;
    double reliabilityScore; // 0.0 to 100.0
    std::string status;      // "In Custody", "Lab Tested", "Corroborated", "Contaminated"
    std::string chainOfCustody;
    std::string forensicNotes;
};

struct Suspect {
    std::string id;
    std::string fullName;
    int age;
    std::string occupation;
    std::string address;
    std::string relationshipToVictim;
    std::string lastKnownLocation;
    std::string alibi;
    bool alibiVerified;
    std::string motive;
    std::string opportunity;
    std::vector<std::string> associatedEvidenceIds;
    std::string investigationStatus; // "Prime Suspect", "Person of Interest", "Alibi Cleared", "Under Surveillance"
    double evidenceRelevanceScore;   // Computed composite score (0.0 to 100.0)
    
    // Attributes for Backtracking Constraint Satisfaction:
    bool presenceInTimeWindow;     // Present at/near crime scene during critical interval
    bool accessToMeansOrWeapon;    // Access to lethal instrument or security access
    bool forensicTraceMatched;     // Physical/digital/biological trace match
    bool alibiInvalidated;         // False or uncorroborated alibi
    bool substantiatedMotive;      // Verifiable motive (financial/revenge/blackmail)
    bool directSceneTrace;         // Fingerprint, audio capture, DNA, or direct observation
};

struct WitnessStatement {
    std::string id;
    std::string witnessName;
    std::string occupation;
    std::string relationshipToCase;
    std::string statementDateTime;
    std::string statementText;
    std::vector<std::string> mentionedKeywords;
    std::string connectedSuspectId;
    double credibilityRating; // 0.0 to 100.0
    std::string interviewerBadge;
};

struct InvestigationClue {
    std::string id;
    std::string title;
    std::string description;
    std::string requiredCondition; // e.g., "TIME_WINDOW_2330_0030", "ACCESS_TO_WEAPON", "FORENSIC_TRACE"
    std::string category;          // "TIMELINE", "WEAPON_MEANS", "FORENSICS", "ALIBI", "MOTIVE", "DIRECT_CONTACT"
    int weight;                    // 1-10
};

struct GraphEdge {
    std::string sourceId;
    std::string targetId;
    std::string relationType; // "OWNS_WEAPON", "SEEN_WITH", "BUSINESS_DISPUTE", "FINGERPRINT_LINK", "ALIBI_DEP"
    double weight;            // 0.0 to 1.0 (significance)
    std::string description;
};

struct GraphNode {
    std::string id;
    std::string label;
    std::string type; // "VICTIM", "SUSPECT", "WITNESS", "EVIDENCE", "LOCATION"
    std::string details;
    double score;
};

struct CaseFile {
    std::string caseId;
    std::string caseTitle;
    CrimeType crimeType;
    std::string dateTime;
    std::string crimeLocation;
    std::string caseDescription;
    CaseStatus status;
    std::string assignedInvestigator;
    std::string victimName;
    std::string victimDetails;
    
    std::vector<Suspect> suspects;
    std::vector<Evidence> evidenceRecords;
    std::vector<WitnessStatement> witnessStatements;
    std::vector<InvestigationClue> clues;
    std::vector<GraphEdge> relationships;
};

} // namespace Culprit

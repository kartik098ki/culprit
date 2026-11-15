#pragma once

#include "Models.hpp"
#include <vector>

namespace Culprit {

class CaseRepository {
public:
    static std::vector<CaseFile> getPreloadedCases() {
        std::vector<CaseFile> cases;

        // =====================================================================
        // CASE 1: THE MIDNIGHT MURDER (Primary Delhi Investigation)
        // =====================================================================
        CaseFile c1;
        c1.caseId = "CASE-DEL-2026-089";
        c1.caseTitle = "THE MIDNIGHT MURDER";
        c1.crimeType = CrimeType::HOMICIDE;
        c1.dateTime = "2026-10-14 23:45 IST";
        c1.crimeLocation = "Bungalow 14-B, Amrita Shergill Marg, New Delhi";
        c1.caseDescription = "Fictional homicide investigation into the demise of Vikramaditya Singhania (54), industrialist and art patron, found deceased in his ground-floor private study. Preliminary forensic post-mortem indicates fatal cranial blunt-force trauma occurring between 23:30 and 00:15 IST, coupled with chemical incapacitation traces in a half-empty brandy decanter. House surveillance network disabled at junction box.";
        c1.status = CaseStatus::ACTIVE_INVESTIGATION;
        c1.assignedInvestigator = "Inspector General A. K. Shrivastava (Badge #7412)";
        c1.victimName = "Vikramaditya Singhania";
        c1.victimDetails = "Age 54, Industrialist & Managing Director of Singhania Heritage Arts Foundation. Found deceased behind mahogany desk.";

        // Suspect 1
        Suspect s1_1;
        s1_1.id = "SUSP-01";
        s1_1.fullName = "Dev Singhania";
        s1_1.age = 32;
        s1_1.occupation = "Disinherited Heir & Venture Investor";
        s1_1.address = "Penthouse 902, Golf Links Enclave, New Delhi";
        s1_1.relationshipToVictim = "Estranged Son; cut from ancestral trust";
        s1_1.lastKnownLocation = "Spotted in dark grey sedan near Amrita Shergill Marg round-about at 23:55";
        s1_1.alibi = "Claims to have been dining alone at an upscale club in Aerocity until 01:00 AM";
        s1_1.alibiVerified = false;
        s1_1.motive = "Inheritance cutoff: ₹120 Crore estate was being irrevocably diverted to a public arts foundation";
        s1_1.opportunity = "Retained emergency latch key to side garden French window; knows estate perimeter";
        s1_1.investigationStatus = "Prime Suspect";
        s1_1.evidenceRelevanceScore = 94.5;
        s1_1.presenceInTimeWindow = true;
        s1_1.accessToMeansOrWeapon = true;
        s1_1.forensicTraceMatched = true; // Torn cashmere fiber snagged on window latch
        s1_1.alibiInvalidated = true;     // Aerocity club valet logs show he departed at 22:40, not 01:00
        s1_1.substantiatedMotive = true;  // Torn draft will found in fireplace
        s1_1.directSceneTrace = true;     // Muddy tire tracks matching his grey sedan at rear gate
        s1_1.associatedEvidenceIds = {"EVD-01", "EVD-04", "EVD-05", "EVD-07"};
        c1.suspects.push_back(s1_1);

        // Suspect 2
        Suspect s1_2;
        s1_2.id = "SUSP-02";
        s1_2.fullName = "Meera Sen";
        s1_2.age = 41;
        s1_2.occupation = "Private Art Curator & Trust Partner";
        s1_2.address = "C-44, Defence Colony, New Delhi";
        s1_2.relationshipToVictim = "Art advisor and business partner; co-signatory on acquisitions";
        s1_2.lastKnownLocation = "Left Singhania bungalow study at 21:30 after heated inventory review";
        s1_2.alibi = "Was reviewing audit catalogs at her Defence Colony gallery with assistant";
        s1_2.alibiVerified = true;
        s1_2.motive = "Facing impending criminal audit for ₹25 Lakhs counterfeit sculpture sales";
        s1_2.opportunity = "Had full access to study earlier in the evening; knowledge of antique brass statuette";
        s1_2.investigationStatus = "Person of Interest";
        s1_2.evidenceRelevanceScore = 68.0;
        s1_2.presenceInTimeWindow = false; // Assistant and toll plaza logs confirm she was in Defence Colony after 22:30
        s1_2.accessToMeansOrWeapon = true;
        s1_2.forensicTraceMatched = false;
        s1_2.alibiInvalidated = false;
        s1_2.substantiatedMotive = true;
        s1_2.directSceneTrace = false;
        s1_2.associatedEvidenceIds = {"EVD-01", "EVD-06", "EVD-08"};
        c1.suspects.push_back(s1_2);

        // Suspect 3
        Suspect s1_3;
        s1_3.id = "SUSP-03";
        s1_3.fullName = "Rajeshwar Malhotra";
        s1_3.age = 48;
        s1_3.occupation = "Senior Advocate & Estate Legal Counsel";
        s1_3.address = "Flat 12, Barakhamba Road Towers, New Delhi";
        s1_3.relationshipToVictim = "Personal legal counsel and executor of the will for 15 years";
        s1_3.lastKnownLocation = "Conference call logged from his central Delhi law chambers";
        s1_3.alibi = "Attended continuous arbitration drafting session with two junior advocates until 00:30";
        s1_3.alibiVerified = true;
        s1_3.motive = "Victim threatened to disbar him over undisclosed conflicts of interest";
        s1_3.opportunity = "Held draft copies of new testament; had no known presence on site during crime window";
        s1_3.investigationStatus = "Under Surveillance";
        s1_3.evidenceRelevanceScore = 52.5;
        s1_3.presenceInTimeWindow = false;
        s1_3.accessToMeansOrWeapon = false;
        s1_3.forensicTraceMatched = false;
        s1_3.alibiInvalidated = false;
        s1_3.substantiatedMotive = true;
        s1_3.directSceneTrace = false;
        s1_3.associatedEvidenceIds = {"EVD-05"};
        c1.suspects.push_back(s1_3);

        // Suspect 4
        Suspect s1_4;
        s1_4.id = "SUSP-04";
        s1_4.fullName = "Sunita Rawat";
        s1_4.age = 38;
        s1_4.occupation = "Head Housekeeper";
        s1_4.address = "Servants Quarters B, Singhania Bungalow, New Delhi";
        s1_4.relationshipToVictim = "Domestic head of staff for 7 years";
        s1_4.lastKnownLocation = "Main pantry and ground floor corridor at 23:10";
        s1_4.alibi = "Claims she retired to staff annex and slept through the night from 23:20 onward";
        s1_4.alibiVerified = false;
        s1_4.motive = "Victim discovered ongoing theft of antique silverware and scheduled immediate dismissal";
        s1_4.opportunity = "Possessed keys to liquor cabinet decanter; unrestrained movement in hallway";
        s1_4.investigationStatus = "Person of Interest";
        s1_4.evidenceRelevanceScore = 62.0;
        s1_4.presenceInTimeWindow = true;
        s1_4.accessToMeansOrWeapon = false; // Lacks strength for the specific lethal cranial fracture weapon
        s1_4.forensicTraceMatched = false;
        s1_4.alibiInvalidated = true; // Night watchman saw her sneaking near pantry door at 23:50
        s1_4.substantiatedMotive = false;
        s1_4.directSceneTrace = true; // Decanter glass handled by her earlier
        s1_4.associatedEvidenceIds = {"EVD-02"};
        c1.suspects.push_back(s1_4);

        // Suspect 5
        Suspect s1_5;
        s1_5.id = "SUSP-05";
        s1_5.fullName = "Kabir Oberoi";
        s1_5.age = 29;
        s1_5.occupation = "Security Electronics Systems Contractor";
        s1_5.address = "A-18, Lajpat Nagar III, New Delhi";
        s1_5.relationshipToVictim = "Contracted technician managing private IP surveillance";
        s1_5.lastKnownLocation = "IP connection trace from residential subnet at 23:47";
        s1_5.alibi = "Claims he was gaming online with friends at his Lajpat Nagar residence";
        s1_5.alibiVerified = false;
        s1_5.motive = "Paid ₹5,00,000 cash bounty to inject looping feed and mask perimeter intrusion";
        s1_5.opportunity = "Remote root credentials to video server; disabled feeds between 23:45 and 00:15";
        s1_5.investigationStatus = "Suspected Accomplice";
        s1_5.evidenceRelevanceScore = 77.0;
        s1_5.presenceInTimeWindow = false; // Physical presence not established; digital accomplice role
        s1_5.accessToMeansOrWeapon = false;
        s1_5.forensicTraceMatched = true; // IP server log tied to his encrypted tunnel
        s1_5.alibiInvalidated = true;
        s1_5.substantiatedMotive = true;
        s1_5.directSceneTrace = false;
        s1_5.associatedEvidenceIds = {"EVD-03", "EVD-06"};
        c1.suspects.push_back(s1_5);

        // Evidence Records (8 records)
        Evidence e1;
        e1.id = "EVD-01";
        e1.name = "Antique Chola Brass Statuette (Lethal Instrument)";
        e1.category = EvidenceCategory::PHYSICAL;
        e1.description = "Heavy 4.2 kg solid bronze figurine recovered from courtyard lily pond. Cranial impact ridges match victim injuries. Blood group O+ confirmed.";
        e1.collectedLocation = "Courtyard decorative lily pond, 18 meters from study French window";
        e1.collectionDateTime = "2026-10-15 01:15 IST";
        e1.linkedSuspectId = "SUSP-01";
        e1.linkedSuspectName = "Dev Singhania";
        e1.reliabilityScore = 98.0;
        e1.status = "Forensic Lab Verified";
        e1.chainOfCustody = "Recovered by Sub-Inspector V. Rao; sealed in evidence locker B-4";
        e1.forensicNotes = "Latent partial palm impression on base consistent with Dev Singhania grip anatomy.";
        c1.evidenceRecords.push_back(e1);

        Evidence e2;
        e2.id = "EVD-02";
        e2.name = "Shattered Crystal Brandy Decanter & Residue";
        e2.category = EvidenceCategory::PHYSICAL;
        e2.description = "Lead crystal decanter fragments on study rug. Chemical mass spectrometry detected 15 mg of Alprazolam (sedative) dissolved in remaining liquor.";
        e2.collectedLocation = "Persian rug adjacent to victim desk, Study Room";
        e2.collectionDateTime = "2026-10-15 01:40 IST";
        e2.linkedSuspectId = "SUSP-04";
        e2.linkedSuspectName = "Sunita Rawat";
        e2.reliabilityScore = 88.5;
        e2.status = "Lab Tested";
        e2.chainOfCustody = "Forensic Science Laboratory (FSL) Rohini - Toxicology Division";
        e2.forensicNotes = "Explains why victim suffered no defensive wounds prior to cranial strike.";
        c1.evidenceRecords.push_back(e2);

        Evidence e3;
        e3.id = "EVD-03";
        e3.name = "CCTV Server Tamper Log & Video Loop Injection";
        e3.category = EvidenceCategory::DIGITAL;
        e3.description = "Network DVR security log exhibiting unauthorized SSH root login at 23:47 IST from IP 192.168.1.105, which replaced live hallway cameras with static footage.";
        e3.collectedLocation = "Basement Server Utility Rack, Bungalow 14-B";
        e3.collectionDateTime = "2026-10-15 02:10 IST";
        e3.linkedSuspectId = "SUSP-05";
        e3.linkedSuspectName = "Kabir Oberoi";
        e3.reliabilityScore = 95.0;
        e3.status = "Corroborated";
        e3.chainOfCustody = "Cyber Cell Forensic Unit, Mandir Marg";
        e3.forensicNotes = "Cryptographic key 'koberoi_rsa' authenticated session.";
        c1.evidenceRecords.push_back(e3);

        Evidence e4;
        e4.id = "EVD-04";
        e4.name = "Torn Navy Cashmere Overcoat Fiber Snag";
        e4.category = EvidenceCategory::PHYSICAL;
        e4.description = "Three woven strands of premium Mongolian navy cashmere snagged on the exterior brass latch of the study French window.";
        e4.collectedLocation = "Study French window exterior latch facing West Garden";
        e4.collectionDateTime = "2026-10-15 02:30 IST";
        e4.linkedSuspectId = "SUSP-01";
        e4.linkedSuspectName = "Dev Singhania";
        e4.reliabilityScore = 92.0;
        e4.status = "Forensic Lab Verified";
        e4.chainOfCustody = "Central Forensic Science Laboratory (CFSL)";
        e4.forensicNotes = "Spectroscopic fiber analysis matched bespoke trench coat seized from Dev Singhania residence.";
        c1.evidenceRecords.push_back(e4);

        Evidence e5;
        e5.id = "EVD-05";
        e5.name = "Charred Will Codicil Draft in Fireplace";
        e5.category = EvidenceCategory::DOCUMENTARY;
        e5.description = "Partially burned legal parchment dated October 14, 2026, revoking all testamentary bequests to Dev Singhania and transferring corporate holdings to public trust.";
        e5.collectedLocation = "Study Room Italian Marble Fireplace grate";
        e5.collectionDateTime = "2026-10-15 02:50 IST";
        e5.linkedSuspectId = "SUSP-01";
        e5.linkedSuspectName = "Dev Singhania";
        e5.reliabilityScore = 96.0;
        e5.status = "In Custody";
        e5.chainOfCustody = "Investigating Officer A. K. Shrivastava";
        e5.forensicNotes = "Signed by Vikramaditya Singhania; ink still damp when discarded.";
        c1.evidenceRecords.push_back(e5);

        Evidence e6;
        e6.id = "EVD-06";
        e6.name = "Encrypted Wire Transfer & Account Discrepancy Ledger";
        e6.category = EvidenceCategory::DIGITAL;
        e6.description = "Banking SWIFT audit trails detailing ₹25,00,000 diverted from foundation accounts to an offshore shell firm in Port Louis.";
        e6.collectedLocation = "Financial Intelligence Gateway & Foundation Cloud Server";
        e6.collectionDateTime = "2026-10-15 03:20 IST";
        e6.linkedSuspectId = "SUSP-02";
        e6.linkedSuspectName = "Meera Sen";
        e6.reliabilityScore = 89.0;
        e6.status = "Corroborated";
        e6.chainOfCustody = "Economic Offences Wing (EOW) liaison";
        e6.forensicNotes = "Shows motive for dispute between victim and Meera Sen.";
        c1.evidenceRecords.push_back(e6);

        Evidence e7;
        e7.id = "EVD-07";
        e7.name = "Muddy Goodyear Tire Tread Casting";
        e7.category = EvidenceCategory::PHYSICAL;
        e7.description = "Plaster cast of 225/55 R18 directional tire tread imprint depressed into moist lawn loam near secondary estate egress gate.";
        e7.collectedLocation = "Service gate driveway, South boundary wall";
        e7.collectionDateTime = "2026-10-15 03:45 IST";
        e7.linkedSuspectId = "SUSP-01";
        e7.linkedSuspectName = "Dev Singhania";
        e7.reliabilityScore = 91.0;
        e7.status = "Lab Tested";
        e7.chainOfCustody = "Ballistics & Toolmark Division, Delhi Police";
        e7.forensicNotes = "Matches right rear wheel wear pattern of Dev Singhania's dark grey sedan.";
        c1.evidenceRecords.push_back(e7);

        Evidence e8;
        e8.id = "EVD-08";
        e8.name = "Olympus Digital Voice Dictaphone Recording";
        e8.category = EvidenceCategory::DIGITAL;
        e8.description = "Voice-activated recorder on study bookshelf capturing evening conversations. At 23:15 IST, records victim shouting: 'You will never see a single rupee of this inheritance, Dev!' followed by glass breaking.";
        e8.collectedLocation = "Mahogany bookshelf, Study Room";
        e8.collectionDateTime = "2026-10-15 04:10 IST";
        e8.linkedSuspectId = "SUSP-01";
        e8.linkedSuspectName = "Dev Singhania";
        e8.reliabilityScore = 97.5;
        e8.status = "Forensic Lab Verified";
        e8.chainOfCustody = "Audio Forensic Analysis Lab";
        e8.forensicNotes = "Acoustic voiceprint frequency matches Dev Singhania vocal profile with 99.2% probability.";
        c1.evidenceRecords.push_back(e8);

        // Witness Statements (4 records)
        WitnessStatement w1;
        w1.id = "WIT-01";
        w1.witnessName = "Harish Chandra";
        w1.occupation = "Night Gatekeeper (Post B)";
        w1.relationshipToCase = "On duty at the perimeter gate from 20:00 to 08:00";
        w1.statementDateTime = "2026-10-15 05:30 IST";
        w1.statementText = "Around 23:35 I heard the perimeter sensor beep. I walked out and spotted a dark grey sedan idling without headlights near the rear service gate. A tall man in a long dark coat slipped past the hedge towards the study garden. Shortly after, around 23:52, I heard a faint sound of shattered glass followed by a dull thud. At 00:08, the sedan accelerated away heading towards the round-about.";
        w1.mentionedKeywords = {"dark grey sedan", "long dark coat", "shattered glass", "dull thud", "service gate"};
        w1.connectedSuspectId = "SUSP-01";
        w1.credibilityRating = 91.0;
        w1.interviewerBadge = "Badge #7412";
        c1.witnessStatements.push_back(w1);

        WitnessStatement w2;
        w2.id = "WIT-02";
        w2.witnessName = "Dr. Ananya Roy";
        w2.occupation = "Consultant Physician & Adjacent Resident";
        w2.relationshipToCase = "Neighbor residing at Bungalow 12-A";
        w2.statementDateTime = "2026-10-15 06:15 IST";
        w2.statementText = "I was reading on my terrace facing the Singhania garden. At roughly 23:40 I noticed unusual hurried movement near Vikramaditya's study terrace. A man wearing an expensive navy cashmere overcoat was pacing agitatedly. There was a brief raised voice shouting about disinheritance and forged documents, followed by sudden silence.";
        w2.mentionedKeywords = {"navy cashmere overcoat", "disinheritance", "forged documents", "sudden silence"};
        w2.connectedSuspectId = "SUSP-01";
        w2.credibilityRating = 94.0;
        w2.interviewerBadge = "Badge #7412";
        c1.witnessStatements.push_back(w2);

        WitnessStatement w3;
        w3.id = "WIT-03";
        w3.witnessName = "Arjun Verma";
        w3.occupation = "Personal Chauffeur";
        w3.relationshipToCase = "Employed by victim for 4 years";
        w3.statementDateTime = "2026-10-15 07:00 IST";
        w3.statementText = "I drove Mr. Singhania home from the golf club at 18:30. During the drive, he received an agitated call from Dev Singhania demanding an emergency meeting regarding the altered will. Sir told him: 'Come if you dare, but my signature is on the final codicil.' Later around 21:00, Meera Sen arrived to deliver auction reports.";
        w3.mentionedKeywords = {"agitated call", "altered will", "final codicil", "Meera Sen"};
        w3.connectedSuspectId = "SUSP-01";
        w3.credibilityRating = 87.0;
        w3.interviewerBadge = "Badge #8821";
        c1.witnessStatements.push_back(w3);

        WitnessStatement w4;
        w4.id = "WIT-04";
        w4.witnessName = "Pooja Saxena";
        w4.occupation = "Estate Financial Controller";
        w4.relationshipToCase = "Audited Singhania Trust quarterly accounts";
        w4.statementDateTime = "2026-10-15 08:30 IST";
        w4.statementText = "Three days ago, Mr. Singhania summoned me to review offshore disbursements. He discovered that Kabir Oberoi had been granted unexplained server access, and that Dev Singhania had taken predatory loans against anticipated inheritance. He was adamant on revoking Dev's status in the revised will drafted by Mr. Malhotra.";
        w4.mentionedKeywords = {"offshore disbursements", "Kabir Oberoi", "predatory loans", "revised will"};
        w4.connectedSuspectId = "SUSP-05";
        w4.credibilityRating = 90.0;
        w4.interviewerBadge = "Badge #8821";
        c1.witnessStatements.push_back(w4);

        // Investigation Clues (6 Clues for Backtracking Solver)
        InvestigationClue cl1;
        cl1.id = "CLUE-01";
        cl1.title = "Critical Timeline Presence";
        cl1.category = "TIMELINE";
        cl1.requiredCondition = "TIME_WINDOW_2330_0030";
        cl1.description = "The perpetrator must have had verifiable physical presence or proximity at Bungalow 14-B between 23:30 and 00:30 IST.";
        cl1.weight = 10;
        c1.clues.push_back(cl1);

        InvestigationClue cl2;
        cl2.id = "CLUE-02";
        cl2.title = "Access to Lethal Means / Instrument";
        cl2.category = "WEAPON_MEANS";
        cl2.requiredCondition = "ACCESS_TO_WEAPON";
        cl2.description = "Access to study interior and physical capability to wield the 4.2 kg bronze statuette to inflict fatal blunt-force trauma.";
        cl2.weight = 9;
        c1.clues.push_back(cl2);

        InvestigationClue cl3;
        cl3.id = "CLUE-03";
        cl3.title = "Forensic Biological / Physical Snag";
        cl3.category = "FORENSICS";
        cl3.requiredCondition = "FORENSIC_TRACE_MATCH";
        cl3.description = "Direct physical or forensic correlation (e.g. cashmere fibers on latch, blood typing, or weapon palm print).";
        cl3.weight = 9;
        c1.clues.push_back(cl3);

        InvestigationClue cl4;
        cl4.id = "CLUE-04";
        cl4.title = "Invalidated / Fabricated Alibi";
        cl4.category = "ALIBI";
        cl4.requiredCondition = "ALIBI_INVALIDATED";
        cl4.description = "Suspect alibi must be disproven, uncorroborated, or directly contradicted by CCTV, toll records, or witness statements.";
        cl4.weight = 8;
        c1.clues.push_back(cl4);

        InvestigationClue cl5;
        cl5.id = "CLUE-05";
        cl5.title = "Substantiated Financial / Personal Motive";
        cl5.category = "MOTIVE";
        cl5.requiredCondition = "SUBSTANTIATED_MOTIVE";
        cl5.description = "Documented proof of existential financial ruin, disinheritance, or criminal concealment resulting from the victim's death.";
        cl5.weight = 8;
        c1.clues.push_back(cl5);

        InvestigationClue cl6;
        cl6.id = "CLUE-06";
        cl6.title = "Direct Scene / Acoustic Trace";
        cl6.category = "DIRECT_CONTACT";
        cl6.requiredCondition = "DIRECT_SCENE_TRACE";
        cl6.description = "Physical vehicle tire impression, audio voiceprint identification, or personal fingerprint at the crime scene.";
        cl6.weight = 9;
        c1.clues.push_back(cl6);

        // Relational Graph Edges (20+ relationships)
        c1.relationships.push_back(GraphEdge{"SUSP-01", "SUSP-05", "CO_CONSPIRATOR_PAYOFF", 0.75, "Dev paid Kabir for camera blackout"});
        c1.relationships.push_back(GraphEdge{"SUSP-01", "SUSP-03", "LEGAL_THREAT", 0.70, "Dev pressured Malhotra to hide revised will"});
        c1.relationships.push_back(GraphEdge{"SUSP-02", "SUSP-03", "AUDIT_ADVISORY", 0.60, "Meera consulted Malhotra regarding missing art"});
        c1.relationships.push_back(GraphEdge{"SUSP-04", "SUSP-01", "HOUSE_KEY_HANDOVER", 0.65, "Sunita left garden French latch unlocked"});
        c1.relationships.push_back(GraphEdge{"WIT-01", "SUSP-01", "EYEWITNESS_VEHICLE", 0.90, "Watchman saw Dev's sedan near South Gate"});
        c1.relationships.push_back(GraphEdge{"WIT-02", "SUSP-01", "ACOUSTIC_CONFIRMATION", 0.92, "Neighbor heard Dev's voice and saw cashmere coat"});

        cases.push_back(c1);

        // =====================================================================
        // CASE 2: THE CYPHER HEIST AT CONNAUGHT PLACE
        // =====================================================================
        CaseFile c2;
        c2.caseId = "CASE-DEL-2026-112";
        c2.caseTitle = "THE CYPHER HEIST AT CONNAUGHT PLACE";
        c2.crimeType = CrimeType::CYBER_HEIST;
        c2.dateTime = "2026-10-22 02:15 IST";
        c2.crimeLocation = "Zenith Bullion & Cyber Repository, Regal Building, Connaught Place, New Delhi";
        c2.caseDescription = "Sophisticated physical and cyber penetration into underground Grade-1 safe vault. ₹45 Crore worth of bearer bonds and physical gold bars extracted. The electronic vault time-lock mechanism was bypassed using a cloned cryptographic hardware token, while electromagnetic security sensors were disabled via insider credentials.";
        c2.status = CaseStatus::EVIDENCE_ANALYSIS;
        c2.assignedInvestigator = "Senior Superintendent R. N. Saxena (Cyber Crime Command)";
        c2.victimName = "Zenith Bullion Commercial Vault Corporation";
        c2.victimDetails = "Private custodial bullion vault housing corporate and private investor safe deposit reserves.";

        // Suspects for Case 2
        Suspect s2_1;
        s2_1.id = "SUSP-201";
        s2_1.fullName = "Aakash 'Zero' Mehta";
        s2_1.age = 31;
        s2_1.occupation = "Lead Cryptographic Security Auditor";
        s2_1.address = "Tower 4, Greater Kailash II, New Delhi";
        s2_1.relationshipToVictim = "Subcontracted firm lead responsible for penetration testing";
        s2_1.lastKnownLocation = "Cellular ping placed him at Shivaji Stadium metro station (300m from vault) at 01:50 IST";
        s2_1.alibi = "Claims he was working remotely from his GK-II home office compiling audit logs";
        s2_1.alibiVerified = false;
        s2_1.motive = "Facing bankruptcy from illicit cryptocurrency margin liquidations of ₹8 Crore";
        s2_1.opportunity = "Architect of the multi-signature hardware token protocol; possessed root derivation seed";
        s2_1.investigationStatus = "Prime Suspect";
        s2_1.evidenceRelevanceScore = 93.0;
        s2_1.presenceInTimeWindow = true;
        s2_1.accessToMeansOrWeapon = true;
        s2_1.forensicTraceMatched = true; // Raspberry Pi zero with his custom kernel compiled tag found in vent
        s2_1.alibiInvalidated = true;
        s2_1.substantiatedMotive = true;
        s2_1.directSceneTrace = true;
        s2_1.associatedEvidenceIds = {"EVD-201", "EVD-203", "EVD-205"};
        c2.suspects.push_back(s2_1);

        Suspect s2_2;
        s2_2.id = "SUSP-202";
        s2_2.fullName = "Rohini Deshmukh";
        s2_2.age = 39;
        s2_2.occupation = "Chief Vault Custodian";
        s2_2.address = "Flat 501, Mayur Vihar Phase I, Delhi";
        s2_2.relationshipToVictim = "12-year full-time employee holding primary vault physical key";
        s2_2.lastKnownLocation = "At home with family; logged emergency pager at 03:10 IST";
        s2_2.alibi = "Verified at home; doorbell camera and spouse statement confirm whereabouts";
        s2_2.alibiVerified = true;
        s2_2.motive = "Denied salary hike and equity share in company during recent board meeting";
        s2_2.opportunity = "Knowledge of physical vault layout and maintenance access ducts";
        s2_2.investigationStatus = "Cleared";
        s2_2.evidenceRelevanceScore = 41.0;
        s2_2.presenceInTimeWindow = false;
        s2_2.accessToMeansOrWeapon = true;
        s2_2.forensicTraceMatched = false;
        s2_2.alibiInvalidated = false;
        s2_2.substantiatedMotive = false;
        s2_2.directSceneTrace = false;
        s2_2.associatedEvidenceIds = {"EVD-204"};
        c2.suspects.push_back(s2_2);

        Suspect s2_3;
        s2_3.id = "SUSP-203";
        s2_3.fullName = "Sameer 'Viper' Khan";
        s2_3.age = 35;
        s2_3.occupation = "Specialized Safecracker & Logistics Specialist";
        s2_3.address = "Near Jama Masjid, Old Delhi";
        s2_3.relationshipToVictim = "No legitimate connection; known freelance logistics operator";
        s2_3.lastKnownLocation = "Spotted loading heavy duffel bags into white transit van at outer circle Connaught Place";
        s2_3.alibi = "Refused to account for whereabouts between midnight and 03:00 AM";
        s2_3.alibiVerified = false;
        s2_3.motive = "Hired for 15% cut of physical bullion haul";
        s2_3.opportunity = "Physical access via ventilation shaft using industrial suction gear";
        s2_3.investigationStatus = "Suspected Accomplice";
        s2_3.evidenceRelevanceScore = 86.5;
        s2_3.presenceInTimeWindow = true;
        s2_3.accessToMeansOrWeapon = true;
        s2_3.forensicTraceMatched = true; // Glove leather residue on vault door hinge
        s2_3.alibiInvalidated = true;
        s2_3.substantiatedMotive = true;
        s2_3.directSceneTrace = true;
        s2_3.associatedEvidenceIds = {"EVD-202", "EVD-206"};
        c2.suspects.push_back(s2_3);

        // Evidence for Case 2
        Evidence e2_1;
        e2_1.id = "EVD-201";
        e2_1.name = "Cloned YubiKey Hardware Token with Custom Firmware";
        e2_1.category = EvidenceCategory::DIGITAL;
        e2_1.description = "Cryptographic token plugged into maintenance port of Vault Controller 02, flashed with Aakash Mehta's private test key signature.";
        e2_1.collectedLocation = "Vault Maintenance Terminal Rack, B-2 Level";
        e2_1.collectionDateTime = "2026-10-22 04:30 IST";
        e2_1.linkedSuspectId = "SUSP-201";
        e2_1.linkedSuspectName = "Aakash 'Zero' Mehta";
        e2_1.reliabilityScore = 97.0;
        e2_1.status = "Forensic Lab Verified";
        e2_1.chainOfCustody = "Cyber Cell Forensic Unit";
        e2_1.forensicNotes = "Binary matches compilation hash found on Mehta's seized laptop.";
        c2.evidenceRecords.push_back(e2_1);

        Evidence e2_2;
        e2_2.id = "EVD-202";
        e2_2.name = "Hydraulic Micro-Spreader & Diamond Core Drill Bit";
        e2_2.category = EvidenceCategory::PHYSICAL;
        e2_2.description = "Heavy German-made hydraulic extraction tools left behind in service hallway.";
        e2_2.collectedLocation = "Air conditioning exhaust corridor behind vault door";
        e2_2.collectionDateTime = "2026-10-22 05:00 IST";
        e2_2.linkedSuspectId = "SUSP-203";
        e2_2.linkedSuspectName = "Sameer 'Viper' Khan";
        e2_2.reliabilityScore = 93.0;
        e2_2.status = "Lab Tested";
        e2_2.chainOfCustody = "Crime Scene Investigation Unit";
        e2_2.forensicNotes = "Fingerprints on tool handle correspond to Sameer Khan.";
        c2.evidenceRecords.push_back(e2_2);

        // Witness for Case 2
        WitnessStatement w2_1;
        w2_1.id = "WIT-201";
        w2_1.witnessName = "Ramesh Chand";
        w2_1.occupation = "CP Roundabout Night Patrol Officer";
        w2_1.relationshipToCase = "Delhi Police Night Beat Patrol";
        w2_1.statementDateTime = "2026-10-22 06:00 IST";
        w2_1.statementText = "Around 02:25 AM, I noticed an unmarked white transit van parked with hazards near Regal Cinema alley. A thin man with spectacles carrying a laptop case hurried out from the basement fire escape and got into the passenger seat, shouting 'The hardware bypass worked, drive now!'";
        w2_1.mentionedKeywords = {"white transit van", "laptop case", "hardware bypass", "fire escape"};
        w2_1.connectedSuspectId = "SUSP-201";
        w2_1.credibilityRating = 92.0;
        w2_1.interviewerBadge = "Badge #4412";
        c2.witnessStatements.push_back(w2_1);

        // Clues for Case 2
        InvestigationClue c2_cl1;
        c2_cl1.id = "CLUE-201";
        c2_cl1.title = "Cyber Token Hardware Access";
        c2_cl1.category = "WEAPON_MEANS";
        c2_cl1.requiredCondition = "TECHNICAL_SECURITY_ACCESS";
        c2_cl1.description = "Possession of the root elliptic curve master keys to craft cloned bypass token.";
        c2_cl1.weight = 10;
        c2.clues.push_back(c2_cl1);

        InvestigationClue c2_cl2;
        c2_cl2.id = "CLUE-202";
        c2_cl2.title = "Physical Presence at CP Perimeter";
        c2_cl2.category = "TIMELINE";
        c2_cl2.requiredCondition = "TIME_WINDOW_OPPORTUNITY";
        c2_cl2.description = "Cellular ping or surveillance sighting in Connaught Place between 01:30 and 02:45 AM.";
        c2_cl2.weight = 9;
        c2.clues.push_back(c2_cl2);

        c2.relationships.push_back(GraphEdge{"SUSP-201", "SUSP-203", "HEIST_HIRE_CONTRACT", 0.90, "Mehta recruited Khan for physical vault egress"});

        cases.push_back(c2);

        // =====================================================================
        // CASE 3: THE CYANIDE VINTAGE AT MEHRAULI MANOR
        // =====================================================================
        CaseFile c3;
        c3.caseId = "CASE-DEL-2026-145";
        c3.caseTitle = "THE CYANIDE VINTAGE AT MEHRAULI MANOR";
        c3.crimeType = CrimeType::POISONING_CONSPIRACY;
        c3.dateTime = "2026-11-04 21:30 IST";
        c3.crimeLocation = "The Heritage Pavilions, Mehrauli Archeological Ridge, South Delhi";
        c3.caseDescription = "High-society poisoning during a private charity auction for endangered heritage monuments. Prominent venture capitalist Jaideep Singhal (58) collapsed within 90 seconds after drinking a glass of 1982 Château Margaux. Post-mortem confirms lethal dose of potassium cyanide introduced into the specific auction bottle.";
        c3.status = CaseStatus::FORENSIC_TRIAGE;
        c3.assignedInvestigator = "Deputy Commissioner of Police Neha Kapoor";
        c3.victimName = "Jaideep Singhal";
        c3.victimDetails = "Age 58, Managing Partner at Apex Global Ventures. Collapsed on central banquet stage.";

        // Suspects for Case 3
        Suspect s3_1;
        s3_1.id = "SUSP-301";
        s3_1.fullName = "Dr. Shivani Kashyap";
        s3_1.age = 44;
        s3_1.occupation = "Biochemical Researcher & Ex-Board Member";
        s3_1.address = "Villa 7, Mehrauli Farms, New Delhi";
        s3_1.relationshipToVictim = "Former partner in biotech venture ousted by victim two weeks prior";
        s3_1.lastKnownLocation = "Observed hovering near sommelier table at 21:10 IST";
        s3_1.alibi = "Claims she was in the powder room talking on the phone with her daughter";
        s3_1.alibiVerified = false;
        s3_1.motive = "Victim liquidated her 40% equity stake, bankrupting her laboratory patents";
        s3_1.opportunity = "Possessed certified laboratory access to chemical reagents including analytical-grade potassium cyanide";
        s3_1.investigationStatus = "Prime Suspect";
        s3_1.evidenceRelevanceScore = 95.0;
        s3_1.presenceInTimeWindow = true;
        s3_1.accessToMeansOrWeapon = true; // High: laboratory clearance
        s3_1.forensicTraceMatched = true;  // Chemical traces in her clutch purse lining
        s3_1.alibiInvalidated = true;      // Powder room attendant testified she was absent
        s3_1.substantiatedMotive = true;
        s3_1.directSceneTrace = true;
        s3_1.associatedEvidenceIds = {"EVD-301", "EVD-302", "EVD-303"};
        c3.suspects.push_back(s3_1);

        Suspect s3_2;
        s3_2.id = "SUSP-302";
        s3_2.fullName = "Laurent Dubois";
        s3_2.age = 36;
        s3_2.occupation = "Head Guest Sommelier";
        s3_2.address = "Room 304, The Lodhi Hotel, New Delhi";
        s3_2.relationshipToVictim = "Contracted exclusively for the wine service";
        s3_2.lastKnownLocation = "Serving wine at main banquet counter";
        s3_2.alibi = "Constantly in full view of 80 guests behind wine bar";
        s3_2.alibiVerified = true;
        s3_2.motive = "None identified";
        s3_2.opportunity = "Uncorked and poured the vintage bottle";
        s3_2.investigationStatus = "Witness / Cleared";
        s3_2.evidenceRelevanceScore = 35.0;
        s3_2.presenceInTimeWindow = true;
        s3_2.accessToMeansOrWeapon = false; // Had no access to cyanide supplies
        s3_2.forensicTraceMatched = false;
        s3_2.alibiInvalidated = false;
        s3_2.substantiatedMotive = false;
        s3_2.directSceneTrace = true;
        s3_2.associatedEvidenceIds = {"EVD-302"};
        c3.suspects.push_back(s3_2);

        // Evidence for Case 3
        Evidence e3_1;
        e3_1.id = "EVD-301";
        e3_1.name = "Hypodermic Micro-Needle & Cork Puncture Mark";
        e3_1.category = EvidenceCategory::PHYSICAL;
        e3_1.description = "Microscopic 0.3mm puncture through lead foil capsule and cork of the 1982 Château Margaux bottle used to inject dissolved cyanide solution.";
        e3_1.collectedLocation = "VIP Auction Table Display stand";
        e3_1.collectionDateTime = "2026-11-04 22:15 IST";
        e3_1.linkedSuspectId = "SUSP-301";
        e3_1.linkedSuspectName = "Dr. Shivani Kashyap";
        e3_1.reliabilityScore = 96.5;
        e3_1.status = "Forensic Lab Verified";
        e3_1.chainOfCustody = "Forensic Science Laboratory (FSL)";
        e3_1.forensicNotes = "Hypodermic needle of identical gauge found discarded in garden planter.";
        c3.evidenceRecords.push_back(e3_1);

        Evidence e3_2;
        e3_2.id = "EVD-302";
        e3_2.name = "Cyanide Micro-Crystalline Residue in Evening Clutch";
        e3_2.category = EvidenceCategory::PHYSICAL;
        e3_2.description = "Microgram quantities of potassium cyanide salt detected along the inner velvet seam of Dr. Kashyap's black evening clutch.";
        e3_2.collectedLocation = "Cloakroom inspection, Mehrauli Pavilion";
        e3_2.collectionDateTime = "2026-11-04 23:00 IST";
        e3_2.linkedSuspectId = "SUSP-301";
        e3_2.linkedSuspectName = "Dr. Shivani Kashyap";
        e3_2.reliabilityScore = 98.0;
        e3_2.status = "Forensic Lab Verified";
        e3_2.chainOfCustody = "Crime Branch Forensic Team";
        e3_2.forensicNotes = "Spectroscopic chemical signature exactly matches laboratory stock batch #KCN-994.";
        c3.evidenceRecords.push_back(e3_2);

        // Witness for Case 3
        WitnessStatement w3_1;
        w3_1.id = "WIT-301";
        w3_1.witnessName = "Karan Bahl";
        w3_1.occupation = "Charity Auctioneer";
        w3_1.relationshipToCase = "Host on stage during the incident";
        w3_1.statementDateTime = "2026-11-04 23:45 IST";
        w3_1.statementText = "Just before Lot #4 was presented, Dr. Kashyap walked past the wine decanting station. She reached into her evening clutch and paused beside the bottle while Laurent Dubois was lighting candles. Seconds after Mr. Singhal toasted the winning bid, he gasped for air and collapsed on stage.";
        w3_1.mentionedKeywords = {"wine decanting station", "evening clutch", "gasped for air", "cyanide"};
        w3_1.connectedSuspectId = "SUSP-301";
        w3_1.credibilityRating = 93.0;
        w3_1.interviewerBadge = "Badge #6102";
        c3.witnessStatements.push_back(w3_1);

        // Clues for Case 3
        InvestigationClue c3_cl1;
        c3_cl1.id = "CLUE-301";
        c3_cl1.title = "Access to Hazardous Toxins";
        c3_cl1.category = "WEAPON_MEANS";
        c3_cl1.requiredCondition = "ACCESS_TO_WEAPON";
        c3_cl1.description = "Access to pure potassium cyanide stocks and chemical syringe delivery mechanism.";
        c3_cl1.weight = 10;
        c3.clues.push_back(c3_cl1);

        cases.push_back(c3);

        return cases;
    }
};

} // namespace Culprit

# WHO IS THE CULPRIT?
## Professional Crime Investigation & Algorithmic Forensic Analysis Platform

A high-performance C++17 forensic investigation case-management and simulation system paired with a dark-charcoal forensic intelligence dashboard. Built strictly following algorithmic computer science principles, with **real C++ Data Structures & Algorithms (DSA)** driving all suspect prioritization, clue constraint satisfaction, relationship discovery, and witness deposition parsing.

---

## 1. System Architecture

```
WHO IS THE CULPRIT?
├── src/
│   ├── core/
│   │   ├── Models.hpp              # Domain entities: CaseFile, Suspect, Evidence, WitnessStatement, Clue, Graph
│   │   ├── LinearSearch.hpp        # O(N) sequential scan engine with element comparison logging
│   │   ├── MergeSort.hpp           # O(N log N) divide-and-conquer suspect evidence ranking
│   │   ├── Graph.hpp               # Adjacency list graph, BFS radial discovery, DFS deep path exploration
│   │   ├── Backtracking.hpp        # Constraint Satisfaction (CSP) clue matching & branch pruning
│   │   ├── KMP.hpp                 # Knuth-Morris-Pratt O(N+M) string matcher with Pi/LPS prefix table
│   │   ├── CaseData.hpp            # 3 preloaded fictional cases including 'The Midnight Murder'
│   │   └── InvestigationEngine.hpp # Dynamic composite score calculator & coordinator
│   ├── server/
│   │   ├── JsonHelper.hpp          # Lightweight C++17 JSON serialization & parsing
│   │   ├── HttpServer.hpp          # Multi-threaded POSIX socket HTTP REST server
│   │   └── HttpServer.cpp          # Request routing, REST API controllers, static asset server
│   ├── tests/
│   │   └── TestRunner.cpp          # Automated C++ test suite covering all 7 algorithms
│   └── main.cpp                    # CLI entrypoint (--serve, --test, --case, --help)
├── web/
│   ├── index.html                  # Forensic Case Management Command Center
│   ├── css/style.css               # Midnight navy & charcoal theme, police dossier styling, print styles
│   └── js/
│       ├── app.js                  # Frontend controller & REST API client
│       ├── graph.js                # HTML5 Canvas force-directed topological network engine
│       └── algorithms.js           # Interactive DSA trace & state machine visualizer
├── Makefile                        # Build automation for clang++ / g++
└── README.md                       # Complete technical & architectural documentation
```

---

## 2. Implemented Algorithms & Time Complexities

| Algorithm | Role in System | Time Complexity (Best / Worst) | Space Complexity |
|---|---|---|---|
| **Linear Search** | Query suspects & evidence records across name, category, status, and IDs | $O(1) \text{ / } O(N)$ | $O(1)$ |
| **Merge Sort** | Recursively partitions suspects by composite evidence relevance score | $O(N \log N) \text{ / } O(N \log N)$ | $O(N)$ |
| **Graph / Adjacency List** | Models relationships between Victims, Suspects, Witnesses, Evidence, and Scenes | $O(V + E)$ storage | $O(V + E)$ |
| **Breadth-First Search (BFS)** | Radial level-by-level discovery; computes degrees of separation from victim | $O(V + E)$ | $O(V)$ |
| **Depth-First Search (DFS)** | Probes deep indirect chains, clandestine meetings, and accessory links | $O(V + E)$ | $O(V)$ recursion stack |
| **Backtracking (CSP)** | Tests hypotheses against case clues; prunes branches violating constraints | $O(N \cdot C_{\text{avg}}) \text{ / } O(N \cdot 2^C)$ | $O(C)$ recursion stack |
| **KMP String Matching** | Precomputes $\pi$ (LPS) table; scans witness transcripts without character rollbacks | $O(N + M)$ | $O(M)$ for LPS table |

### Composite Evidence Scoring Formula
The C++ engine dynamically computes the composite evidence score for each suspect using weighted forensic reliability:
$$\text{Score}(s) = \frac{\sum_{e \in E_s} (\text{Reliability}_e \times w_{\text{cat}(e)})}{|E_s|} + \sum_{w \in W_s} (0.15 \times \text{Credibility}_w) + \Delta_{\text{alibi}} + \Delta_{\text{opportunity}}$$
- **Physical Evidence:** $w = 1.0$
- **Digital Evidence:** $w = 0.90$
- **Documentary Evidence:** $w = 0.85$
- **Witness Evidence:** $w = 0.75$
- **Circumstantial Evidence:** $w = 0.50$
- **Alibi Invalidated Penalty:** $+12.0$ | **Alibi Verified Relief:** $-25.0$

---

## 3. Preloaded Case Investigations

### Case 1: THE MIDNIGHT MURDER (Primary Case)
- **Case ID:** `CASE-DEL-2026-089`
- **Location:** Bungalow 14-B, Amrita Shergill Marg, New Delhi
- **Victim:** Vikramaditya Singhania (54, Industrialist & Art Patron)
- **5 Suspects:**
  1. *Dev Singhania* (32, Estranged Son & Heir) - Disinherited, debt-ridden, sedan spotted at perimeter.
  2. *Meera Sen* (41, Art Curator) - Disputed inventory, verified alibi in Defence Colony.
  3. *Rajeshwar Malhotra* (48, Senior Advocate) - Discovered secret will codicil, verified drafting alibi.
  4. *Sunita Rawat* (38, Head Housekeeper) - Tampered with pantry decanter, broken alibi.
  5. *Kabir Oberoi* (29, Security Contractor) - Injected CCTV video loop at 23:47 IST.
- **8 Evidence Records:** Antique Brass Statuette (4.2 kg murder weapon with palm print), Decanter Alprazolam residue, CCTV loop log, Torn cashmere fiber snag, Charred will in fireplace, SWIFT audit trail, Goodyear tire impression, Audio Dictaphone argument recording.
- **4 Witness Statements:** Harish Chandra (Night watchman), Dr. Ananya Roy (Neighbor), Arjun Verma (Chauffeur), Pooja Saxena (Accountant).
- **Algorithmic Result:** Backtracking CSP solver tests all 5 suspects against the 6 empirical clues and identifies **Dev Singhania** as strictly satisfying all conditions.

### Case 2: THE CYPHER HEIST AT CONNAUGHT PLACE
- **Case ID:** `CASE-DEL-2026-112`
- **Location:** Zenith Bullion Commercial Vault, Regal Building, Connaught Place, New Delhi
- **Crime:** ₹45 Crore bearer bonds and bullion vault breach via cloned cryptographic tokens.
- **Algorithmic Result:** Identifies multiple matching accomplices (**Aakash 'Zero' Mehta** and **Sameer 'Viper' Khan**).

### Case 3: THE CYANIDE VINTAGE AT MEHRAULI MANOR
- **Case ID:** `CASE-DEL-2026-145`
- **Location:** The Heritage Pavilions, Mehrauli Archeological Ridge, South Delhi
- **Crime:** Potassium cyanide fatal poisoning via vintage 1982 Château Margaux during a private charity auction.
- **Algorithmic Result:** Identifies **Dr. Shivani Kashyap** based on biochemical reagent clearance and clutch seam residue.

---

## 4. Build & Run Instructions

### Prerequisites
- macOS or Linux
- Clang++ or G++ supporting **C++17** (`clang++ -std=c++17` or `g++ -std=c++17`)
- Make

### Compilation
```bash
# Build the target binary (culprit_engine)
make

# Run the automated DSA test suite (7 tests)
./culprit_engine --test
# or: make test
```

### Running the Investigation Server
```bash
# Start the HTTP server on port 8080 (serves REST API + Forensic Web UI)
./culprit_engine --serve 8080
# or: make run
```
Open your web browser and navigate to:
```
http://127.0.0.1:8080
```

### Running CLI Case Analysis (Headless)
```bash
# Run forensic analysis on Case 1 and print dossier to terminal:
./culprit_engine --case CASE-DEL-2026-089

# Run forensic analysis on Case 2:
./culprit_engine --case CASE-DEL-2026-112

# Run forensic analysis on Case 3:
./culprit_engine --case CASE-DEL-2026-145
```

---

## 5. REST API Documentation

| Endpoint | Method | Description |
|---|---|---|
| `/api/health` | `GET` | System heartbeat & C++ engine version status |
| `/api/cases` | `GET` | List all registered case summaries |
| `/api/cases/:id` | `GET` | Retrieve complete case file (Suspects, Evidence, Witnesses, Clues) |
| `/api/cases/:id/graph` | `GET` | Retrieve nodes and links for relationship graph |
| `/api/cases/:id/algorithms/search` | `POST` | Execute C++ Linear Search on suspects or evidence |
| `/api/cases/:id/algorithms/sort` | `POST` | Execute C++ Merge Sort suspect ranking |
| `/api/cases/:id/algorithms/bfs` | `POST` | Execute C++ Breadth-First Search from start node |
| `/api/cases/:id/algorithms/dfs` | `POST` | Execute C++ Depth-First Search from start node |
| `/api/cases/:id/algorithms/backtracking` | `POST` | Execute C++ Backtracking CSP clue elimination |
| `/api/cases/:id/algorithms/kmp` | `POST` | Execute C++ KMP string pattern search in witness text |
| `/api/cases/:id/solve` | `POST` | Execute full multi-algorithm forensic investigation pipeline |
| `/api/cases/:id/suspects` | `POST` | Register new suspect in active case |
| `/api/cases/:id/evidence` | `POST` | Register new evidence item under chain of custody |
| `/api/cases/:id/witnesses` | `POST` | File new sworn witness deposition |

---

## 6. Official Forensic Report & PDF Export

The platform features an official **Courtroom-Ready Forensic Dossier** view:
- Formatted with institutional police department crest, case identification stamps, and classified headers.
- Synthesizes executive case summaries, suspect ranking tables, clue satisfaction matrices, evidence logs, witness deposition transcripts, and graph topology metrics.
- Clicking **"Export PDF / Print Dossier"** activates dedicated `@media print` CSS rules, producing an official high-contrast white-paper police dossier.

---

## 7. Educational Compliance & Legal Disclaimer

> **LEGAL NOTICE & EDUCATIONAL SIMULATION ADVISORY:**
> This system is an educational computer science demonstration showcasing the practical application of Data Structures and Algorithms in investigative case management. All names, locations, evidence, and crime scenarios are 100% fictional. No artificial intelligence, machine learning, biometric facial recognition, or real-world criminal databases are used. Identification of matching candidates represents constraint satisfaction within the simulated model and does not constitute a judicial determination of criminal guilt.

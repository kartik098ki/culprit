/**
 * WHO IS THE CULPRIT? - Primary Frontend Application Controller
 * Connects Forensic Dashboard UI to the C++17 Algorithm Engine REST API
 */

class CulpritApplication {
  constructor() {
    this.currentCaseId = 'CASE-DEL-2026-089';
    this.cases = [];
    this.currentCaseData = null;
    this.analysisCache = null;

    this.init();
  }

  async init() {
    this.startClock();
    await this.loadCases();
    await this.switchCase(this.currentCaseId);
  }

  startClock() {
    const clockEl = document.getElementById('systemClock');
    const update = () => {
      const now = new Date();
      const timeStr = now.toTimeString().split(' ')[0] + ' IST';
      if (clockEl) clockEl.innerText = timeStr;
    };
    update();
    setInterval(update, 1000);
  }

  // ========================================================================
  // API CLIENT METHODS WITH AUTOMATIC SERVERLESS / CLIENT-ENGINE FALLBACK
  // ========================================================================
  async apiGet(path) {
    try {
      const res = await fetch(path);
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      return await res.json();
    } catch (err) {
      console.warn(`[API] Remote call to ${path} failed or running in serverless mode. Falling back to local engine:`, err.message);
      return this.localFallback(path, 'GET');
    }
  }

  async apiPost(path, payload = {}) {
    try {
      const res = await fetch(path, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(payload)
      });
      if (!res.ok) throw new Error(`HTTP ${res.status}`);
      return await res.json();
    } catch (err) {
      console.warn(`[API] Remote call to ${path} failed or running in serverless mode. Falling back to local engine:`, err.message);
      return this.localFallback(path, 'POST', payload);
    }
  }

  localFallback(path, method, payload = {}) {
    if (!window.localForensicEngine) return null;
    const le = window.localForensicEngine;

    if (path === '/api/cases') return le.getCases();
    if (path === '/api/health') return { status: "ONLINE", system: "WHO IS THE CULPRIT? Forensic Engine (Vercel Serverless / Client Engine)", version: "2.0-VERCEL" };
    if (path === '/api/cpp-code') {
      return {
        merge_sort: "// C++17 Divide-and-Conquer Merge Sort for Suspect Prioritization (O(N log N))\nvoid mergeSort(std::vector<Suspect>& arr, int left, int right) {\n    if (left >= right) return;\n    int mid = left + (right - left) / 2;\n    mergeSort(arr, left, mid);\n    mergeSort(arr, mid + 1, right);\n    merge(arr, left, mid, right);\n}\n\nvoid merge(std::vector<Suspect>& arr, int left, int mid, int right) {\n    std::vector<Suspect> L(arr.begin() + left, arr.begin() + mid + 1);\n    std::vector<Suspect> R(arr.begin() + mid + 1, arr.begin() + right + 1);\n    int i = 0, j = 0, k = left;\n    while (i < L.size() && j < R.size()) {\n        if (L[i].evidenceScore >= R[j].evidenceScore) arr[k++] = L[i++];\n        else arr[k++] = R[j++];\n    }\n    while (i < L.size()) arr[k++] = L[i++];\n    while (j < R.size()) arr[k++] = R[j++];\n}",
        backtracking: "// C++17 Backtracking Constraint Satisfaction Elimination Engine\nvoid backtrackSolve(const vector<Suspect>& suspects, size_t sIdx,\n                    const vector<Clue>& clues, size_t cIdx,\n                    vector<Suspect>& solutions) {\n    if (sIdx >= suspects.size()) return;\n    const auto& cand = suspects[sIdx];\n    if (cIdx >= clues.size()) {\n        solutions.push_back(cand);\n        backtrackSolve(suspects, sIdx + 1, clues, 0, solutions);\n        return;\n    }\n    if (evaluateConstraint(cand, clues[cIdx])) {\n        backtrackSolve(suspects, sIdx, clues, cIdx + 1, solutions);\n    } else {\n        logBranchPruned(cand, clues[cIdx]);\n        backtrackSolve(suspects, sIdx + 1, clues, 0, solutions);\n    }\n}",
        bfs: "// C++17 Adjacency List Breadth-First Search (O(V + E))\nBFSResult runBFS(const std::string& startId) {\n    std::queue<std::string> q;\n    std::unordered_set<std::string> visited;\n    q.push(startId); visited.insert(startId);\n    distance[startId] = 0;\n    while (!q.empty()) {\n        std::string u = q.front(); q.pop();\n        for (const auto& edge : adjList[u]) {\n            std::string v = edge.targetId;\n            if (visited.find(v) == visited.end()) {\n                visited.insert(v);\n                distance[v] = distance[u] + 1;\n                parent[v] = u;\n                q.push(v);\n            }\n        }\n    }\n    return result;\n}",
        kmp: "// C++17 Knuth-Morris-Pratt (KMP) Linear-Time String Matching (O(N + M))\nstd::vector<int> computeLps(const std::string& pat) {\n    int m = pat.length(), len = 0, i = 1;\n    std::vector<int> lps(m, 0);\n    while (i < m) {\n        if (pat[i] == pat[len]) { len++; lps[i++] = len; }\n        else if (len != 0) len = lps[len - 1];\n        else lps[i++] = 0;\n    }\n    return lps;\n}\n\nvoid kmpSearch(const std::string& txt, const std::string& pat) {\n    auto lps = computeLps(pat);\n    int i = 0, j = 0;\n    while (i < txt.length()) {\n        if (pat[j] == txt[i]) { i++; j++; }\n        if (j == pat.length()) {\n            recordMatch(i - j, i - 1);\n            j = lps[j - 1];\n        } else if (i < txt.length() && pat[j] != txt[i]) {\n            if (j != 0) j = lps[j - 1];\n            else i++;\n        }\n    }\n}"
      };
    }

    if (path.startsWith('/api/cases/')) {
      const parts = path.split('/');
      const caseId = parts[3];
      const subRoute = parts.slice(4).join('/');

      if (!subRoute) return le.getCase(caseId);
      if (subRoute === 'graph') return le.getGraphData(caseId);
      if (subRoute === 'memory') return le.getMemory(caseId);
      if (subRoute === 'algorithms/sort') return le.runMergeSort(caseId);
      if (subRoute === 'algorithms/backtracking') return le.runBacktracking(caseId);
      if (subRoute === 'algorithms/bfs') return le.runBFS(caseId, payload.startNode || `VICTIM_${caseId}`);
      if (subRoute === 'algorithms/dfs') return le.runBFS(caseId, payload.startNode || `VICTIM_${caseId}`);
      if (subRoute === 'algorithms/kmp') {
        const stmt = (le.getCase(caseId).witnessStatements || [])[0];
        return le.runKMP(stmt ? stmt.statementText : "", payload.pattern || "dark grey sedan");
      }
      if (subRoute === 'algorithms/search') {
        const cf = le.getCase(caseId);
        const q = (payload.query || '').toLowerCase();
        if (payload.target === 'evidence') {
          const matches = cf.evidenceRecords.filter(e => e.name.toLowerCase().includes(q) || e.category.toLowerCase().includes(q));
          return { algorithmName: 'Linear Search', totalComparisons: cf.evidenceRecords.length, matches, traceLogs: [] };
        } else {
          const matches = cf.suspects.filter(s => s.fullName.toLowerCase().includes(q) || s.occupation.toLowerCase().includes(q));
          return { algorithmName: 'Linear Search', totalComparisons: cf.suspects.length, matches, traceLogs: [] };
        }
      }
      if (subRoute === 'algorithms/shortest-path') {
        return { fromId: payload.fromId, toId: payload.toId, path: le.getShortestPath(caseId, payload.fromId, payload.toId), pathLength: 2 };
      }
      if (subRoute === 'solve') {
        const ranking = le.runMergeSort(caseId);
        const bt = le.runBacktracking(caseId);
        const bfs = le.runBFS(caseId, `VICTIM_${caseId}`);
        return {
          caseId: caseId,
          caseTitle: le.getCase(caseId).caseTitle,
          investigator: le.getCase(caseId).assignedInvestigator,
          rankingResult: ranking,
          clueMatchingResult: bt,
          primaryNetworkBfs: bfs,
          primaryPersonsOfInterest: bt.matchingSuspects,
          executiveForensicSummary: `COMPREHENSIVE FORENSIC SYNTHESIS: Investigation evaluated ${ranking.sortedSuspects.length} suspects and case evidence records. Merge Sort prioritized ${ranking.sortedSuspects[0].fullName} as prime suspect (${ranking.sortedSuspects[0].evidenceRelevanceScore}%). CSP Backtracking identified ${bt.matchingSuspects.length} candidate(s) strictly satisfying 100% of the empirical clues.`,
          legalDisclaimer: "EDUCATIONAL FORENSIC SIMULATION: Matching candidates represent persons of high investigative interest who satisfy all empirical constraints."
        };
      }
    }
    return null;
  }

  // ========================================================================
  // CASE MANAGEMENT
  // ========================================================================
  async loadCases() {
    const list = await this.apiGet('/api/cases');
    if (list && list.length > 0) {
      this.cases = list;
      this.populateCaseSelector();
      this.renderCasesGrid();
    }
  }

  populateCaseSelector() {
    const sel = document.getElementById('caseSelector');
    if (!sel) return;
    sel.innerHTML = this.cases.map(c => `
      <option value="${c.caseId}" ${c.caseId === this.currentCaseId ? 'selected' : ''}>
        ${c.caseTitle} (${c.caseId})
      </option>
    `).join('');
  }

  async switchCase(caseId) {
    this.currentCaseId = caseId;
    const caseData = await this.apiGet(`/api/cases/${caseId}`);
    if (!caseData) return;

    this.currentCaseData = caseData;

    // Update UI components
    this.renderDashboard();
    this.renderSuspects();
    this.renderEvidence();
    this.renderWitnesses();
    this.loadGraph();
    this.loadMemoryAnalysis();
    this.populateProofPathDropdown();
    this.generateReport();
    this.showCppSource('merge_sort');

    // Run algorithms in background to prefill tabs
    this.runMergeSortTab();
    this.runBacktrackingTab();
  }

  async refreshCurrentCase() {
    await this.switchCase(this.currentCaseId);
  }

  // ========================================================================
  // TAB NAVIGATION
  // ========================================================================
  switchTab(tabId) {
    document.querySelectorAll('.tab-pane').forEach(p => p.classList.remove('active'));
    document.querySelectorAll('.nav-item').forEach(b => b.classList.remove('active'));

    const targetPane = document.getElementById(tabId);
    if (targetPane) targetPane.classList.add('active');

    const navBtn = document.querySelector(`.nav-item[data-tab="${tabId}"]`);
    if (navBtn) navBtn.classList.add('active');

    if (tabId === 'tab-graph' && window.graphEngine) {
      setTimeout(() => {
        window.graphEngine.initCanvasSize();
        window.graphEngine.resetZoom();
      }, 100);
    }
  }

  // ========================================================================
  // DASHBOARD RENDERING
  // ========================================================================
  renderDashboard() {
    const c = this.currentCaseData;
    if (!c) return;

    // Stat counters
    const statCases = document.getElementById('statTotalCases');
    const statSuspects = document.getElementById('statTotalSuspects');
    const statEvidence = document.getElementById('statTotalEvidence');
    const statWitnesses = document.getElementById('statTotalWitnesses');
    const statClues = document.getElementById('statTotalClues');

    if (statCases) statCases.innerText = this.cases.length || 3;
    if (statSuspects) statSuspects.innerText = (c.suspects || []).length;
    if (statEvidence) statEvidence.innerText = (c.evidenceRecords || []).length;
    if (statWitnesses) statWitnesses.innerText = (c.witnessStatements || []).length;
    if (statClues) statClues.innerText = (c.clues || []).length;

    // Active Case File Card
    const dashId = document.getElementById('dashCaseId');
    const dashTitle = document.getElementById('dashCaseTitle');
    const dashLoc = document.getElementById('dashCaseLocation');
    const dashTime = document.getElementById('dashCaseTime');
    const dashOfficer = document.getElementById('dashCaseOfficer');
    const dashVictim = document.getElementById('dashCaseVictim');
    const dashDesc = document.getElementById('dashCaseDesc');

    if (dashId) dashId.innerText = c.caseId;
    if (dashTitle) dashTitle.innerText = c.caseTitle;
    if (dashLoc) dashLoc.innerText = c.crimeLocation;
    if (dashTime) dashTime.innerText = c.dateTime;
    if (dashOfficer) dashOfficer.innerText = c.assignedInvestigator;
    if (dashVictim) dashVictim.innerText = `${c.victimName} (${c.victimDetails || 'Victim'})`;
    if (dashDesc) dashDesc.innerText = c.caseDescription;

    // Clues list
    const cluesList = document.getElementById('dashCluesList');
    if (cluesList) {
      cluesList.innerHTML = (c.clues || []).map(clue => `
        <div class="clue-item">
          <div class="clue-badge">${clue.id}</div>
          <div class="clue-content">
            <div class="clue-title">${clue.title}</div>
            <div class="clue-desc">${clue.description}</div>
          </div>
        </div>
      `).join('');
    }

    // Activity timeline
    const timeline = document.getElementById('dashActivityTimeline');
    if (timeline) {
      timeline.innerHTML = `
        <div class="timeline-item">
          <div class="timeline-dot"></div>
          <div class="timeline-time">${c.dateTime}</div>
          <div class="timeline-text">First Responder report logged at ${c.crimeLocation}.</div>
        </div>
        <div class="timeline-item">
          <div class="timeline-dot"></div>
          <div class="timeline-time">Forensic Collection Window</div>
          <div class="timeline-text">${(c.evidenceRecords || []).length} evidentiary items secured under Chain of Custody.</div>
        </div>
        <div class="timeline-item">
          <div class="timeline-dot"></div>
          <div class="timeline-time">Algorithmic Engine Ready</div>
          <div class="timeline-text">Merge Sort and CSP Backtracking elimination models configured.</div>
        </div>
      `;
    }
  }

  // ========================================================================
  // CASES MANAGEMENT TAB
  // ========================================================================
  renderCasesGrid() {
    const container = document.getElementById('casesGridContainer');
    if (!container) return;

    container.innerHTML = this.cases.map(c => `
      <div class="dossier-card mb-4" style="border-left-color: ${c.caseId === this.currentCaseId ? '#e11d48' : '#3b82f6'};">
        <div class="dossier-header">
          <span class="status-pill status-active">${c.status || 'Active Case'}</span>
          <span class="case-id-tag">${c.caseId}</span>
        </div>
        <h3 class="dossier-title">${c.caseTitle}</h3>
        <div class="dossier-meta">
          <div><strong>CRIME:</strong> ${c.crimeType}</div>
          <div><strong>LOCATION:</strong> ${c.crimeLocation}</div>
          <div><strong>INVESTIGATOR:</strong> ${c.assignedInvestigator}</div>
          <div><strong>VICTIM:</strong> ${c.victimName}</div>
        </div>
        <p class="dossier-desc">${c.caseDescription}</p>
        <div class="dossier-actions">
          <button class="btn btn-sm ${c.caseId === this.currentCaseId ? 'btn-danger' : 'btn-outline'}" onclick="window.culpritApp.switchCase('${c.caseId}')">
            ${c.caseId === this.currentCaseId ? 'Active Case' : 'Load This Case'}
          </button>
        </div>
      </div>
    `).join('');
  }

  // ========================================================================
  // SUSPECTS TAB & COMPARISON MATRIX
  // ========================================================================
  renderSuspects(suspectsList = null) {
    const suspects = suspectsList || (this.currentCaseData ? this.currentCaseData.suspects : []);
    const container = document.getElementById('suspectsListContainer');
    if (!container) return;

    container.innerHTML = suspects.map(s => `
      <div class="suspect-card">
        <div class="suspect-header">
          <div class="suspect-identity">
            <div class="suspect-avatar">${s.fullName.charAt(0)}</div>
            <div>
              <div class="suspect-name">${s.fullName}</div>
              <div class="suspect-sub">Age: ${s.age} &bull; ${s.occupation}</div>
            </div>
          </div>
          <span class="status-pill ${s.investigationStatus.includes('Prime') ? 'status-active' : 'status-pill'}">
            ${s.investigationStatus}
          </span>
        </div>
        <div class="suspect-body">
          <div class="suspect-detail-row">
            <strong>RELATION TO VICTIM:</strong> ${s.relationshipToVictim}
          </div>
          <div class="suspect-detail-row">
            <strong>CLAIMED ALIBI:</strong> ${s.alibi}
            <span class="alibi-badge ${s.alibiVerified ? 'alibi-verified' : 'alibi-broken'}">
              ${s.alibiVerified ? 'VERIFIED' : 'UNVERIFIED / BROKEN'}
            </span>
          </div>
          <div class="suspect-detail-row">
            <strong>MOTIVE:</strong> ${s.motive}
          </div>
          <div class="suspect-detail-row">
            <strong>OPPORTUNITY:</strong> ${s.opportunity}
          </div>
          <div class="score-meter mt-2">
            <span class="text-xs text-dim">EVIDENCE RELEVANCE:</span>
            <div class="score-bar-bg flex-1">
              <div class="score-bar-fill" style="width: ${Math.min(100, s.evidenceRelevanceScore)}%;"></div>
            </div>
            <span class="score-num text-accent">${s.evidenceRelevanceScore.toFixed(1)}%</span>
          </div>
          <div class="suspect-chips">
            ${s.presenceInTimeWindow ? '<span class="chip-tag text-accent">Time Window Opportunity</span>' : ''}
            ${s.accessToMeansOrWeapon ? '<span class="chip-tag text-accent">Access to Means/Weapon</span>' : ''}
            ${s.forensicTraceMatched ? '<span class="chip-tag text-accent">Forensic Snag Matched</span>' : ''}
            ${s.substantiatedMotive ? '<span class="chip-tag text-accent">Motive Substantiated</span>' : ''}
          </div>
        </div>
      </div>
    `).join('');

    this.renderSuspectComparisonMatrix();
  }

  renderSuspectComparisonMatrix() {
    const suspects = this.currentCaseData ? this.currentCaseData.suspects : [];
    const header = document.getElementById('comparisonTableHeader');
    const tbody = document.getElementById('comparisonTableBody');
    if (!header || !tbody) return;

    header.innerHTML = `<th>CRITERIA / CLUE</th>` + suspects.map(s => `
      <th>${s.fullName}<br><span class="text-dim text-xs">${s.occupation}</span></th>
    `).join('');

    const rows = [
      { label: 'Time Window Opportunity', key: 'presenceInTimeWindow', type: 'bool' },
      { label: 'Access to Lethal Means/Weapon', key: 'accessToMeansOrWeapon', type: 'bool' },
      { label: 'Biological/Forensic Trace Match', key: 'forensicTraceMatched', type: 'bool' },
      { label: 'Alibi Verification Status', key: 'alibiVerified', type: 'alibi' },
      { label: 'Substantiated Financial/Revenge Motive', key: 'substantiatedMotive', type: 'bool' },
      { label: 'Direct Scene Trace (Vehicle/Audio/Print)', key: 'directSceneTrace', type: 'bool' },
      { label: 'Calculated Evidence Relevance Score', key: 'evidenceRelevanceScore', type: 'score' },
      { label: 'Investigation Classification', key: 'investigationStatus', type: 'status' }
    ];

    tbody.innerHTML = rows.map(r => `
      <tr>
        <td><strong>${r.label}</strong></td>
        ${suspects.map(s => {
          if (r.type === 'bool') {
            const val = s[r.key];
            return `<td><span class="status-pill ${val ? 'status-active' : 'status-pill'}">${val ? 'SATISFIED' : 'ABSENT'}</span></td>`;
          } else if (r.type === 'alibi') {
            return `<td><span class="alibi-badge ${s.alibiVerified ? 'alibi-verified' : 'alibi-broken'}">${s.alibiVerified ? 'VERIFIED' : 'COLLAPSED'}</span></td>`;
          } else if (r.type === 'score') {
            return `<td><strong class="text-accent">${s.evidenceRelevanceScore.toFixed(1)}%</strong></td>`;
          } else {
            return `<td><span class="badge-tag">${s[r.key]}</span></td>`;
          }
        }).join('')}
      </tr>
    `).join('');
  }

  async executeSuspectSearch() {
    const query = document.getElementById('suspectSearchInput').value.trim();
    const field = document.getElementById('suspectSearchField').value;
    if (!query) {
      this.renderSuspects();
      return;
    }

    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/algorithms/search`, {
      target: 'suspects',
      field: field,
      query: query
    });

    if (res) {
      window.algoVisualizer.setAlgoData('linear_search', res);
      this.renderSuspects(res.matches || []);
    }
  }

  // ========================================================================
  // EVIDENCE TAB
  // ========================================================================
  renderEvidence(evidenceList = null) {
    const list = evidenceList || (this.currentCaseData ? this.currentCaseData.evidenceRecords : []);
    const container = document.getElementById('evidenceGridContainer');
    if (!container) return;

    container.innerHTML = list.map(e => `
      <div class="evidence-card">
        <div class="pin-marker"></div>
        <div class="evidence-header">
          <span class="evidence-id-tag">${e.id}</span>
          <span class="evidence-cat-badge">${e.category}</span>
        </div>
        <h4 class="evidence-title">${e.name}</h4>
        <p class="evidence-desc">${e.description}</p>
        <div class="score-meter mb-2">
          <span class="text-xs text-dim">RELIABILITY:</span>
          <div class="score-bar-bg flex-1">
            <div class="score-bar-fill" style="width: ${e.reliabilityScore}%;"></div>
          </div>
          <span class="score-num">${e.reliabilityScore}%</span>
        </div>
        <div class="evidence-meta">
          <div><strong>COLLECTED AT:</strong> ${e.collectedLocation}</div>
          <div><strong>CHAIN OF CUSTODY:</strong> ${e.chainOfCustody}</div>
          ${e.linkedSuspectName ? `<div><strong>LINKED SUSPECT:</strong> <span class="text-accent">${e.linkedSuspectName}</span></div>` : ''}
          <div><strong>LAB NOTES:</strong> ${e.forensicNotes || 'Sealed for court submission.'}</div>
        </div>
      </div>
    `).join('');
  }

  filterEvidenceByCategory(category) {
    if (!this.currentCaseData) return;
    if (category === 'ALL') {
      this.renderEvidence();
    } else {
      const filtered = this.currentCaseData.evidenceRecords.filter(e => e.category === category);
      this.renderEvidence(filtered);
    }
  }

  async executeEvidenceSearch() {
    const query = document.getElementById('evidenceSearchInput').value.trim();
    if (!query) {
      this.renderEvidence();
      return;
    }

    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/algorithms/search`, {
      target: 'evidence',
      field: 'all',
      query: query
    });

    if (res) {
      window.algoVisualizer.setAlgoData('linear_search', res);
      this.renderEvidence(res.matches || []);
    }
  }

  // ========================================================================
  // WITNESS STATEMENTS & KMP TAB
  // ========================================================================
  renderWitnesses() {
    const list = this.currentCaseData ? this.currentCaseData.witnessStatements : [];
    const container = document.getElementById('witnessListContainer');
    const selector = document.getElementById('kmpWitnessSelector');

    if (container) {
      container.innerHTML = list.map(w => `
        <div class="witness-card">
          <div class="witness-header">
            <div>
              <div class="witness-name">${w.witnessName}</div>
              <div class="text-dim text-xs">${w.occupation} &bull; ${w.relationshipToCase} &bull; ${w.statementDateTime}</div>
            </div>
            <span class="badge-tag">Credibility: ${w.credibilityRating}%</span>
          </div>
          <div class="witness-statement-quote">
            "${w.statementText}"
          </div>
          <div class="suspect-chips">
            <span class="text-xs text-dim mr-2">Key Mentioned Terms:</span>
            ${(w.mentionedKeywords || []).map(kw => `<span class="chip-tag" onclick="window.culpritApp.quickKmpSearch('${w.id}', '${kw}')" style="cursor: pointer;">${kw}</span>`).join('')}
          </div>
        </div>
      `).join('');
    }

    if (selector) {
      selector.innerHTML = list.map(w => `
        <option value="${w.id}">${w.witnessName} (${w.id} - ${w.occupation})</option>
      `).join('');
    }
  }

  quickKmpSearch(statementId, keyword) {
    const sel = document.getElementById('kmpWitnessSelector');
    const input = document.getElementById('kmpPatternInput');
    if (sel) sel.value = statementId;
    if (input) input.value = keyword;
    this.executeKmpSearch();
  }

  async executeKmpSearch() {
    const statementId = document.getElementById('kmpWitnessSelector').value;
    const pattern = document.getElementById('kmpPatternInput').value.trim();
    if (!pattern) return;

    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/algorithms/kmp`, {
      statementId,
      pattern
    });

    if (res) {
      window.algoVisualizer.setAlgoData('kmp', res);
      this.displayKmpResult(res);
    }
  }

  displayKmpResult(res) {
    const container = document.getElementById('kmpResultContainer');
    const metrics = document.getElementById('kmpMetricsBar');
    const lpsWrapper = document.getElementById('kmpLpsTableWrapper');
    const transcriptBox = document.getElementById('kmpTranscriptBox');

    if (!container) return;
    container.style.display = 'block';

    if (metrics) {
      metrics.innerHTML = `
        <div>PATTERN: <strong>"${res.pattern}"</strong> (Len ${res.patternLength})</div>
        <div>MATCHES: <strong class="text-accent">${(res.occurrences || []).length}</strong></div>
        <div>COMPARISONS: <strong>${res.totalCharacterComparisons}</strong></div>
        <div>NAIVE COMPARISONS SAVED: <strong class="text-emerald">${res.naiveBruteForceComparisons - res.totalCharacterComparisons} probes</strong></div>
        <div>EXEC TIME: <strong>${res.executionTimeMicros}&mu;s</strong></div>
      `;
    }

    // Render Pi Table
    if (lpsWrapper && res.lpsTable) {
      let headHtml = '<tr><th>Index (i)</th>' + res.lpsTable.map((_, i) => `<th>${i}</th>`).join('') + '</tr>';
      let charHtml = '<tr><th>Char</th>' + res.pattern.split('').map(c => `<td><strong>${c}</strong></td>`).join('') + '</tr>';
      let valHtml = '<tr><th>&Pi;[i] (LPS)</th>' + res.lpsTable.map(v => `<td>${v}</td>`).join('') + '</tr>';
      lpsWrapper.innerHTML = `<table class="lps-table">${headHtml}${charHtml}${valHtml}</table>`;
    }

    // Highlight text in statement
    if (transcriptBox && this.currentCaseData) {
      const stmt = this.currentCaseData.witnessStatements.find(w => w.id === res.witnessStatementId);
      if (stmt) {
        const fullText = stmt.statementText;
        const re = new RegExp(`(${res.pattern})`, 'gi');
        const highlighted = fullText.replace(re, `<mark class="kmp-highlight">$1</mark>`);
        transcriptBox.innerHTML = `<strong>DEPOSITION TRANSCRIPT (${stmt.witnessName}):</strong><br><br>${highlighted}`;
      }
    }
  }

  // ========================================================================
  // INTERACTIVE GRAPH
  // ========================================================================
  async loadGraph() {
    const graphData = await this.apiGet(`/api/cases/${this.currentCaseId}/graph`);
    if (graphData && window.graphEngine) {
      window.graphEngine.setData(graphData);
    }
  }

  async runBfsFromSelected() {
    const startNode = window.graphEngine && window.graphEngine.selectedNode ? window.graphEngine.selectedNode.id : `VICTIM_${this.currentCaseId}`;
    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/algorithms/bfs`, { startNode });

    if (res) {
      window.algoVisualizer.setAlgoData('bfs', res);
      if (window.graphEngine && res.traversalOrder) {
        window.graphEngine.animateBfs(res.traversalOrder);
      }
    }
  }

  async runDfsFromSelected() {
    const startNode = window.graphEngine && window.graphEngine.selectedNode ? window.graphEngine.selectedNode.id : `VICTIM_${this.currentCaseId}`;
    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/algorithms/dfs`, { startNode });

    if (res) {
      window.algoVisualizer.setAlgoData('dfs', res);
      if (window.graphEngine && res.traversalOrder) {
        window.graphEngine.animateBfs(res.traversalOrder);
      }
    }
  }

  setBfsRootFromInspector() {
    this.runBfsFromSelected();
  }

  setDfsRootFromInspector() {
    this.runDfsFromSelected();
  }

  // ========================================================================
  // EVIDENTIARY SHORTEST PROOF PATH & MEMORY ANALYSIS
  // ========================================================================
  populateProofPathDropdown() {
    const sel = document.getElementById('proofPathSuspectSelect');
    if (!sel || !this.currentCaseData) return;

    sel.innerHTML = (this.currentCaseData.suspects || []).map(s => `
      <option value="${s.id}">${s.fullName} (${s.id})</option>
    `).join('');
  }

  async findProofPath() {
    const sel = document.getElementById('proofPathSuspectSelect');
    const summary = document.getElementById('proofPathSummary');
    if (!sel || !this.currentCaseData) return;

    const fromId = sel.value;
    const toId = `VICTIM_${this.currentCaseId}`;

    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/algorithms/shortest-path`, { fromId, toId });
    if (res && res.path && res.path.length > 0) {
      if (window.graphEngine) {
        window.graphEngine.highlightPath(res.path);
      }
      if (summary) {
        summary.innerHTML = `<strong>Shortest Proof Chain:</strong> ${res.path.join(' &rarr; ')} (${res.pathLength} hops)`;
      }
    } else {
      if (summary) {
        summary.innerText = 'No direct evidence chain connects to victim.';
      }
    }
  }

  async loadMemoryAnalysis() {
    const ma = await this.apiGet(`/api/cases/${this.currentCaseId}/memory`);
    if (!ma) return;

    const vEl = document.getElementById('memVertexCount');
    const eEl = document.getElementById('memEdgeCount');
    const dEl = document.getElementById('memDensity');
    const lEl = document.getElementById('memAdjListBytes');
    const mEl = document.getElementById('memAdjMatrixBytes');
    const rEl = document.getElementById('memRecommendationBox');

    if (vEl) vEl.innerText = ma.vertexCount;
    if (eEl) eEl.innerText = ma.edgeCount;
    if (dEl) dEl.innerText = `${(ma.graphDensity * 100).toFixed(1)}%`;
    if (lEl) lEl.innerText = `${ma.adjListBytes.toLocaleString()} B`;
    if (mEl) mEl.innerText = `${ma.adjMatrixBytes.toLocaleString()} B`;
    if (rEl) {
      rEl.innerHTML = `<strong>ACADEMIC ARCHITECTURAL JUSTIFICATION:</strong> ${ma.recommendation} In police criminal networks, entities form sparse clusters rather than complete graphs (|E| &ll; |V|&sup2;). The Adjacency List representation guarantees optimal O(V + E) traversal efficiency for Breadth-First and Depth-First Search without paying the quadratic O(V&sup2;) space and initialization penalty.`;
    }
  }

  async showCppSource(algoKey) {
    // Update active pill button
    const container = document.getElementById('tab-cpp-code');
    if (container) {
      container.querySelectorAll('.algo-pill').forEach(btn => {
        btn.classList.remove('active');
        if (btn.innerText.toLowerCase().includes(algoKey.replace('_', ' '))) {
          btn.classList.add('active');
        }
      });
    }

    const title = document.getElementById('cppCodeHeaderTitle');
    const codeDisplay = document.getElementById('cppSourceCodeDisplay');
    const proofExp = document.getElementById('cppProofExplanation');

    if (!this.cppCodeCache) {
      this.cppCodeCache = await this.apiGet('/api/cpp-code');
    }

    const code = this.cppCodeCache ? this.cppCodeCache[algoKey] : '// C++ source loading...';

    if (algoKey === 'merge_sort') {
      if (title) title.innerText = 'src/core/MergeSort.hpp — Divide-and-Conquer Sort (O(N log N))';
      if (codeDisplay) codeDisplay.innerText = code || '// MergeSort implementation';
      if (proofExp) {
        proofExp.innerHTML = `
          <div class="text-sm" style="line-height: 1.6;">
            <strong class="text-accent">Recurrence Relation:</strong><br>
            <code>T(N) = 2T(N/2) + &Theta;(N)</code><br><br>
            By <strong>Master Theorem (Case 2)</strong>, where <code>a = 2, b = 2, f(N) = &Theta;(N)</code>:<br>
            <code>c = log_b(a) = log_2(2) = 1</code>.<br>
            Since <code>f(N) = &Theta;(N^c) = &Theta;(N^1)</code>, the overall time complexity is strictly:<br>
            <strong class="text-accent">T(N) = &Theta;(N log N)</strong> across all inputs.<br><br>
            <strong>Space Complexity:</strong> <code>O(N)</code> auxiliary buffer for sorted merge array.
          </div>
        `;
      }
    } else if (algoKey === 'backtracking') {
      if (title) title.innerText = 'src/core/Backtracking.hpp — Constraint Satisfaction Clue Engine';
      if (codeDisplay) codeDisplay.innerText = code || '// Backtracking implementation';
      if (proofExp) {
        proofExp.innerHTML = `
          <div class="text-sm" style="line-height: 1.6;">
            <strong class="text-accent">State-Space Search Tree & Pruning:</strong><br>
            Evaluates <code>N</code> suspects against <code>C</code> clues. Unpruned brute force generates <code>O(N &bull; 2^C)</code> states.<br><br>
            <strong>Branch Pruning Efficiency:</strong><br>
            When a candidate fails constraint <code>C_k</code>, all <code>2^{C - k}</code> descendant sub-trees are pruned immediately.<br>
            Effective empirical complexity drops to <strong class="text-accent">O(N &bull; C_avg)</strong> where <code>C_avg &ll; C</code>.
          </div>
        `;
      }
    } else if (algoKey === 'bfs') {
      if (title) title.innerText = 'src/core/Graph.hpp — Adjacency List BFS (O(V + E))';
      if (codeDisplay) codeDisplay.innerText = code || '// Graph BFS implementation';
      if (proofExp) {
        proofExp.innerHTML = `
          <div class="text-sm" style="line-height: 1.6;">
            <strong class="text-accent">Queue Invariant & Radial Traversal:</strong><br>
            Every vertex is enqueued at most once: <code>O(V)</code>.<br>
            Every edge in the adjacency list is traversed at most twice (undirected): <code>O(2E)</code>.<br><br>
            Total Time Complexity: <strong class="text-accent">O(V + E)</strong>.<br>
            Guarantees the shortest path in unweighted relationship networks.
          </div>
        `;
      }
    } else if (algoKey === 'kmp') {
      if (title) title.innerText = 'src/core/KMP.hpp — Knuth-Morris-Pratt Substring Matcher (O(N + M))';
      if (codeDisplay) codeDisplay.innerText = code || '// KMP implementation';
      if (proofExp) {
        proofExp.innerHTML = `
          <div class="text-sm" style="line-height: 1.6;">
            <strong class="text-accent">Prefix Function & Automata Shift:</strong><br>
            Precomputes <code>&Pi;</code> table (LPS) in <code>O(M)</code> preprocessing time.<br>
            Text pointer <code>i</code> strictly increases from <code>0</code> to <code>N-1</code> with zero rollbacks.<br><br>
            Total Time Complexity: <strong class="text-accent">O(N + M)</strong> (vs <code>O(N &bull; M)</code> in naive brute-force string search).
          </div>
        `;
      }
    }
  }

  // ========================================================================
  // ALGORITHM CONSOLE HELPERS
  // ========================================================================
  async runMergeSortTab() {
    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/algorithms/sort`);
    if (res) {
      window.algoVisualizer.setAlgoData('merge_sort', res);
      // Update dashboard table
      const dashTable = document.getElementById('dashSuspectTableBody');
      if (dashTable && res.sortedSuspects) {
        dashTable.innerHTML = res.sortedSuspects.map((s, idx) => `
          <tr>
            <td><span class="badge-tag">#${idx + 1}</span></td>
            <td><strong>${s.fullName}</strong></td>
            <td class="text-dim">${s.occupation}</td>
            <td><span class="alibi-badge ${s.alibiVerified ? 'alibi-verified' : 'alibi-broken'}">${s.alibiVerified ? 'VERIFIED' : 'UNVERIFIED'}</span></td>
            <td><strong class="text-accent">${s.evidenceRelevanceScore.toFixed(1)}%</strong></td>
            <td><button class="btn btn-xs btn-outline" onclick="window.culpritApp.switchTab('tab-suspects')">Inspect</button></td>
          </tr>
        `).join('');
      }
    }
  }

  async runBacktrackingTab() {
    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/algorithms/backtracking`);
    if (res) {
      window.algoVisualizer.setAlgoData('backtracking', res);
    }
  }

  showAlgoDetails(algoKey) {
    window.algoVisualizer.showAlgo(algoKey);
  }

  async runAllAlgorithmsDiagnostic() {
    await this.runMergeSortTab();
    await this.runBacktrackingTab();
    await this.runBfsFromSelected();
    await this.runDfsFromSelected();
    await this.executeKmpSearch();
    alert('All C++17 algorithmic modules executed. Check each tab pill for detailed traces.');
  }

  // ========================================================================
  // CASE SOLVER & REPORT GENERATION
  // ========================================================================
  openSolveModal() {
    const modal = document.getElementById('solverModal');
    if (modal) modal.classList.add('active');

    // Run complete solve pipeline
    this.executeFullSolve();
  }

  closeSolveModal() {
    const modal = document.getElementById('solverModal');
    if (modal) modal.classList.remove('active');
  }

  async executeFullSolve() {
    const modalBody = document.getElementById('solverModalBody');
    const res = await this.apiPost(`/api/cases/${this.currentCaseId}/solve`);

    if (!res) {
      if (modalBody) modalBody.innerHTML = '<div class="text-accent text-center">Failed to execute C++ solver.</div>';
      return;
    }

    this.analysisCache = res;

    // Cache to algo visualizers
    if (res.rankingResult) window.algoVisualizer.setAlgoData('merge_sort', res.rankingResult);
    if (res.clueMatchingResult) window.algoVisualizer.setAlgoData('backtracking', res.clueMatchingResult);
    if (res.primaryNetworkBfs) window.algoVisualizer.setAlgoData('bfs', res.primaryNetworkBfs);
    if (res.primaryNetworkDfs) window.algoVisualizer.setAlgoData('dfs', res.primaryNetworkDfs);

    // Render solver modal summary
    if (modalBody) {
      const topSuspect = (res.rankingResult.sortedSuspects && res.rankingResult.sortedSuspects.length > 0) ? res.rankingResult.sortedSuspects[0] : null;
      const matching = res.clueMatchingResult.matchingSuspects || [];

      modalBody.innerHTML = `
        <div class="solver-results">
          <div class="alert-box mb-4" style="background: rgba(225, 29, 72, 0.1); border-color: var(--accent-red);">
            <div class="alert-title text-accent">SIMULATED FORENSIC DETERMINATION</div>
            <div class="alert-body">
              <strong>${matching.length} Candidate(s) Strictly Satisfy All Empirical Case Clues:</strong>
              ${matching.map(m => `
                <div class="mt-2 p-2 bg-dark rounded">
                  <div class="text-accent font-bold" style="font-size: 1.1rem;">&bull; ${m.fullName} (${m.age} yo, ${m.occupation})</div>
                  <div class="text-sm text-dim">Motive: ${m.motive}</div>
                  <div class="text-sm text-dim">Opportunity: ${m.opportunity}</div>
                  <div class="text-xs text-muted mt-1">Calculated Relevance Score: <strong>${m.evidenceRelevanceScore.toFixed(1)}%</strong> &bull; Alibi Status: <span class="text-accent">BROKEN/UNVERIFIED</span></div>
                </div>
              `).join('')}
            </div>
          </div>

          <div class="dashboard-grid mb-3">
            <div class="panel p-3">
              <div class="label-heading">ALGORITHM 1: MERGE SORT RANKING</div>
              <div class="text-sm">Sorted ${res.rankingResult.sortedSuspects.length} suspects using <strong>${res.rankingResult.totalComparisons} comparisons</strong> (O(N log N)).</div>
              <div class="text-sm mt-1">Highest Evidence Correlation: <strong>${topSuspect ? topSuspect.fullName : 'None'} (${topSuspect ? topSuspect.evidenceRelevanceScore.toFixed(1) : 0}%)</strong></div>
            </div>
            <div class="panel p-3">
              <div class="label-heading">ALGORITHM 2: BACKTRACKING CSP</div>
              <div class="text-sm">Tested ${res.clueMatchingResult.totalCandidatesTested} candidates across ${this.currentCaseData.clues.length} clues.</div>
              <div class="text-sm mt-1">Pruned <strong>${res.clueMatchingResult.totalBranchesPruned} invalid branch hypotheses</strong>.</div>
            </div>
          </div>

          <p class="text-xs text-dim italic">${res.legalDisclaimer}</p>
        </div>
      `;
    }

    this.generateReport();
  }

  viewFullReportFromSolver() {
    this.closeSolveModal();
    this.switchTab('tab-report');
  }

  async generateReport() {
    let analysis = this.analysisCache;
    if (!analysis) {
      analysis = await this.apiPost(`/api/cases/${this.currentCaseId}/solve`);
      this.analysisCache = analysis;
    }
    if (!analysis || !this.currentCaseData) return;

    const c = this.currentCaseData;

    // Report Headers
    const repCaseId = document.getElementById('repCaseId');
    const repCrime = document.getElementById('repCrimeType');
    const repTime = document.getElementById('repDateTime');
    const repLoc = document.getElementById('repLocation');
    const repOfficer = document.getElementById('repOfficer');
    const repVictim = document.getElementById('repVictim');
    const repSynopsis = document.getElementById('repSynopsis');

    if (repCaseId) repCaseId.innerText = c.caseId;
    if (repCrime) repCrime.innerText = c.crimeType;
    if (repTime) repTime.innerText = c.dateTime;
    if (repLoc) repLoc.innerText = c.crimeLocation;
    if (repOfficer) repOfficer.innerText = c.assignedInvestigator;
    if (repVictim) repVictim.innerText = `${c.victimName} (${c.victimDetails || 'Victim'})`;
    if (repSynopsis) repSynopsis.innerText = c.caseDescription;

    // Section 2: Suspect Ranking Table
    const repSuspectBody = document.getElementById('repSuspectTableBody');
    if (repSuspectBody && analysis.rankingResult && analysis.rankingResult.sortedSuspects) {
      repSuspectBody.innerHTML = analysis.rankingResult.sortedSuspects.map((s, idx) => `
        <tr>
          <td>#${idx + 1}</td>
          <td><strong>${s.fullName}</strong></td>
          <td>${s.age} / ${s.occupation}</td>
          <td><strong>${s.evidenceRelevanceScore.toFixed(1)}%</strong></td>
          <td>${s.alibiVerified ? 'VERIFIED' : 'UNVERIFIED / BROKEN'}</td>
          <td>${s.investigationStatus}</td>
        </tr>
      `).join('');
    }

    // Section 3: Matching Candidates Box
    const repMatchingBox = document.getElementById('repMatchingBox');
    if (repMatchingBox && analysis.clueMatchingResult) {
      const matches = analysis.clueMatchingResult.matchingSuspects || [];
      repMatchingBox.innerHTML = matches.map(m => `
        <div style="padding: 0.75rem; background: #fff1f2; border-left: 4px solid #dc2626; margin-bottom: 0.5rem;">
          <div style="font-weight: 700; color: #991b1b;">PRIMARY FORENSIC PERSON OF INTEREST: ${m.fullName}</div>
          <div style="font-size: 0.78rem; color: #475569;">${m.occupation} &bull; Age: ${m.age}</div>
          <div style="font-size: 0.78rem; margin-top: 0.25rem;"><strong>Motive:</strong> ${m.motive}</div>
          <div style="font-size: 0.78rem;"><strong>Opportunity:</strong> ${m.opportunity}</div>
          <div style="font-size: 0.78rem;"><strong>Status:</strong> Satisfies 100% of empirical case clues (Timeline, Means, Trace, Broken Alibi, Motive, Scene Artifacts).</div>
        </div>
      `).join('');
    }

    // Section 4: Evidence Table
    const repEvBody = document.getElementById('repEvidenceTableBody');
    if (repEvBody && c.evidenceRecords) {
      repEvBody.innerHTML = c.evidenceRecords.map(e => `
        <tr>
          <td><code>${e.id}</code></td>
          <td><strong>${e.name}</strong><br><span style="font-size: 0.72rem; color: #64748b;">${e.description}</span></td>
          <td>${e.category}</td>
          <td>${e.reliabilityScore}%</td>
          <td>${e.chainOfCustody}</td>
        </tr>
      `).join('');
    }

    // Section 5: Witness Summary
    const repWitSummary = document.getElementById('repWitnessSummary');
    if (repWitSummary && c.witnessStatements) {
      repWitSummary.innerHTML = c.witnessStatements.map(w => `
        <div style="margin-bottom: 0.65rem; padding: 0.5rem; background: #f8fafc; border: 1px solid #e2e8f0;">
          <strong>${w.witnessName} (${w.occupation}):</strong> "${w.statementText}"
        </div>
      `).join('');
    }

    // Section 7: Final Findings
    const repFinal = document.getElementById('repFinalFindings');
    if (repFinal && analysis.executiveForensicSummary) {
      repFinal.innerHTML = analysis.executiveForensicSummary;
    }
  }

  // ========================================================================
  // MODAL CREATION FORMS
  // ========================================================================
  openNewSuspectModal() {
    const modal = document.getElementById('newSuspectModal');
    if (modal) modal.classList.add('active');
  }

  openNewEvidenceModal() {
    const modal = document.getElementById('newEvidenceModal');
    const sel = document.getElementById('evLinkedSuspect');
    if (sel && this.currentCaseData) {
      sel.innerHTML = '<option value="">-- No Direct Link --</option>' + (this.currentCaseData.suspects || []).map(s => `
        <option value="${s.id}">${s.fullName} (${s.id})</option>
      `).join('');
    }
    if (modal) modal.classList.add('active');
  }

  openNewWitnessModal() {
    const modal = document.getElementById('newWitnessModal');
    if (modal) modal.classList.add('active');
  }

  closeModal(id) {
    const modal = document.getElementById(id);
    if (modal) modal.classList.remove('active');
  }

  async submitNewSuspect(e) {
    e.preventDefault();
    const payload = {
      fullName: document.getElementById('suspName').value,
      age: document.getElementById('suspAge').value,
      occupation: document.getElementById('suspOccupation').value,
      relationshipToVictim: document.getElementById('suspRelationship').value,
      alibi: document.getElementById('suspAlibi').value,
      motive: document.getElementById('suspMotive').value,
      opportunity: document.getElementById('suspOpportunity').value,
      presenceInTimeWindow: document.getElementById('suspPresence').checked ? 'true' : 'false',
      accessToMeansOrWeapon: document.getElementById('suspWeaponAccess').checked ? 'true' : 'false',
      forensicTraceMatched: document.getElementById('suspForensicMatch').checked ? 'true' : 'false',
      alibiVerified: document.getElementById('suspAlibiVerified').checked ? 'true' : 'false'
    };

    await this.apiPost(`/api/cases/${this.currentCaseId}/suspects`, payload);
    this.closeModal('newSuspectModal');
    await this.refreshCurrentCase();
  }

  async submitNewEvidence(e) {
    e.preventDefault();
    const payload = {
      name: document.getElementById('evName').value,
      category: document.getElementById('evCategory').value,
      reliabilityScore: document.getElementById('evReliability').value,
      description: document.getElementById('evDescription').value,
      collectedLocation: document.getElementById('evLocation').value,
      linkedSuspectId: document.getElementById('evLinkedSuspect').value
    };

    await this.apiPost(`/api/cases/${this.currentCaseId}/evidence`, payload);
    this.closeModal('newEvidenceModal');
    await this.refreshCurrentCase();
  }

  async submitNewWitness(e) {
    e.preventDefault();
    const payload = {
      witnessName: document.getElementById('witName').value,
      occupation: document.getElementById('witOccupation').value,
      credibilityRating: document.getElementById('witCredibility').value,
      relationshipToCase: document.getElementById('witRelation').value,
      statementText: document.getElementById('witStatement').value
    };

    await this.apiPost(`/api/cases/${this.currentCaseId}/witnesses`, payload);
    this.closeModal('newWitnessModal');
    await this.refreshCurrentCase();
  }
}

// Global initialization
window.addEventListener('DOMContentLoaded', () => {
  window.culpritApp = new CulpritApplication();
});

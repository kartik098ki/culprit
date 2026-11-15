/**
 * WHO IS THE CULPRIT? - Algorithm Console & Execution Visualizer
 * Formats and renders step-by-step trace logs, counters, and complexity metrics
 * Features an Interactive Algorithm Stepper Player for Classroom Demonstrations
 */

class AlgorithmVisualizer {
  constructor() {
    this.currentAlgo = 'merge_sort';
    this.cache = {};
    this.currentStep = 1;
    this.totalSteps = 1;
    this.isPlaying = false;
    this.playTimer = null;
  }

  setAlgoData(algoKey, data) {
    this.cache[algoKey] = data;
    if (this.currentAlgo === algoKey) {
      this.currentStep = 1;
      this.render();
    }
  }

  showAlgo(algoKey) {
    this.stopPlay();
    this.currentAlgo = algoKey;
    this.currentStep = 1;

    // Update active pill button
    document.querySelectorAll('.algo-pill').forEach(btn => {
      btn.classList.remove('active');
      if (btn.innerText.toLowerCase().includes(algoKey.replace('_', ' '))) {
        btn.classList.add('active');
      }
    });

    this.render();
  }

  // ========================================================================
  // STEPPER CONTROLS (FOR TEACHER PRESENTATION)
  // ========================================================================
  firstStep() {
    this.goToStep(1);
  }

  prevStep() {
    if (this.currentStep > 1) {
      this.goToStep(this.currentStep - 1);
    }
  }

  nextStep() {
    if (this.currentStep < this.totalSteps) {
      this.goToStep(this.currentStep + 1);
    } else {
      this.stopPlay();
    }
  }

  lastStep() {
    this.goToStep(this.totalSteps);
  }

  goToStep(step) {
    this.currentStep = Math.max(1, Math.min(this.totalSteps, step));
    this.updateStepperUI();
    this.highlightCurrentStepRow();
  }

  togglePlay() {
    if (this.isPlaying) {
      this.stopPlay();
    } else {
      this.startPlay();
    }
  }

  startPlay() {
    this.isPlaying = true;
    const btn = document.getElementById('algoPlayBtn');
    if (btn) btn.innerText = '⏸ Pause';

    if (this.currentStep >= this.totalSteps) {
      this.currentStep = 1;
    }

    this.playTimer = setInterval(() => {
      if (this.currentStep < this.totalSteps) {
        this.nextStep();
      } else {
        this.stopPlay();
      }
    }, 600);
  }

  stopPlay() {
    this.isPlaying = false;
    if (this.playTimer) {
      clearInterval(this.playTimer);
      this.playTimer = null;
    }
    const btn = document.getElementById('algoPlayBtn');
    if (btn) btn.innerText = '▶ Auto-Play';
  }

  updateStepperUI() {
    const counter = document.getElementById('algoStepCounter');
    const slider = document.getElementById('algoStepSlider');

    if (counter) counter.innerText = `Step ${this.currentStep} of ${this.totalSteps}`;
    if (slider) {
      slider.max = this.totalSteps;
      slider.value = this.currentStep;
    }
  }

  highlightCurrentStepRow() {
    const rows = document.querySelectorAll('#algoStepBody tr');
    rows.forEach((r, idx) => {
      if (idx === this.currentStep - 1) {
        r.classList.add('step-highlight-row');
        r.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
      } else {
        r.classList.remove('step-highlight-row');
      }
    });
  }

  // ========================================================================
  // RENDER LOGIC
  // ========================================================================
  render() {
    const data = this.cache[this.currentAlgo];
    const title = document.getElementById('algoInspectorTitle');
    const subtitle = document.getElementById('algoInspectorSubtitle');
    const tags = document.getElementById('algoComplexityTags');
    const summary = document.getElementById('algoSummaryBox');
    const header = document.getElementById('algoStepHeader');
    const tbody = document.getElementById('algoStepBody');

    if (!title || !tbody) return;

    if (!data) {
      if (summary) summary.innerText = 'No execution trace loaded for this algorithm. Trigger execution or run full solver.';
      tbody.innerHTML = '<tr><td colspan="5" class="text-center text-muted">Awaiting C++ engine execution...</td></tr>';
      this.totalSteps = 1;
      this.updateStepperUI();
      return;
    }

    const steps = data.traceLogs || [];
    this.totalSteps = Math.max(1, steps.length);
    this.updateStepperUI();

    if (this.currentAlgo === 'merge_sort') {
      title.innerText = data.algorithmName || 'Merge Sort (Divide-and-Conquer)';
      subtitle.innerText = 'Divide-and-conquer sorting of suspects by evidence score.';
      tags.innerHTML = `
        <span class="complexity-badge">Best: ${data.timeComplexityBest || 'O(N log N)'}</span>
        <span class="complexity-badge">Worst: ${data.timeComplexityWorst || 'O(N log N)'}</span>
        <span class="complexity-badge">Space: ${data.spaceComplexity || 'O(N)'}</span>
        <span class="complexity-badge">Comparisons: ${data.totalComparisons || 0}</span>
      `;
      summary.innerHTML = `<strong>EXECUTION SYNTHESIS:</strong> ${data.summary || 'Merge sort completed.'}`;
      header.innerHTML = `
        <th>STEP #</th>
        <th>PHASE</th>
        <th>SPAN [L..R]</th>
        <th>COMPARISON DETAIL</th>
        <th>SUB-ARRAY STATE / NOTES</th>
      `;

      tbody.innerHTML = steps.map((s, idx) => `
        <tr class="${idx === this.currentStep - 1 ? 'step-highlight-row' : ''}">
          <td><span class="badge-tag">#${s.stepNumber}</span></td>
          <td><span class="status-pill ${s.phase === 'DIVIDE' ? 'status-active' : 'status-verified'}">${s.phase}</span></td>
          <td><code>[${s.leftIndex}..${s.rightIndex}] (Depth ${s.depth})</code></td>
          <td><strong>${s.comparisonDetails || 'Partition'}</strong></td>
          <td><div class="text-xs text-muted mb-1">${s.notes}</div><code>${s.subArrayState || ''}</code></td>
        </tr>
      `).join('');

    } else if (this.currentAlgo === 'backtracking') {
      title.innerText = data.algorithmName || 'Backtracking / Constraint Satisfaction Engine';
      subtitle.innerText = 'Exhaustively evaluates suspect combinations against empirical clues, pruning invalid branches.';
      tags.innerHTML = `
        <span class="complexity-badge">Time: ${data.timeComplexityPruned || 'O(N * C)'}</span>
        <span class="complexity-badge">Space: ${data.spaceComplexity || 'O(C)'}</span>
        <span class="complexity-badge">Pruned Branches: ${data.totalBranchesPruned || 0}</span>
        <span class="complexity-badge">Matching: ${(data.matchingSuspects || []).length}</span>
      `;
      summary.innerHTML = `<strong>CSP RESOLUTION:</strong> ${data.summary || 'Backtracking completed.'}`;
      header.innerHTML = `
        <th>STEP #</th>
        <th>DEPTH</th>
        <th>CANDIDATE</th>
        <th>ACTION & CONSTRAINT</th>
        <th>DECISION / PRUNING JUSTIFICATION</th>
      `;

      tbody.innerHTML = steps.map((s, idx) => {
        let badgeClass = s.action === 'BRANCH_PRUNED' ? 'status-alert' : (s.action === 'SOLUTION_FOUND' ? 'status-active' : 'status-pill');
        return `
          <tr class="${idx === this.currentStep - 1 ? 'step-highlight-row' : ''}">
            <td><span class="badge-tag">#${s.stepNumber}</span></td>
            <td><code>Depth ${s.treeDepth}</code></td>
            <td><strong>${s.candidateName}</strong></td>
            <td><span class="status-pill ${badgeClass}">${s.action}</span><div class="text-xs text-muted mt-1">${s.constraintTested || ''}</div></td>
            <td class="${s.action === 'BRANCH_PRUNED' ? 'text-accent' : ''}">${s.decisionReason}</td>
          </tr>
        `;
      }).join('');

    } else if (this.currentAlgo === 'bfs') {
      title.innerText = data.algorithmName || 'Breadth-First Search (BFS)';
      subtitle.innerText = 'Radial level-by-level traversal identifying connected entities and degrees of separation.';
      tags.innerHTML = `
        <span class="complexity-badge">Time: ${data.timeComplexity || 'O(V + E)'}</span>
        <span class="complexity-badge">Space: ${data.spaceComplexity || 'O(V)'}</span>
        <span class="complexity-badge">Visited: ${data.verticesVisited || 0} vertices</span>
        <span class="complexity-badge">Traversed: ${data.edgesTraversed || 0} edges</span>
      `;
      summary.innerHTML = `<strong>BFS TOPOLOGY:</strong> ${data.summary || 'BFS complete.'}`;
      header.innerHTML = `
        <th>STEP #</th>
        <th>OPERATION</th>
        <th>CURRENT ENTITY</th>
        <th>DISTANCE LEVEL</th>
        <th>QUEUE LOG & NOTES</th>
      `;

      tbody.innerHTML = steps.map((s, idx) => `
        <tr class="${idx === this.currentStep - 1 ? 'step-highlight-row' : ''}">
          <td><span class="badge-tag">#${s.stepNumber}</span></td>
          <td><span class="status-pill status-verified">${s.action}</span></td>
          <td><strong>${s.currentNodeLabel}</strong></td>
          <td><code>Level ${s.currentDistance}</code></td>
          <td><div class="text-xs text-muted">${s.notes}</div></td>
        </tr>
      `).join('');

    } else if (this.currentAlgo === 'dfs') {
      title.innerText = data.algorithmName || 'Depth-First Search (DFS)';
      subtitle.innerText = 'Deep relationship chain exploration uncovering indirect paths and cycle links.';
      tags.innerHTML = `
        <span class="complexity-badge">Time: ${data.timeComplexity || 'O(V + E)'}</span>
        <span class="complexity-badge">Space: ${data.spaceComplexity || 'O(V)'}</span>
        <span class="complexity-badge">Max Depth: ${data.maxRecursionDepth || 0}</span>
        <span class="complexity-badge">Vertices: ${data.verticesVisited || 0}</span>
      `;
      summary.innerHTML = `<strong>DFS PATH RECONSTRUCTION:</strong> ${data.summary || 'DFS complete.'}`;
      header.innerHTML = `
        <th>STEP #</th>
        <th>OPERATION</th>
        <th>CURRENT ENTITY</th>
        <th>CALL STACK DEPTH</th>
        <th>RELATION CHAIN & NOTES</th>
      `;

      tbody.innerHTML = steps.map((s, idx) => `
        <tr class="${idx === this.currentStep - 1 ? 'step-highlight-row' : ''}">
          <td><span class="badge-tag">#${s.stepNumber}</span></td>
          <td><span class="status-pill status-active">${s.action}</span></td>
          <td><strong>${s.currentNodeLabel}</strong></td>
          <td><code>Depth ${s.currentDistance}</code></td>
          <td><div class="text-xs text-muted">${s.notes}</div></td>
        </tr>
      `).join('');

    } else if (this.currentAlgo === 'kmp') {
      title.innerText = data.algorithmName || 'Knuth-Morris-Pratt (KMP) String Matching';
      subtitle.innerText = 'Linear-time substring search avoiding character rollbacks via the Pi/LPS table.';
      tags.innerHTML = `
        <span class="complexity-badge">Preprocess: ${data.timeComplexityPreprocess || 'O(M)'}</span>
        <span class="complexity-badge">Search: ${data.timeComplexitySearch || 'O(N)'}</span>
        <span class="complexity-badge">Total Time: ${data.timeComplexityTotal || 'O(N + M)'}</span>
        <span class="complexity-badge">Comparisons: ${data.totalCharacterComparisons || 0}</span>
      `;
      summary.innerHTML = `<strong>KMP TRANSCRIPT SCAN:</strong> ${data.summary || 'KMP complete.'}`;
      header.innerHTML = `
        <th>STEP #</th>
        <th>ACTION</th>
        <th>TEXT POINTER (i)</th>
        <th>PATTERN POINTER (j)</th>
        <th>STATE MACHINE LOG</th>
      `;

      tbody.innerHTML = steps.map((s, idx) => `
        <tr class="${idx === this.currentStep - 1 ? 'step-highlight-row' : ''}">
          <td><span class="badge-tag">#${s.stepNumber}</span></td>
          <td><span class="status-pill ${s.action === 'OCCURRENCE_FOUND' ? 'status-alert' : (s.action === 'CHAR_MATCH' ? 'status-verified' : 'status-pill')}">${s.action}</span></td>
          <td><code>i = ${s.textIndex} ('${s.textChar || ''}')</code></td>
          <td><code>j = ${s.patternIndex} ('${s.patternChar || ''}')</code></td>
          <td><div class="text-xs text-muted">${s.notes}</div></td>
        </tr>
      `).join('');

    } else if (this.currentAlgo === 'linear_search') {
      title.innerText = data.algorithmName || 'Linear Search (Sequential Scan)';
      subtitle.innerText = 'Iterative search over suspects or evidence attributes.';
      tags.innerHTML = `
        <span class="complexity-badge">Best: ${data.timeComplexityBest || 'O(1)'}</span>
        <span class="complexity-badge">Worst: ${data.timeComplexityWorst || 'O(N)'}</span>
        <span class="complexity-badge">Comparisons: ${data.totalComparisons || 0}</span>
        <span class="complexity-badge">Matches: ${(data.matches || []).length}</span>
      `;
      summary.innerHTML = `<strong>LINEAR SEARCH SUMMARY:</strong> ${data.summary || 'Search complete.'}`;
      header.innerHTML = `
        <th>STEP #</th>
        <th>ACTION</th>
        <th>ELEMENT ID / NAME</th>
        <th>EVALUATED VALUE</th>
        <th>MATCH OUTCOME</th>
      `;

      tbody.innerHTML = steps.map((s, idx) => `
        <tr class="${idx === this.currentStep - 1 ? 'step-highlight-row' : ''}">
          <td><span class="badge-tag">#${s.stepNumber}</span></td>
          <td><code>${s.action}</code></td>
          <td><strong>${s.candidateName} (${s.candidateId})</strong></td>
          <td><code>${s.evaluatedValue}</code></td>
          <td><span class="status-pill ${s.isMatch ? 'status-verified' : 'status-pill'}">${s.isMatch ? 'MATCH' : 'CONTINUE'}</span></td>
        </tr>
      `).join('');
    }
  }
}

window.algoVisualizer = new AlgorithmVisualizer();

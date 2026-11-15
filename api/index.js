// Vercel Serverless Function API Handler for WHO IS THE CULPRIT
module.exports = (req, res) => {
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

  if (req.method === 'OPTIONS') {
    return res.status(200).end();
  }

  const url = req.url || '';

  if (url.includes('/api/health')) {
    return res.json({
      status: "ONLINE",
      system: "WHO IS THE CULPRIT? Forensic Engine (Vercel Serverless)",
      version: "2.0-VERCEL-SERVERLESS"
    });
  }

  if (url.includes('/api/cpp-code')) {
    return res.json({
      merge_sort: "// C++17 Divide-and-Conquer Merge Sort for Suspect Prioritization (O(N log N))\nvoid mergeSort(std::vector<Suspect>& arr, int left, int right) {\n    if (left >= right) return;\n    int mid = left + (right - left) / 2;\n    mergeSort(arr, left, mid);\n    mergeSort(arr, mid + 1, right);\n    merge(arr, left, mid, right);\n}\n\nvoid merge(std::vector<Suspect>& arr, int left, int mid, int right) {\n    std::vector<Suspect> L(arr.begin() + left, arr.begin() + mid + 1);\n    std::vector<Suspect> R(arr.begin() + mid + 1, arr.begin() + right + 1);\n    int i = 0, j = 0, k = left;\n    while (i < L.size() && j < R.size()) {\n        if (L[i].evidenceScore >= R[j].evidenceScore) arr[k++] = L[i++];\n        else arr[k++] = R[j++];\n    }\n    while (i < L.size()) arr[k++] = L[i++];\n    while (j < R.size()) arr[k++] = R[j++];\n}",
      backtracking: "// C++17 Backtracking Constraint Satisfaction Elimination Engine\nvoid backtrackSolve(const vector<Suspect>& suspects, size_t sIdx,\n                    const vector<Clue>& clues, size_t cIdx,\n                    vector<Suspect>& solutions) {\n    if (sIdx >= suspects.size()) return;\n    const auto& cand = suspects[sIdx];\n    if (cIdx >= clues.size()) {\n        solutions.push_back(cand);\n        backtrackSolve(suspects, sIdx + 1, clues, 0, solutions);\n        return;\n    }\n    if (evaluateConstraint(cand, clues[cIdx])) {\n        backtrackSolve(suspects, sIdx, clues, cIdx + 1, solutions);\n    } else {\n        logBranchPruned(cand, clues[cIdx]);\n        backtrackSolve(suspects, sIdx + 1, clues, 0, solutions);\n    }\n}",
      bfs: "// C++17 Adjacency List Breadth-First Search (O(V + E))\nBFSResult runBFS(const std::string& startId) {\n    std::queue<std::string> q;\n    std::unordered_set<std::string> visited;\n    q.push(startId); visited.insert(startId);\n    distance[startId] = 0;\n    while (!q.empty()) {\n        std::string u = q.front(); q.pop();\n        for (const auto& edge : adjList[u]) {\n            std::string v = edge.targetId;\n            if (visited.find(v) == visited.end()) {\n                visited.insert(v);\n                distance[v] = distance[u] + 1;\n                parent[v] = u;\n                q.push(v);\n            }\n        }\n    }\n    return result;\n}",
      kmp: "// C++17 Knuth-Morris-Pratt (KMP) Linear-Time String Matching (O(N + M))\nstd::vector<int> computeLps(const std::string& pat) {\n    int m = pat.length(), len = 0, i = 1;\n    std::vector<int> lps(m, 0);\n    while (i < m) {\n        if (pat[i] == pat[len]) { len++; lps[i++] = len; }\n        else if (len != 0) len = lps[len - 1];\n        else lps[i++] = 0;\n    }\n    return lps;\n}\n\nvoid kmpSearch(const std::string& txt, const std::string& pat) {\n    auto lps = computeLps(pat);\n    int i = 0, j = 0;\n    while (i < txt.length()) {\n        if (pat[j] == txt[i]) { i++; j++; }\n        if (j == pat.length()) {\n            recordMatch(i - j, i - 1);\n            j = lps[j - 1];\n        } else if (i < txt.length() && pat[j] != txt[i]) {\n            if (j != 0) j = lps[j - 1];\n            else i++;\n        }\n    }\n}"
    });
  }

  return res.json({ status: "OK", message: "Forensic API online" });
};

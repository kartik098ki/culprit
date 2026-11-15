/**
 * WHO IS THE CULPRIT? - Interactive Relationship Network Graph
 * High-performance HTML5 Canvas Force-Directed Topological Engine
 */

class InvestigationGraphEngine {
  constructor(canvasId) {
    this.canvas = document.getElementById(canvasId);
    if (!this.canvas) return;
    this.ctx = this.canvas.getContext('2d');

    this.nodes = [];
    this.links = [];
    this.nodeMap = new Map();

    // Camera / Pan & Zoom
    this.transform = { x: 0, y: 0, scale: 1 };
    this.isDragging = false;
    this.draggedNode = null;
    this.lastMouse = { x: 0, y: 0 };
    this.selectedNode = null;
    this.hoveredNode = null;

    // Traversal Animation State
    this.highlightedNodes = new Set();
    this.highlightedEdges = new Set();
    this.animating = false;
    this.animationStep = 0;

    this.initCanvasSize();
    this.attachEventListeners();
    this.startSimulation();
  }

  initCanvasSize() {
    const rect = this.canvas.parentElement.getBoundingClientRect();
    this.canvas.width = rect.width || 800;
    this.canvas.height = rect.height || 640;
    this.transform.x = this.canvas.width / 2;
    this.transform.y = this.canvas.height / 2;
  }

  setData(graphData) {
    this.nodes = [];
    this.links = [];
    this.nodeMap.clear();

    const width = this.canvas.width;
    const height = this.canvas.height;

    // Build nodes
    (graphData.nodes || []).forEach((n, idx) => {
      const angle = (idx / graphData.nodes.length) * Math.PI * 2;
      const radius = 180 + Math.random() * 80;
      const nodeObj = {
        id: n.id,
        label: n.label,
        type: n.type,
        details: n.details,
        score: n.score || 0,
        x: Math.cos(angle) * radius,
        y: Math.sin(angle) * radius,
        vx: 0,
        vy: 0,
        radius: n.type === 'VICTIM' ? 24 : (n.type === 'SUSPECT' ? 20 : 16)
      };
      this.nodes.push(nodeObj);
      this.nodeMap.set(n.id, nodeObj);
    });

    // Build links
    (graphData.links || []).forEach(l => {
      const source = this.nodeMap.get(l.source);
      const target = this.nodeMap.get(l.target);
      if (source && target) {
        this.links.push({
          source,
          target,
          relationType: l.relationType,
          description: l.description,
          weight: l.weight || 0.5
        });
      }
    });

    this.resetZoom();
  }

  attachEventListeners() {
    window.addEventListener('resize', () => this.initCanvasSize());

    this.canvas.addEventListener('mousedown', e => this.onMouseDown(e));
    this.canvas.addEventListener('mousemove', e => this.onMouseMove(e));
    window.addEventListener('mouseup', () => this.onMouseUp());
    this.canvas.addEventListener('wheel', e => this.onWheel(e));
  }

  toWorld(x, y) {
    return {
      x: (x - this.transform.x) / this.transform.scale,
      y: (y - this.transform.y) / this.transform.scale
    };
  }

  getNodeAt(x, y) {
    const world = this.toWorld(x, y);
    for (let i = this.nodes.length - 1; i >= 0; i--) {
      const n = this.nodes[i];
      const dx = world.x - n.x;
      const dy = world.y - n.y;
      if (dx * dx + dy * dy <= n.radius * n.radius) {
        return n;
      }
    }
    return null;
  }

  onMouseDown(e) {
    const rect = this.canvas.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;

    const hit = this.getNodeAt(x, y);
    if (hit) {
      this.draggedNode = hit;
      this.selectedNode = hit;
      this.updateInspector(hit);
    } else {
      this.isDragging = true;
      this.lastMouse = { x, y };
    }
  }

  onMouseMove(e) {
    const rect = this.canvas.getBoundingClientRect();
    const x = e.clientX - rect.left;
    const y = e.clientY - rect.top;

    if (this.draggedNode) {
      const world = this.toWorld(x, y);
      this.draggedNode.x = world.x;
      this.draggedNode.y = world.y;
      this.draggedNode.vx = 0;
      this.draggedNode.vy = 0;
    } else if (this.isDragging) {
      this.transform.x += (x - this.lastMouse.x);
      this.transform.y += (y - this.lastMouse.y);
      this.lastMouse = { x, y };
    } else {
      const hit = this.getNodeAt(x, y);
      if (hit !== this.hoveredNode) {
        this.hoveredNode = hit;
        this.canvas.style.cursor = hit ? 'pointer' : 'default';
      }
    }
  }

  onMouseUp() {
    this.isDragging = false;
    this.draggedNode = null;
  }

  onWheel(e) {
    e.preventDefault();
    const zoomFactor = e.deltaY < 0 ? 1.1 : 0.9;
    const newScale = Math.max(0.3, Math.min(3.0, this.transform.scale * zoomFactor));

    const rect = this.canvas.getBoundingClientRect();
    const mouseX = e.clientX - rect.left;
    const mouseY = e.clientY - rect.top;

    this.transform.x = mouseX - (mouseX - this.transform.x) * (newScale / this.transform.scale);
    this.transform.y = mouseY - (mouseY - this.transform.y) * (newScale / this.transform.scale);
    this.transform.scale = newScale;
  }

  resetZoom() {
    this.transform.scale = 1.0;
    this.transform.x = this.canvas.width / 2;
    this.transform.y = this.canvas.height / 2;
  }

  updateInspector(node) {
    const badge = document.getElementById('inspectorNodeType');
    const title = document.getElementById('inspectorNodeLabel');
    const desc = document.getElementById('inspectorNodeDetails');
    const meta = document.getElementById('inspectorNodeMeta');

    if (badge) badge.innerText = node.type + ' DOSSIER';
    if (title) title.innerText = node.label;
    if (desc) desc.innerText = node.details || 'Registered investigative entity.';

    if (meta) {
      const connectedEdges = this.links.filter(l => l.source.id === node.id || l.target.id === node.id);
      meta.innerHTML = `
        <div><strong>ENTITY ID:</strong> ${node.id}</div>
        <div><strong>TYPE:</strong> ${node.type}</div>
        <div><strong>DIRECT CONNECTIONS:</strong> ${connectedEdges.length} relation(s)</div>
        ${node.score ? `<div><strong>EVIDENTIARY SCORE:</strong> ${node.score.toFixed(1)}%</div>` : ''}
      `;
    }
  }

  // Force-directed Physics Simulation Step
  updatePhysics() {
    const kRepel = 2800;
    const kAttract = 0.045;
    const centerAttract = 0.008;

    // Repulsion between nodes
    for (let i = 0; i < this.nodes.length; i++) {
      const n1 = this.nodes[i];
      for (let j = i + 1; j < this.nodes.length; j++) {
        const n2 = this.nodes[j];
        const dx = n2.x - n1.x;
        const dy = n2.y - n1.y;
        const distSq = dx * dx + dy * dy + 1;
        const dist = Math.sqrt(distSq);

        if (dist < 400) {
          const force = kRepel / distSq;
          const fx = (dx / dist) * force;
          const fy = (dy / dist) * force;
          n1.vx -= fx;
          n1.vy -= fy;
          n2.vx += fx;
          n2.vy += fy;
        }
      }

      // Center gravity
      n1.vx -= n1.x * centerAttract;
      n1.vy -= n1.y * centerAttract;
    }

    // Spring attraction along links
    this.links.forEach(link => {
      const dx = link.target.x - link.source.x;
      const dy = link.target.y - link.source.y;
      const dist = Math.sqrt(dx * dx + dy * dy) || 1;
      const force = (dist - 140) * kAttract;

      const fx = (dx / dist) * force;
      const fy = (dy / dist) * force;

      link.source.vx += fx;
      link.source.vy += fy;
      link.target.vx -= fx;
      link.target.vy -= fy;
    });

    // Apply velocities with damping
    this.nodes.forEach(n => {
      if (n !== this.draggedNode) {
        n.x += n.vx;
        n.y += n.vy;
        n.vx *= 0.88; // Damping
        n.vy *= 0.88;
      }
    });
  }

  getNodeColor(type) {
    switch (type) {
      case 'VICTIM': return '#ef4444';
      case 'SUSPECT': return '#f59e0b';
      case 'WITNESS': return '#10b981';
      case 'EVIDENCE': return '#06b6d4';
      case 'LOCATION': return '#8b5cf6';
      default: return '#94a3b8';
    }
  }

  render() {
    this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);

    this.ctx.save();
    this.ctx.translate(this.transform.x, this.transform.y);
    this.ctx.scale(this.transform.scale, this.transform.scale);

    // 1. Draw Links
    this.links.forEach(l => {
      const isEdgeActive = this.highlightedEdges.has(l);
      this.ctx.beginPath();
      this.ctx.moveTo(l.source.x, l.source.y);
      this.ctx.lineTo(l.target.x, l.target.y);

      if (isEdgeActive) {
        this.ctx.strokeStyle = '#e11d48';
        this.ctx.lineWidth = 3;
      } else {
        this.ctx.strokeStyle = 'rgba(71, 85, 105, 0.4)';
        this.ctx.lineWidth = 1.2;
      }
      this.ctx.stroke();

      // Relation label at midpoint
      const midX = (l.source.x + l.target.x) / 2;
      const midY = (l.source.y + l.target.y) / 2;
      this.ctx.font = '9px Fira Code';
      this.ctx.fillStyle = isEdgeActive ? '#f43f5e' : 'rgba(148, 163, 184, 0.6)';
      this.ctx.textAlign = 'center';
      this.ctx.fillText(l.relationType, midX, midY - 4);
    });

    // 2. Draw Nodes
    this.nodes.forEach(n => {
      const isSelected = n === this.selectedNode;
      const isHighlighted = this.highlightedNodes.has(n.id);
      const color = this.getNodeColor(n.type);

      // Outer glow for selected or highlighted nodes
      if (isSelected || isHighlighted || n.type === 'VICTIM') {
        this.ctx.beginPath();
        this.ctx.arc(n.x, n.y, n.radius + 8, 0, Math.PI * 2);
        this.ctx.fillStyle = isHighlighted ? 'rgba(225, 29, 72, 0.35)' : 'rgba(59, 130, 246, 0.25)';
        this.ctx.fill();
      }

      // Main Node Circle
      this.ctx.beginPath();
      this.ctx.arc(n.x, n.y, n.radius, 0, Math.PI * 2);
      this.ctx.fillStyle = '#121a2e';
      this.ctx.fill();
      this.ctx.lineWidth = 2.5;
      this.ctx.strokeStyle = isHighlighted ? '#e11d48' : color;
      this.ctx.stroke();

      // Inner icon or initial
      this.ctx.font = 'bold 11px Rajdhani';
      this.ctx.fillStyle = '#fff';
      this.ctx.textAlign = 'center';
      this.ctx.textBaseline = 'middle';
      const initial = n.label.charAt(0);
      this.ctx.fillText(initial, n.x, n.y);

      // Label below node
      this.ctx.font = '11px Inter';
      this.ctx.fillStyle = '#e2e8f0';
      this.ctx.textBaseline = 'top';
      this.ctx.fillText(n.label, n.x, n.y + n.radius + 5);
    });

    this.ctx.restore();
  }

  startSimulation() {
    const loop = () => {
      this.updatePhysics();
      this.render();
      requestAnimationFrame(loop);
    };
    requestAnimationFrame(loop);
  }

  // Highlight specific path chain (for Shortest Evidentiary Proof Path)
  highlightPath(pathArray) {
    this.highlightedNodes.clear();
    this.highlightedEdges.clear();
    if (!pathArray || pathArray.length === 0) return;

    pathArray.forEach(id => this.highlightedNodes.add(id));
    for (let i = 0; i < pathArray.length - 1; i++) {
      const u = pathArray[i];
      const v = pathArray[i + 1];
      this.links.forEach(l => {
        if ((l.source.id === u && l.target.id === v) || (l.source.id === v && l.target.id === u)) {
          this.highlightedEdges.add(l);
        }
      });
    }
  }

  // Animate BFS Traversal Ripple
  animateBfs(traversalOrder) {
    this.highlightedNodes.clear();
    this.highlightedEdges.clear();

    let step = 0;
    const interval = setInterval(() => {
      if (step >= traversalOrder.length) {
        clearInterval(interval);
        return;
      }
      const nodeId = traversalOrder[step];
      this.highlightedNodes.add(nodeId);

      // Highlight edges connecting to previously visited nodes
      this.links.forEach(l => {
        if (this.highlightedNodes.has(l.source.id) && this.highlightedNodes.has(l.target.id)) {
          this.highlightedEdges.add(l);
        }
      });

      const nodeObj = this.nodeMap.get(nodeId);
      if (nodeObj) this.updateInspector(nodeObj);

      step++;
    }, 280);
  }
}

// Global engine instance
window.addEventListener('DOMContentLoaded', () => {
  window.graphEngine = new InvestigationGraphEngine('investigationGraphCanvas');
});

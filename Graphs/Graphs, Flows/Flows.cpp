#include <vector>
#include <iostream>

struct Edge {
  int u = 0;
  int f = 0;
  int c = 0;
  size_t back;
  explicit Edge(int u0, int c0) : u(u0), c(c0) {
  }
};

class Graph {
  std::vector<std::vector<Edge>> graph_;
  int size_;

 public:
  explicit Graph(int n) : graph_(n + 1), size_(n) {
  }
  void AddEdge(int v, int u, int c) {
    graph_[v].emplace_back(Edge(u, c));
    graph_[u].emplace_back(Edge(v, 0));
    graph_[v].back().back = graph_[u].size() - 1;
    graph_[u].back().back = graph_[v].size() - 1;
  }
  int DFS(int v, int cmin, std::vector<bool>& visited) {
    if (v == size_) {
      return cmin;
    }
    visited[v] = true;
    for (auto& edge : graph_[v]) {
      if (!visited[edge.u] && edge.f < edge.c) {
        int delta = DFS(edge.u, std::min(cmin, edge.c - edge.f), visited);
        if (delta > 0) {
          edge.f += delta;
          graph_[edge.u][edge.back].f -= delta;
          return delta;
        }
      }
    }
    return 0;
  }
  int FF() {
    int flow = 0;
    while (true) {
      std::vector<bool> visited(size_ + 1, false);
      int cur = DFS(1, 100000, visited);
      if (cur == 0) {
        break;
      }
      flow += cur;
    }
    return flow;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n = 0;
  int m = 0;
  int v = 0;
  int u = 0;
  int c = 0;
  std::cin >> n >> m;
  Graph graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> v >> u >> c;
    graph.AddEdge(v, u, c);
  }
  std::cout << graph.FF() << '\n';
}

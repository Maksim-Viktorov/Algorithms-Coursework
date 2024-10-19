#include <iostream>
#include <vector>

class UnorientedGraph {
  std::vector<std::vector<int>> graph_;
  bool IsBipartite(std::vector<int>& mark, size_t cur) {
    for (size_t i = 0; i < graph_[cur].size(); ++i) {
      if (mark[graph_[cur][i]] != 0) {
        if (mark[graph_[cur][i]] == mark[cur]) {
          return false;
        }
        continue;
      }
      mark[graph_[cur][i]] = 3 - mark[cur];
      if (!IsBipartite(mark, graph_[cur][i])) {
        return false;
      }
    }
    return true;
  }

 public:
  UnorientedGraph() = default;
  explicit UnorientedGraph(int n) : graph_(n + 1) {
  }
  void AddEdge(int start, int finish) {
    graph_[start].emplace_back(finish);
  }
  bool IsBipartite() {
    if (graph_.empty()) {
      return true;
    }
    std::vector<int> mark(graph_.size());
    for (size_t i = 0; i < mark.size(); ++i) {
      mark[i] = 0;
    }
    for (size_t i = 1; i < graph_.size(); ++i) {
      if (mark[i] == 0) {
        mark[i] = 1;
      }
      if (!IsBipartite(mark, i)) {
        return false;
      }
    }
    return true;
  }
};

int main() {
  size_t n, m;
  std::cin >> n >> m;
  UnorientedGraph graph(n);
  int a, b;
  for (size_t i = 0; i < m; ++i) {
    std::cin >> a >> b;
    graph.AddEdge(a, b);
    graph.AddEdge(b, a);
  }
  std::cout << (graph.IsBipartite() ? "YES" : "NO");
  return 0;
}

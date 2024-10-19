#include <iostream>
#include <vector>

class OrientedGraph {
  std::vector<std::vector<int>> graph_;

 public:
  OrientedGraph() = default;
  explicit OrientedGraph(int n) : graph_(n + 1) {
  }
  void AddEdge(int start, int finish) {
    graph_[start].emplace_back(finish);
  }
  void DFSVisit(int cur, std::vector<bool>& visited) {
    visited[cur] = true;
    for (size_t i = 0; i < graph_[cur].size(); ++i) {
      if (!visited[graph_[cur][i]]) {
        DFSVisit(graph_[cur][i], visited);
      }
    }
  }
  int ConnectivityComponentsCount() {
    int components_count = 0;
    std::vector<bool> visited(graph_.size(), false);
    for (size_t i = 1; i < graph_.size(); ++i) {
      if (visited[i] == false) {
        ++components_count;
        DFSVisit(i, visited);
      }
    }
    return components_count;
  }
};

int main() {
  int n;
  std::cin >> n;
  OrientedGraph graph(n);
  int a;
  for (int i = 1; i < n + 1; ++i) {
    std::cin >> a;
    graph.AddEdge(a, i);
    graph.AddEdge(i, a);
  }
  std::cout << graph.ConnectivityComponentsCount() << '\n';
  return 0;
}

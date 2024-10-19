#include <iostream>
#include <unordered_map>
#include <vector>

class pair_hash {  // NOLINT
 public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U>& x) const {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

class OrientedGraph {
  int vert_count_ = 0;
  std::vector<std::vector<int>> graph_;

 public:
  OrientedGraph() = default;
  explicit OrientedGraph(int n) : vert_count_(n), graph_(vert_count_ + 1) {
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
  OrientedGraph MakeLikeUnoriented() {
    OrientedGraph unoriented(graph_.size() - 1);
    for (size_t i = 1; i < graph_.size(); ++i) {
      for (auto neighbour : graph_[i]) {
        unoriented.AddEdge(i, neighbour);
        unoriented.AddEdge(neighbour, i);
      }
    }
    return unoriented;
  }
  int EulerCheck() {
    int start = 0;
    auto unoriented = MakeLikeUnoriented();
    std::vector<bool> visited(unoriented.graph_.size(), false);
    for (size_t i = 1; i < unoriented.graph_.size(); ++i) {
      if (!visited[i] && start != 0 && !unoriented.graph_[i].empty()) {
        return -1;
      }
      if (!visited[i] && !unoriented.graph_[i].empty()) {
        unoriented.DFSVisit(i, visited);
        start = i;
      }
    }
    return start;
  }
  void DFSEulerCycle(int cur, std::vector<int>& cycle, std::unordered_map<std::pair<int, int>, int, pair_hash>& visited,
                     std::vector<size_t>& idx_for_iter) {
    while (idx_for_iter[cur] < graph_[cur].size()) {
      int neighbour = graph_[cur][idx_for_iter[cur]];
      if (visited[std::pair<int, int>(cur, neighbour)]) {
        --visited[std::pair<int, int>(cur, neighbour)];
        ++idx_for_iter[cur];
        DFSEulerCycle(neighbour, cycle, visited, idx_for_iter);
      }
    }
    cycle.emplace_back(cur);
  }
  std::vector<int> EulerCycle() {
    int start = EulerCheck();
    if (start == -1) {
      return std::vector<int>();
    }
    std::unordered_map<std::pair<int, int>, int, pair_hash> visited;
    for (size_t i = start; i < graph_.size(); ++i) {
      for (auto neighbour : graph_[i]) {
        ++visited[std::pair<int, int>(i, neighbour)];
      }
    }
    std::vector<int> cycle;
    std::vector<size_t> idx_for_iter(graph_.size(), 0);
    DFSEulerCycle(start, cycle, visited, idx_for_iter);
    return std::vector<int>(cycle.rbegin(), cycle.rend());
  }
};

int main() {
  int n, m, k, a, b;
  std::cin >> m >> n;
  if (n < 1 || m < 1) {
    std::cout << 0;
    return 0;
  }
  OrientedGraph graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> k >> a;
    for (int j = 0; j < k; ++j) {
      std::cin >> b;
      graph.AddEdge(a, b);
      a = b;
    }
  }
  auto cycle = graph.EulerCycle();
  std::cout << cycle.size();
  for (auto i : cycle) {
    std::cout << ' ' << i;
  }
  return 0;
}

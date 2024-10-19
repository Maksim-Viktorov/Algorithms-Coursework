#include <iostream>
#include <vector>

struct OrientedGraph {
  std::vector<std::vector<int>> graph;
  OrientedGraph() = default;
  explicit OrientedGraph(int n) : graph(n + 1) {
  }
  OrientedGraph Transpose() const {
    OrientedGraph transposed(graph.size() - 1);
    for (size_t i = 1; i < graph.size(); ++i) {
      for (auto& neighbour : graph[i]) {
        transposed.graph[neighbour].emplace_back(i);
      }
    }
    return transposed;
  }
  bool TopSortDFS(int cur, std::vector<char>& color, std::vector<int>& top_sorted) const {
    color[cur] = 'g';
    for (auto& i : graph[cur]) {
      if (color[i] == 'g') {
        return false;
      }
      if (color[i] == 'w') {
        if (!TopSortDFS(i, color, top_sorted)) {
          return false;
        }
      }
    }
    color[cur] = 'b';
    top_sorted.emplace_back(cur);
    return true;
  }
  std::vector<int> TopSort() const {
    std::vector<char> color(graph.size(), 'w');
    std::vector<int> top_sorted;
    for (size_t i = 1; i < graph.size(); ++i) {
      if (color[i] == 'w') {
        if (!TopSortDFS(i, color, top_sorted)) {
          return std::vector<int>();
        }
      }
    }
    return std::vector<int>(top_sorted.rbegin(), top_sorted.rend());
  }
  bool TopSortNoCyclesDFS(int cur, std::vector<char>& color, std::vector<int>& top_sorted) const {
    color[cur] = 'g';
    for (auto& i : graph[cur]) {
      if (color[i] == 'w') {
        if (!TopSortNoCyclesDFS(i, color, top_sorted)) {
          return false;
        }
      }
    }
    color[cur] = 'b';
    top_sorted.emplace_back(cur);
    return true;
  }
  std::vector<int> TopSortNoCycles() const {
    std::vector<char> color(graph.size(), 'w');
    std::vector<int> top_sorted;
    for (size_t i = 1; i < graph.size(); ++i) {
      if (color[i] == 'w') {
        if (!TopSortNoCyclesDFS(i, color, top_sorted)) {
          return std::vector<int>();
        }
      }
    }
    return std::vector<int>(top_sorted.rbegin(), top_sorted.rend());
  }
  void DFSVisit(int cur, std::vector<int>& verticies, std::vector<bool>& visited) const {
    verticies.emplace_back(cur);
    visited[cur] = true;
    for (size_t i = 0; i < graph[cur].size(); ++i) {
      if (!visited[graph[cur][i]]) {
        DFSVisit(graph[cur][i], verticies, visited);
      }
    }
  }
  std::vector<std::vector<int>> DFS(std::vector<int> order) const {
    std::vector<bool> visited(order.size() + 1, false);
    std::vector<std::vector<int>> con_comp(1);
    for (auto& v : order) {
      if (!visited[v]) {
        con_comp.emplace_back();
        DFSVisit(v, con_comp[con_comp.size() - 1], visited);
      }
    }
    return con_comp;
  }
  std::vector<std::vector<int>> SCC() const {
    auto order = TopSortNoCycles();
    auto transposed = Transpose();
    auto con_comp = transposed.DFS(order);
    return con_comp;
  }
  std::pair<int, std::vector<int>> SortedSCC() const {
    auto con_comp = SCC();
    std::vector<int> enumerate(graph.size());
    for (size_t i = 1; i < con_comp.size(); ++i) {
      for (auto& v : con_comp[i]) {
        enumerate[v] = i;
      }
    }
    OrientedGraph unsorted_comp(con_comp.size() - 1);
    for (size_t i = 1; i < graph.size(); ++i) {
      for (auto& neighbour : graph[i]) {
        if (enumerate[i] != enumerate[neighbour]) {
          unsorted_comp.graph[enumerate[i]].emplace_back(enumerate[neighbour]);
        }
      }
    }
    auto sorted_comp = unsorted_comp.TopSort();
    for (size_t i = 0; i < sorted_comp.size(); ++i) {
      for (auto& v : con_comp[sorted_comp[i]]) {
        enumerate[v] = i + 1;
      }
    }
    int count = sorted_comp.size();
    return std::pair<int, std::vector<int>>(count, enumerate);
  }
};

int main() {
  int n, m;
  std::cin >> n >> m;
  OrientedGraph graph(n);
  int a, b;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b;
    graph.graph[a].emplace_back(b);
  }
  auto pair = graph.SortedSCC();
  auto sorted = std::move(pair.second);
  std::cout << pair.first << '\n';
  for (size_t i = 1; i < sorted.size(); ++i) {
    std::cout << sorted[i] << ' ';
  }
  return 0;
}

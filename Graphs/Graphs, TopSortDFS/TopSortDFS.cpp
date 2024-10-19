#include <iostream>
#include <vector>

struct OrientedGraph {
  std::vector<std::vector<int>> graph;
  OrientedGraph() = default;
  explicit OrientedGraph(int n) : graph(n + 1) {
  }
  bool TopSortDFS(int cur, std::vector<char>& color, std::vector<int>& top_sorted) {
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
  std::vector<int> TopSort() {
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
  auto top_sorted = graph.TopSort();
  if (top_sorted.empty()) {
    std::cout << -1;
  } else {
    for (auto& i : top_sorted) {
      std::cout << i << ' ';
    }
  }
  return 0;
}

#include <iostream>
#include <vector>

struct OrientedGraph {
  std::vector<std::vector<int>> graph;
  OrientedGraph() = default;
  explicit OrientedGraph(int n) : graph(n + 1) {
  }
  int CycleDFS(int cur, std::vector<char>& color, std::vector<int>& parent) {
    color[cur] = 'g';
    for (auto& i : graph[cur]) {
      if (color[i] == 'g') {
        parent[i] = cur;
        return i;
      }
      if (color[i] == 'w') {
        parent[i] = cur;
        int start = CycleDFS(i, color, parent);
        if (start != -1) {
          return start;
        }
      }
    }
    color[cur] = 'b';
    return -1;
  }
  std::vector<int> Cycle() {
    std::vector<char> color(graph.size(), 'w');
    std::vector<int> parent(graph.size());
    int start = -1;
    for (size_t i = 1; i < graph.size(); ++i) {
      if (color[i] == 'w') {
        parent[i] = i;
        start = CycleDFS(i, color, parent);
        if (start != -1) {
          break;
        }
      }
    }
    if (start == -1) {
      return std::vector<int>();
    }
    std::vector<int> cycle;
    cycle.emplace_back(start);
    int i = start;
    while (parent[i] != start) {
      cycle.emplace_back(parent[i]);
      i = parent[i];
    }
    cycle = std::vector<int>(cycle.rbegin(), cycle.rend());
    return cycle;
  }
};

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n;
  std::cin >> n;
  OrientedGraph graph(n);
  char color;
  for (int i = 1; i < n + 1; ++i) {
    for (int j = i + 1; j < n + 1; ++j) {
      std::cin >> color;
      if (color == 'R') {
        graph.graph[i].emplace_back(j);
      } else {
        graph.graph[j].emplace_back(i);
      }
    }
  }
  std::cout << (graph.Cycle().empty() ? "YES\n" : "NO\n");
  return 0;
}

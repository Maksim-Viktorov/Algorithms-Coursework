#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class UnorientedGraph {
  int vert_count_ = 0;
  std::vector<std::vector<std::pair<int, int>>> graph_;

public:
  UnorientedGraph() = default;
  explicit UnorientedGraph(int n) : vert_count_(n), graph_(vert_count_ + 1) {
  }
  void AddEdge(int start, int finish, int count) {
    graph_[start].emplace_back(finish, count);
  }
  void BridgesDFSVisit(int cur, int last_edge, std::vector<char>& colors, std::set<int>& bridges, int& time,
                       std::vector<int>& time_in, std::vector<int>& time_up) const {
    colors[cur] = 'g';
    time_in[cur] = time_up[cur] = ++time;
    for (auto neighbour : graph_[cur]) {
      if (colors[neighbour.first] == 'g' && neighbour.second != last_edge) {
        time_up[cur] = std::min(time_up[cur], time_in[neighbour.first]);
      }
      if (colors[neighbour.first] == 'w') {
        BridgesDFSVisit(neighbour.first, neighbour.second, colors, bridges, time, time_in, time_up);
        time_up[cur] = std::min(time_up[cur], time_up[neighbour.first]);
        if (time_in[cur] < time_up[neighbour.first]) {
          bridges.insert(neighbour.second);
        }
      }
    }
    colors[cur] = 'b';
  }
  std::set<int> Bridges() const {
    std::vector<char> colors(graph_.size(), 'w');
    std::vector<int> time_in(graph_.size(), vert_count_ * 2);
    std::vector<int> time_up(graph_.size(), vert_count_ * 2);
    std::set<int> bridges;
    int time = 0;
    for (size_t i = 1; i < graph_.size(); ++i) {
      if (colors[i] == 'w') {
        BridgesDFSVisit(i, 0, colors, bridges, time, time_in, time_up);
      }
    }
    return bridges;
  }
};

int main() {
  int n, m;
  std::cin >> n >> m;
  UnorientedGraph graph(n);
  int a, b;
  for (int i = 1; i < m + 1; ++i) {
    std::cin >> a >> b;
    if (a == b) {
      continue;
    }
    graph.AddEdge(a, b, i);
    graph.AddEdge(b, a, i);
  }
  auto bridges = graph.Bridges();
  std::cout << bridges.size() << '\n';
  for (auto edge : bridges) {
    std::cout << edge << '\n';
  }
  return 0;
}
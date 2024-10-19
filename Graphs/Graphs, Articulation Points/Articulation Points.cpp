#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

class UnorientedGraph {
  int vert_count_ = 0;
  std::vector<std::vector<int>> graph_;

 public:
  UnorientedGraph() = default;
  explicit UnorientedGraph(int n) : vert_count_(n), graph_(vert_count_ + 1) {
  }
  void AddEdge(int start, int finish) {
    graph_[start].emplace_back(finish);
  }
  void APDFSVisit(int cur, bool is_root, std::vector<char>& colors, std::set<int>& articulation_points, int& time,
                  std::vector<int>& time_in, std::vector<int>& time_up) const {
    colors[cur] = 'g';
    time_in[cur] = time_up[cur] = ++time;
    int n_children = 0;
    for (auto neighbour : graph_[cur]) {
      if (colors[neighbour] == 'g') {
        time_up[cur] = std::min(time_up[cur], time_in[neighbour]);
      }
      if (colors[neighbour] == 'w') {
        ++n_children;
        APDFSVisit(neighbour, false, colors, articulation_points, time, time_in, time_up);
        time_up[cur] = std::min(time_up[cur], time_up[neighbour]);
        if (!is_root && time_in[cur] <= time_up[neighbour]) {
          articulation_points.insert(cur);
        }
      }
    }
    if (is_root && n_children > 1) {
      articulation_points.insert(cur);
    }
    colors[cur] = 'b';
  }
  std::set<int> ArticulationPoints() const {
    std::vector<char> colors(graph_.size(), 'w');
    std::vector<int> time_in(graph_.size(), vert_count_ * 2);
    std::vector<int> time_up(graph_.size(), vert_count_ * 2);
    std::set<int> articulation_points;
    int time = 0;
    for (size_t i = 1; i < graph_.size(); ++i) {
      if (colors[i] == 'w') {
        APDFSVisit(i, true, colors, articulation_points, time, time_in, time_up);
      }
    }
    return articulation_points;
  }
};

int main() {
  int n, m;
  std::cin >> n >> m;
  UnorientedGraph graph(n);
  int a, b;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b;
    graph.AddEdge(a, b);
    graph.AddEdge(b, a);
  }
  auto articulation_points = graph.ArticulationPoints();
  std::cout << articulation_points.size() << '\n';
  for (auto point : articulation_points) {
    std::cout << point << '\n';
  }
  return 0;
}

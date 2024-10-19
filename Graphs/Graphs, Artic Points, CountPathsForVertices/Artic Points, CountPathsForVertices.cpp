#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

enum Color { WHITE, GRAY, BLACK };

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
  void APDFSVisit(int cur, bool is_root, std::vector<Color>& colors,
                  std::unordered_map<int, std::vector<int>>& articulation_points, int& time, std::vector<int>& time_in,
                  std::vector<int>& time_up, std::vector<int>& time_max) const {
    colors[cur] = GRAY;
    time_in[cur] = time_up[cur] = time_max[cur] = ++time;
    bool flag = false;
    int n_children = 0;
    for (auto neighbour : graph_[cur]) {
      if (colors[neighbour] == GRAY) {
        time_up[cur] = std::min(time_up[cur], time_in[neighbour]);
      }
      if (colors[neighbour] != WHITE) {
        if (time_in[cur] < time_in[neighbour]) {
          time_max[neighbour] = -1;
        }
      }
      if (colors[neighbour] == WHITE) {
        ++n_children;
        APDFSVisit(neighbour, false, colors, articulation_points, time, time_in, time_up, time_max);
        time_up[cur] = std::min(time_up[cur], time_up[neighbour]);
        time_max[cur] = std::max(time_max[cur], time_max[neighbour]);
        if (!is_root && time_in[cur] <= time_up[neighbour]) {
          flag = true;
        }
      }
    }
    if (articulation_points.find(cur) == articulation_points.end() && ((is_root && n_children > 1) || flag)) {
      std::vector<int> neighbour_count;
      int count = 0;
      for (auto neighbour : graph_[cur]) {
        if (time_in[cur] <= time_up[neighbour] && time_max[neighbour] != -1) {
          neighbour_count.emplace_back(time_max[neighbour] - time_in[neighbour] + 1);
          count += time_max[neighbour] - time_in[neighbour] + 1;
        }
      }
      neighbour_count.emplace_back(graph_.size() - count - 2);
      articulation_points[cur] = std::move(neighbour_count);
    }
    colors[cur] = BLACK;
  }
  std::unordered_map<int, std::vector<int>> ArticulationPoints() const {
    std::vector<Color> colors(graph_.size(), WHITE);
    std::vector<int> time_in(graph_.size(), vert_count_ * 2);
    std::vector<int> time_up(graph_.size(), vert_count_ * 2);
    std::vector<int> time_max(graph_.size(), -1);
    std::unordered_map<int, std::vector<int>> articulation_points;
    int time = 0;
    for (size_t i = 1; i < graph_.size(); ++i) {
      if (colors[i] == WHITE) {
        APDFSVisit(i, true, colors, articulation_points, time, time_in, time_up, time_max);
      }
    }
    return articulation_points;
  }
  std::vector<int> CountPathsForVertices() {
    auto articulation_points = ArticulationPoints();
    std::vector<int> path_count(graph_.size(), 0);
    for (auto pair : articulation_points) {
      int vert = pair.first;
      std::vector<int> neighbour_count = std::move(pair.second);
      int tmp_path_count = 0, tmp_count = 0;
      for (auto count : neighbour_count) {
        tmp_path_count += tmp_count * count;
        tmp_count += count;
      }
      path_count[vert] += tmp_path_count;
    }
    for (auto& count : path_count) {
      count += graph_.size() - 2;
    }
    return path_count;
  }
};

int main() {
  int n, m;
  std::cin >> n >> m;
  UnorientedGraph graph(n);
  int a, b;
  for (int i = 0; i < m; ++i) {
    std::cin >> a >> b;
    if (a == b) {
      continue;
    }
    graph.AddEdge(a, b);
    graph.AddEdge(b, a);
  }
  auto path_count = graph.CountPathsForVertices();
  for (size_t i = 1; i < path_count.size(); ++i) {
    std::cout << path_count[i] << '\n';
  }
  return 0;
}

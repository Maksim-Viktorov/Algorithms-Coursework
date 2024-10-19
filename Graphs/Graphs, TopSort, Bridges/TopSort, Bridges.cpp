#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

enum Color { WHITE, GRAY, BLACK };

class pair_hash {  // NOLINT

 public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U>& x) const {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

class OrientedGraph {
  std::vector<std::vector<int>> graph_;

 public:
  OrientedGraph() = default;
  explicit OrientedGraph(int n) : graph_(n + 1) {
  }
  void AddEdge(int start, int finish) {
    graph_[start].emplace_back(finish);
  }
  OrientedGraph Transpose() const {
    OrientedGraph transposed(graph_.size() - 1);
    for (size_t i = 1; i < graph_.size(); ++i) {
      for (auto& neighbour : graph_[i]) {
        transposed.graph_[neighbour].emplace_back(i);
      }
    }
    return transposed;
  }
  bool TopSortDFS(int cur, std::vector<Color>& color, std::vector<int>& top_sorted) const {
    color[cur] = GRAY;
    for (auto& i : graph_[cur]) {
      if (color[i] == GRAY) {
        return false;
      }
      if (color[i] == WHITE) {
        if (!TopSortDFS(i, color, top_sorted)) {
          return false;
        }
      }
    }
    color[cur] = BLACK;
    top_sorted.emplace_back(cur);
    return true;
  }
  std::vector<int> TopSort() const {
    std::vector<Color> color(graph_.size(), WHITE);
    std::vector<int> top_sorted;
    for (size_t i = 1; i < graph_.size(); ++i) {
      if (color[i] == WHITE) {
        if (!TopSortDFS(i, color, top_sorted)) {
          return std::vector<int>();
        }
      }
    }
    return std::vector<int>(top_sorted.rbegin(), top_sorted.rend());
  }
  bool TopSortNoCyclesDFS(int cur, std::vector<Color>& color, std::vector<int>& top_sorted) const {
    color[cur] = GRAY;
    for (auto& i : graph_[cur]) {
      if (color[i] == WHITE) {
        if (!TopSortNoCyclesDFS(i, color, top_sorted)) {
          return false;
        }
      }
    }
    color[cur] = BLACK;
    top_sorted.emplace_back(cur);
    return true;
  }
  std::vector<int> TopSortNoCycles() const {
    std::vector<Color> color(graph_.size(), WHITE);
    std::vector<int> top_sorted;
    for (size_t i = 1; i < graph_.size(); ++i) {
      if (color[i] == WHITE) {
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
    for (size_t i = 0; i < graph_[cur].size(); ++i) {
      if (!visited[graph_[cur][i]]) {
        DFSVisit(graph_[cur][i], verticies, visited);
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
  OrientedGraph Condensate() const {
    auto con_comp = SCC();
    std::vector<int> enumerate(graph_.size());
    for (size_t i = 1; i < con_comp.size(); ++i) {
      for (auto& v : con_comp[i]) {
        enumerate[v] = i;
      }
    }
    OrientedGraph unsorted_comp(con_comp.size() - 1);
    for (size_t i = 1; i < graph_.size(); ++i) {
      for (auto& neighbour : graph_[i]) {
        if (enumerate[i] != enumerate[neighbour]) {
          unsorted_comp.graph_[enumerate[i]].emplace_back(enumerate[neighbour]);
          unsorted_comp.graph_[enumerate[neighbour]].emplace_back(enumerate[i]);
        }
      }
    }
    return unsorted_comp;
  }
  void BridgesDFSVisit(int cur, int parent, std::vector<Color>& colors,
                       std::unordered_set<std::pair<int, int>, pair_hash>& bridges, int& time,
                       std::vector<int>& time_in, std::vector<int>& time_up) const {
    colors[cur] = GRAY;
    time_in[cur] = time_up[cur] = ++time;
    for (auto neighbour : graph_[cur]) {
      if (colors[neighbour] == GRAY && neighbour != parent) {
        time_up[cur] = std::min(time_up[cur], time_in[neighbour]);
      }
      if (colors[neighbour] == WHITE) {
        BridgesDFSVisit(neighbour, cur, colors, bridges, time, time_in, time_up);
        time_up[cur] = std::min(time_up[cur], time_up[neighbour]);
        if (time_in[cur] < time_up[neighbour]) {
          if (cur < neighbour) {
            bridges.emplace(cur, neighbour);
          } else {
            bridges.emplace(neighbour, cur);
          }
        }
      }
    }
    colors[cur] = BLACK;
  }
  std::unordered_set<std::pair<int, int>, pair_hash> Bridges() const {
    std::vector<Color> colors(graph_.size(), WHITE);
    std::vector<int> time_in(graph_.size(), graph_.size() * 2);
    std::vector<int> time_up(graph_.size(), graph_.size() * 2);
    std::unordered_set<std::pair<int, int>, pair_hash> bridges;
    int time = 0;
    for (size_t i = 1; i < graph_.size(); ++i) {
      if (colors[i] == WHITE) {
        BridgesDFSVisit(i, 0, colors, bridges, time, time_in, time_up);
      }
    }
    return bridges;
  }
  int CountAdditional() {
    auto bridges = Bridges();
    OrientedGraph to_cond(graph_.size() - 1);
    for (auto& pair : bridges) {
      to_cond.graph_[pair.first].emplace_back(pair.second);
    }
    for (size_t i = 1; i < graph_.size(); ++i) {
      for (auto neighbour : graph_[i]) {
        if (int(i) < neighbour) {
          if (bridges.find(std::pair<int, int>(i, neighbour)) == bridges.end()) {
            to_cond.AddEdge(i, neighbour);
            to_cond.AddEdge(neighbour, i);
          }
        } else if (bridges.find(std::pair<int, int>(neighbour, i)) == bridges.end()) {
          to_cond.AddEdge(i, neighbour);
          to_cond.AddEdge(neighbour, i);
        }
      }
    }
    auto cond = to_cond.Condensate();
    int additional = 0;
    for (size_t i = 1; i < cond.graph_.size(); ++i) {
      if (cond.graph_[i].size() == 1) {
        ++additional;
      }
    }
    return additional / 2 + additional % 2;
  }
};

int main() {
  int n, m, start, finish;
  std::cin >> n >> m;
  OrientedGraph graph(n);
  for (int i = 0; i < m; ++i) {
    std::cin >> start >> finish;
    graph.AddEdge(start, finish);
    graph.AddEdge(finish, start);
  }
  std::cout << graph.CountAdditional() << '\n';
  return 0;
}

#include <iostream>
#include <vector>

enum Color { WHITE, GRAY, BLACK };

class OrientedGraph {
  std::vector<std::pair<int, std::vector<int>>> graph_;

 public:
  OrientedGraph() = default;
  explicit OrientedGraph(int n) : graph_(n + 1) {
  }
  void AddEdge(int start, int finish) {
    graph_[start].second.emplace_back(finish);
  }
  void SetValue(int vert, int val) {
    graph_[vert].first = val;
  }
  int GetValue(int vert) {
    return graph_[vert].first;
  }
  OrientedGraph Transpose() const {
    OrientedGraph transposed(graph_.size() - 1);
    for (size_t i = 1; i < graph_.size(); ++i) {
      for (auto& neighbour : graph_[i].second) {
        transposed.graph_[neighbour].second.emplace_back(i);
      }
    }
    return transposed;
  }
  bool TopSortDFS(int cur, std::vector<Color>& color, std::vector<int>& top_sorted) const {
    color[cur] = GRAY;
    for (auto& i : graph_[cur].second) {
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
    for (auto& i : graph_[cur].second) {
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
    for (size_t i = 0; i < graph_[cur].second.size(); ++i) {
      if (!visited[graph_[cur].second[i]]) {
        DFSVisit(graph_[cur].second[i], verticies, visited);
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
      for (auto& neighbour : graph_[i].second) {
        if (enumerate[i] != enumerate[neighbour]) {
          unsorted_comp.graph_[enumerate[i]].second.emplace_back(enumerate[neighbour]);
        }
      }
    }
    return unsorted_comp;
  }
  int CountDrains() {
    OrientedGraph condensate = Condensate();
    int count = 0;
    for (size_t i = 1; i < condensate.graph_.size(); ++i) {
      if (condensate.graph_[i].second.empty()) {
        ++count;
      }
    }
    return count;
  }
};

int main() {
  int n, m, val;
  std::cin >> n >> m;
  OrientedGraph graph(n * m);
  for (int i = 1; i < n + 1; ++i) {
    for (int j = 1; j < m + 1; ++j) {
      std::cin >> val;
      graph.SetValue((i - 1) * m + j, val);
      if (j > 1) {
        if (graph.GetValue((i - 1) * m + j - 1) > val) {
          graph.AddEdge((i - 1) * m + j - 1, (i - 1) * m + j);
        } else if (graph.GetValue((i - 1) * m + j - 1) < val) {
          graph.AddEdge((i - 1) * m + j, (i - 1) * m + j - 1);
        } else {
          graph.AddEdge((i - 1) * m + j - 1, (i - 1) * m + j);
          graph.AddEdge((i - 1) * m + j, (i - 1) * m + j - 1);
        }
      }
      if (i > 1) {
        if (graph.GetValue((i - 2) * m + j) > val) {
          graph.AddEdge((i - 2) * m + j, (i - 1) * m + j);
        } else if (graph.GetValue((i - 2) * m + j) < val) {
          graph.AddEdge((i - 1) * m + j, (i - 2) * m + j);
        } else {
          graph.AddEdge((i - 2) * m + j, (i - 1) * m + j);
          graph.AddEdge((i - 1) * m + j, (i - 2) * m + j);
        }
      }
    }
  }
  std::cout << graph.CountDrains() << '\n';
  return 0;
}

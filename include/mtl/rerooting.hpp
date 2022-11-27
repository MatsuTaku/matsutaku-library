#pragma once
#include "graph.hpp"
#include <vector>

template <typename T, T (*F)(T, int), T (*merge)(T, T), T (*G)(T, int),
          T (*e)()>
struct ReRootingGraph : public Graph {
  std::vector<std::vector<T>> dp;
  using Graph::g;
  using Graph::n;

private:
  T dfs(int s, int p) {
    T m = e();
    for (int i = 0; i < (int)g[s].size(); i++) {
      int t = g[s][i];
      if (t == p)
        continue;
      dp[s][i] = dfs(t, s);
      m = merge(m, F(dp[s][i], t));
    }
    return G(m, s);
  }
  void dfs2(int s, int p, T prop) {
    for (int i = 0; i < (int)g[s].size(); i++) {
      if (g[s][i] == p) {
        dp[s][i] = prop;
        break;
      }
    }
    std::vector<T> cR(g[s].size(), e());
    for (int i = (int)g[s].size() - 2; i >= 0; i--)
      cR[i] = merge(cR[i + 1], F(dp[s][i + 1], g[s][i + 1]));
    T cL = e();
    for (int i = 0; i < (int)g[s].size(); i++) {
      if (g[s][i] != p) {
        dfs2(g[s][i], s, G(merge(cL, cR[i]), s));
      }
      cL = merge(cL, F(dp[s][i], g[s][i]));
    }
  }

public:
  ReRootingGraph(int n) : Graph(n), dp(n) {}

  void calc(int root = 0) {
    for (int i = 0; i < n; i++)
      dp[i].assign(g[i].size(), e());
    dfs(root, -1);
    dfs2(root, -1, e());
  }
};

template <typename W, typename T, T (*F)(T, int, W), T (*merge)(T, T),
          T (*G)(T, int), T (*e)()>
struct ReRootingNetwork : public Network<W> {
  using base = Network<W>;
  std::vector<std::vector<T>> dp;
  using base::g;
  using base::n;

private:
  T dfs(int s, int p) {
    T m = e();
    for (int i = 0; i < (int)g[s].size(); i++) {
      auto [t, w] = g[s][i];
      if (t == p)
        continue;
      dp[s][i] = dfs(t, s);
      m = merge(m, F(dp[s][i], t, w));
    }
    return G(m, s);
  }
  void dfs2(int s, int p, T prop) {
    for (int i = 0; i < (int)g[s].size(); i++) {
      if (g[s][i].t == p) {
        dp[s][i] = prop;
        break;
      }
    }
    std::vector<T> cR(g[s].size(), e());
    for (int i = (int)g[s].size() - 2; i >= 0; i--) {
      auto [t, w] = g[s][i + 1];
      cR[i] = merge(cR[i + 1], F(dp[s][i + 1], t, w));
    }
    T cL = e();
    for (int i = 0; i < (int)g[s].size(); i++) {
      auto [t, w] = g[s][i];
      if (t != p) {
        dfs2(t, s, G(merge(cL, cR[i]), s));
      }
      cL = merge(cL, F(dp[s][i], t, w));
    }
  }

public:
  ReRootingNetwork(int n) : base(n), dp(n) {}

  void calc(int root = 0) {
    for (int i = 0; i < n; i++)
      dp[i].assign(g[i].size(), e());
    dfs(root, -1);
    dfs2(root, -1, e());
  }
};
#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

#include "test2colors.h"

typedef std::vector<std::vector<int64_t>> vvi;

std::pair<vvi, vvi> permute_graph(int64_t n, vvi A, vvi B,
                                  std::vector<int64_t> p) {
  vvi newA, newB;

  for (int i = 0; i < n; i++) {
    newA.push_back(std::vector<int64_t>(n));
    newB.push_back(std::vector<int64_t>(n));
    for (int j = 0; j < n; j++) {
      newA[i][j] = A[p[i]][p[j]];
      newB[i][j] = B[p[i]][p[j]];
    }
  }

  return {newA, newB};
}

bool are_graphs_isomorphic(int64_t n, int64_t mask1, int64_t mask2) {
  vvi A, B, C, D;
  std::tie(A, B) = buildGraphByMask(n, mask1);
  std::tie(C, D) = buildGraphByMask(n, mask2);

  std::vector<int64_t> p(n);
  std::iota(p.begin(), p.end(), 0);

  do {
    vvi newA, newB;
    std::tie(newA, newB) = permute_graph(n, A, B, p);
    if (std::tie(newA, newB) == std::tie(C, D) ||
        std::tie(newA, newB) == std::tie(D, C)) {
      return true;
    }
  } while (next_permutation(p.begin(), p.end()));

  return false;
}

int main() {
  int mask, cnt;
  int N = 4; //Set this based on your graphs
  std::vector<int> masks;
  while (std::cin >> mask >> cnt) {
    //if (cnt == 10) { //Add this if-clause to consider only masks with specific scr-index
      bool flag = true;
      for (int64_t mask2 : masks) {
        if (are_graphs_isomorphic(N, mask, mask2)) {
          flag = false;
          break;
        }
      }
      if (flag) {
        masks.push_back(mask);
        std::cout << mask << '\n';
      } 
    //}
  }
}

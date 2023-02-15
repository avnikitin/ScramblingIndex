#include <algorithm>
#include <iostream>
#include <numeric>
#include <tuple>
#include <vector>

typedef std::vector <std::vector <int32_t>> vvi;

std::pair <vvi, vvi> build_graph_from_mask(int32_t n, int32_t mask) {
    vvi A(n), B(n);

    int pos = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i].push_back((mask >> pos) & 1);
            pos++;
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            B[i].push_back((mask >> pos) & 1);
            pos++;
        }
    }

    return {A, B};
}

std::pair <vvi, vvi> permute_graph(int32_t n, vvi A, vvi B, std::vector <int32_t> p) {
    vvi newA(n), newB(n);
    for (int i = 0; i < n; i++) {
        newA[i].resize(n);
        newB[i].resize(n);
        for (int j = 0; j < n; j++) {
            newA[i][j] = A[p[i]][p[j]];
            newB[i][j] = B[p[i]][p[j]];
        }
    }
    return {newA, newB};
}

bool are_graphs_isomorphic(int32_t n, int32_t mask1, int32_t mask2) {
    vvi A, B, C, D;
    std::tie(A, B) = build_graph_from_mask(n, mask1);
    std::tie(C, D) = build_graph_from_mask(n, mask2);

    std::vector <int32_t> p(n);
    std::iota(p.begin(), p.end(), 0);
    
    do {
        vvi newA, newB;
        std::tie(newA, newB) = permute_graph(n, A, B, p);
        if (std::tie(newA, newB) == std::tie(C, D) || std::tie(newA, newB) == std::tie(D, C)) {
            return true;
        }
    } while(next_permutation(p.begin(), p.end()));
    return false;
}

/*int main() {
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    std::vector <int32_t> masks;
    int32_t mask;
    while(std::cin >> mask) {
        bool flag = true;
        for (int32_t mask2: masks) {
            if (are_graphs_isomorphic(3, mask, mask2)) {
                flag = false;
                break;
            }
        }
        if (flag) {
            masks.push_back(mask);
            std::cout << mask << '\n';
        }
    }
    return 0;
}*/

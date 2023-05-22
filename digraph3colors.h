#pragma once
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#include "digraph2colors.h"
/*std::vector <int32_t> getOnesPositions(uint64_t x) {
	std::vector <int32_t> v;
	while (x > 0) {
		int32_t dist = _tzcnt_u64(x);
		v.push_back(dist);
		x -= ((uint64_t)1 << dist);
	}
	return v;
}*/

typedef std::vector <std::vector <int32_t>> vvi;
typedef std::vector <std::vector < std::vector <uint64_t>>> vvvll;

class diGraph3colors {
private:
	int n;
	vvi A, B, C;

	int32_t scr, h, l, w;
	//h steps in A, l steps in B, w steps in C

	vvi outNeighbour;

	bool checkIfEveryPairOfVerticesHasAnOutNeighbour(const vvvll& last, int _h, int _l) {
		int32_t successful_pairs = 0;
		for (int out_n = 0; out_n < n; out_n++) {
			std::vector <int32_t> bits = getOnesPositions(last[_h][_l][out_n]);
			for (int i = 0; i < bits.size(); i++) {
				for (int j = i + 1; j < bits.size(); j++) {
					successful_pairs += (outNeighbour[bits[i]][bits[j]] == -1);
					outNeighbour[bits[i]][bits[j]] = out_n;
				}
			}
		}
		return successful_pairs * 2 == n * (n - 1);
	}

	void calculateScramblingIndex() {
		if (n == 1) {
			scr = h = l = w = 0;
			return;
		}
		vvvll prev, last;
		std::vector <uint64_t> prev_00(n);
		for (int32_t i = 0; i < n; i++) {
			prev_00[i] = ((uint64_t)1 << i);
		}
		std::vector <std::vector <uint64_t>> prev_0;
		prev_0.push_back(prev_00);
		prev.push_back(prev_0);
		for (int32_t _scr = 1; _scr <= 150; _scr++) {//change _scr <= 100 to true
			for (int32_t _h = 0; _h <= _scr; _h++) {
				std::vector <std::vector <uint64_t>> new_row =
					std::vector <std::vector <uint64_t>> (_scr - _h + 1,
						std::vector<uint64_t>(n, 0));
				last.push_back(new_row);
				for (int32_t _l = 0; _l <= _scr - _h; _l++) {
					int32_t _w = _scr - _h - _l;
					for (int32_t v = 0; v < n; v++) {
						for (int32_t u = 0; u < n; u++) {
							if (_h > 0 && A[u][v]) {
								last[_h][_l][v] |= prev[_h - 1][_l][u];
							}
							if (_l > 0 && B[u][v]) {
								last[_h][_l][v] |= prev[_h][_l - 1][u];
							}
							if (_w > 0 && C[u][v]) {
								last[_h][_l][v] |= prev[_h][_l][u];
							}
						}
					}
					if (checkIfEveryPairOfVerticesHasAnOutNeighbour(last, _h, _l)) {
						scr = _scr;
						h = _h;
						l = _l;
						w = _w;
						for (int i = 0; i < n; i++) {
							for (int j = 0; j < i; j++) {
								outNeighbour[i][j] = outNeighbour[j][i];
								//because we only calculated right-upper part of table
							}
						}
						return;
					}
					else {
						std::fill(outNeighbour.begin(), outNeighbour.end(), std::vector <int32_t>(n, -1));
					}
				}
			}
			prev.clear();
			std::swap(prev, last);
		}
		scr = -1; // Answer not found
	}

public:
	diGraph3colors() {}
	diGraph3colors(const vvi& _A, const vvi& _B, const vvi& _C) {
		assert(_A.size() == _B.size() && _B.size() == _C.size());
		n = _A.size();
		for (int i = 0; i < n; i++) {
			assert(_A[i].size() == n && _B[i].size() == n && _C[i].size() == n);
			for (int j = 0; j < n; j++) {
				assert(0 <= _A[i][j] && _A[i][j] <= 1);
				assert(0 <= _B[i][j] && _B[i][j] <= 1);
				assert(0 <= _C[i][j] && _C[i][j] <= 1);
			}
		}
		A = _A;
		B = _B;
		C = _C;
		outNeighbour = vvi(n, std::vector <int32_t>(n, -1));

		calculateScramblingIndex();
	}

	int32_t get_scr() const {
		return scr;
	}

	int32_t get_h() const {
		return h;
	}

	int32_t get_l() const {
		return l;
	}

	int32_t get_w() const {
		return w;
	}

};

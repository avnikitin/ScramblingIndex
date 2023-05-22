#pragma once
#include <cassert>
#include <iostream>
#include <vector>
#include <string>

std::vector <int32_t> getOnesPositions(uint64_t x) {
	std::vector <int32_t> v;
	while (x > 0) {
		int32_t dist = __builtin_ctz(x);
		v.push_back(dist);
		x -= ((uint64_t)1 << dist);
	}
	return v;
}

typedef std::vector <std::vector <int32_t>> vvi;
typedef std::vector < std::vector <uint64_t>> vvll;

class diGraph2colors {
private:
	int32_t n;
	vvi A, B;

	int32_t scr, h, l;

	vvi outNeighbour;

	int32_t _MAX_BOUND;

	std::vector <std::vector <std::pair <int, int>>> loc_scr;

	void calculateScramblingIndex() {
		_MAX_BOUND = (3 * n * n * n + 2 * n * n - 2 * n + 1) / 2; //for n >= 5
		//std::cout << _MAX_BOUND << '\n';
		if (n == 1) {
			scr = h = l = 0;
			return;
		}
		vvll prev; // prev[h][v]
		vvll last;

		loc_scr.resize(n, std::vector <std::pair <int, int>> (n, {-1, -1}));

		std::vector <uint64_t> prev_0(n);
		for (int32_t i = 0; i < n; i++) {
			prev_0[i] = ((uint64_t)1 << i);
		}
		prev.push_back(prev_0);
		for (int32_t _scr = 1; _scr <= _MAX_BOUND; _scr++) {
			for (int32_t _h = 0; _h <= _scr; _h++) {
				int32_t _l = _scr - _h;
				last.push_back(std::vector <uint64_t>(n, 0));
				for (int32_t v = 0; v < n; v++) { // calculating last[_h][v]
					for (int32_t u = 0; u < n; u++) { // last edge might be u -> v
						if (_h > 0 && A[u][v]) {
							last[_h][v] |= prev[_h - 1][u];
						}
						if (_l > 0 && B[u][v]) {
							last[_h][v] |= prev[_h][u];
						}
					}
				}

				//checking if every pair of vertices has an outneigbour
				int32_t successful_pairs = 0;
				for (int out_n = 0; out_n < n; out_n++) {
					std::vector <int32_t> bits = getOnesPositions(last[_h][out_n]);
					for (int i = 0; i < bits.size(); i++) {
						for (int j = i + 1; j < bits.size(); j++) {
							successful_pairs += (outNeighbour[bits[i]][bits[j]] == -1);
							outNeighbour[bits[i]][bits[j]] = out_n;
							if (loc_scr[bits[i]][bits[j]] == std::make_pair(-1, -1)) {
								loc_scr[bits[i]][bits[j]] = {_h, _scr - _h};
								loc_scr[bits[j]][bits[i]] = {_h, _scr - _h};
							}
						}
					}
				}
				if (successful_pairs * 2 == n * (n - 1)) {
					scr = _scr;
					h = _h;
					l = _l;

					std::cout << "Scrambling Index Found! It's equal to " << scr << "=" << h << "+" << l << "\n";
					for (int v = 0; v < n; v++) {
						std::cout << "At vertex " << v + 1 << " collect these vertices: ";
						for (int u = 0; u < n; u++) {
							if ((1ll << u) & last[h][v]) {
								std::cout << u + 1 << ' ';
							}
						}
						std::cout << '\n';
						//std::cout << last[h][v] <<  '\n';
					}

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
			prev.clear();
			std::swap(prev, last);
		}
		scr = -1;
	}

	std::string separator;
	std::string first_row;
	std::int32_t _BLOCK_LEN = 3;
	std::string minus_block;
	std::string space_block;

	void add_spaces_until(std::string& s, const int n) const {
		while (s.length() < n) {
			s += ' ';
		}
	}

public:
	diGraph2colors() {}
	diGraph2colors(const vvi& _A, const vvi& _B) {
		assert(_A.size() == _B.size());
		for (int i = 0; i < _A.size(); i++) {
			assert(_A[i].size() == _A.size() && _B[i].size() == _A.size());
			for (int j = 0; j < _A[i].size(); j++) {
				assert(0 <= _A[i][j] && _A[i][j] <= 1);
				assert(0 <= _B[i][j] && _B[i][j] <= 1);
			}
		}
		n = _A.size();
		A = _A;
		B = _B;
		outNeighbour = vvi(n, std::vector <int32_t>(n, -1));

		calculateScramblingIndex();

		minus_block = "";
		space_block = "";
		for (int i = 0; i < _BLOCK_LEN; i++) {
			minus_block += "-";
			space_block += " ";
		}

		separator = "+";
		for (int i = 0; i <= n; i++) {
			separator += minus_block + "+";
		}

		first_row = "|" + space_block + "|";
		for (int i = 0; i < n; i++) {
			std::string cur_num = std::to_string(i);
			add_spaces_until(cur_num, _BLOCK_LEN);
			first_row += cur_num + "|";
		}
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

	vvi get_outneighbours() const {
		return outNeighbour;
	}

	void describe_local_scrambling() const {
		std::cout << "Scrambling Index is " << scr << '\n';
		std::cout << "Local Scrambling Index is:\n";
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (i == j) {
					std::cout << "0 ";
				} else {
					std::cout << loc_scr[i][j].first + loc_scr[i][j].second << ' ';
				}
			}
			std::cout << '\n';
		}
		std::cout << '\n';
	}
	
	void describe_outneighbours() const {
		std::cout << "Scrambling Index is " << scr << '\n';
		std::cout << "(h, l) values are " << h << ' ' << l << '\n';
		std::cout << "Table of common outneighbours:\n";
		std::cout << separator << '\n' << first_row << '\n' << separator << '\n';
		for (int i = 0; i < n; i++) {
			const std::vector <int32_t>& v = outNeighbour[i];
			std::cout << "|";
			std::string row_name = std::to_string(i);
			add_spaces_until(row_name, _BLOCK_LEN);
			std::cout << row_name << "|";
			for (int32_t x : v) {
				std::string out_n = std::to_string(x);
				add_spaces_until(out_n, _BLOCK_LEN);
				std::cout << out_n << '|';
			}
			std::cout << '\n';
			std::cout << separator << '\n';
		}
	}
};

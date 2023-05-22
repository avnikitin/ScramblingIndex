#pragma once
#include <utility>
#include <random>
#include <vector>
#include <chrono>

#include <stddef.h>

#include <thread>
#include <mutex>

#include "digraph2colors.h"

std::mt19937 gen(clock());
//chrono::high_resolution_clock::now().time_since_epoch().count()

typedef std::vector <std::vector <int32_t>> vvi;

const size_t THREAD_NUMBER = 32;

void print_arr(vvi A) {
	int n = A.size();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << A[i][j] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

std::pair <vvi, vvi> build_Wielandt_theorem_32(int32_t n, int blue_edge) {
	vvi A = vvi(n, std::vector<int32_t>(n, 0));
	vvi B = vvi(n, std::vector<int32_t>(n, 0));
	B[blue_edge][blue_edge + 1] = 1;
	for (int i = 0; i < n - 1; i++) {
		if (i != blue_edge) {
			A[i][i + 1] = 1;
		}
	}
	A[n - 1][0] = 1;
	A[n - 2][0] = 1;
	return std::make_pair(A, B);
}

std::pair <vvi, vvi> build_Wielandt_theorem_33(int32_t n) {
	vvi A = vvi(n, std::vector<int32_t>(n, 0));
	vvi B = vvi(n, std::vector<int32_t>(n, 0));
	B[n - 1][0] = B[n - 2][0] = 1;
	for (int i = 0; i < n - 1; i++) {
		A[i][i + 1] = 1;
	}
	return std::make_pair(A, B);
}

std::pair <vvi, vvi> build_Wielandt_theorem_34(int32_t n) {

	vvi A = vvi(n, std::vector<int32_t>(n, 0));
	vvi B = vvi(n, std::vector<int32_t>(n, 0));
	B[n - 2][0] = B[n - 2][n - 1] = 1;
	A[n - 1][0] = 1;
	for (int i = 0; i < n - 2; i++) {
		A[i][i + 1] = 1;
	}
	return std::make_pair(A, B);
}

std::pair<vvi, vvi> buildGraphByMask(int32_t n, uint64_t mask) {
	vvi A = vvi(n, std::vector<int32_t>(n, 0));
	vvi B = vvi(n, std::vector<int32_t>(n, 0));

	int pos = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = ((mask >> pos) & 1);
			pos++;
		}
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			B[i][j] = ((mask >> pos) & 1);
			pos++;
		}
	}
	return std::make_pair(A, B);
}

uint64_t buildMaskbyGraph(int32_t n, std::pair<vvi, vvi> gr) {
	vvi grs[2];
	grs[0] = gr.first;
	grs[1] = gr.second;
	uint64_t mask = 0;
	int cnt = 0;
	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (grs[k][i][j]) {
					uint64_t one = 1;
					mask += (one << cnt);
				}
				cnt++;
			}
		}
	}
	return mask;
}

void bruteforce2Colors(int32_t n) {
	int32_t bits = 2 * n * n; // two n*n matrices
	uint64_t max_mask = ((uint64_t)1 << bits);

	int32_t _CNT = 0;
	int32_t _CNT_PRINTED = 0;

	uint64_t best_mask = 0;
	int32_t best_scr = 0;

	std::mutex g_lock;
	std::vector<std::thread> workers;
	auto thread_cnt = max_mask / THREAD_NUMBER;
	for (size_t i = 0; i < THREAD_NUMBER; ++i) {
		workers.emplace_back([&](size_t from, size_t to){
			{
				std::lock_guard<std::mutex> lck(g_lock);
				std::cerr << from << ":" << to << ".Let's gooooo!\n";
			}
			for (uint64_t mask = from; mask < to; mask++) {
				vvi A;
				vvi B;
				std::tie(A, B) = buildGraphByMask(n, mask);
				bool flag = false;
				int32_t sumA = 0, sumB = 0;
				for (int i = 0; i < n; i++) {
					int32_t outA = 0;
					int32_t outB = 0;
					for (int j = 0; j < n; j++) {
						outA += A[i][j];
						outB += B[i][j];
					}
					flag |= (outA + outB == 0); // a vertex does not have outneighbor
					sumA += outA;
					sumB += outB;
				}
				if (flag || sumA == 0 || sumB == 0) {
					continue;
				}
				diGraph2colors dg(A, B);
				//if (dg.get_scr() >= n * n - 2 * n + 2) { //Add this if-clause to find only graphs whose scrambling-index is >= n^2-2n+2
					std::lock_guard<std::mutex> lck(g_lock);
					std::cout << mask << ' ' << dg.get_scr() << '\n' << std::flush;
				//}
				 if (best_scr < dg.get_scr()) {
				 	best_scr = dg.get_scr();
				 	best_mask = mask;
				 }
			}
		}, i * thread_cnt, (i + 1) * thread_cnt);
	}

	for (auto& t : workers) {
		t.join();
	}
}

std::pair<vvi, vvi> build_scrambling_graph(int n) {
	vvi A, B;
	A.resize(n, std::vector<int32_t>(n, 0));
	B.resize(n, std::vector<int32_t>(n, 0));

	for (int i = 0; i + 1 < n; i++) {
		A[i][i + 1] = 1;
	}
	A[n - 2][0] = 1;
	B[n - 2][n - 1] = 1;
	B[n - 1][0] = 1;
	
	return std::make_pair(A, B);
}

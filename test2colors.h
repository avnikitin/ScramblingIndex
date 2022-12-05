#pragma once

#include <utility>
#include <random>
#include <vector>
#include <chrono>

std::mt19937 gen(clock());
//chrono::high_resolution_clock::now().time_since_epoch().count()

typedef std::vector <std::vector <int32_t>> vvi;

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

std::pair<vvi, vvi> build_my_Wielandt(int32_t n) {
	vvi A = vvi(n, std::vector<int32_t>(n, 0));
	vvi B = vvi(n, std::vector<int32_t>(n, 0));

	for (int i = 0; i < n - 1; i++) {
		A[i][i + 1] = 1;
	}
	A[n - 1][0] = 1;

	for (int i = 0; i < n - 2; i++) {
		B[i][i + 1] = 1;
	}
	B[n - 2][0] = 1;

	return std::make_pair(A, B);
}

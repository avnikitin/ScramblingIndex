#pragma once

#include <tuple>
#include "digraph3colors.h"

std::tuple<vvi, vvi, vvi> buildWielandt3color(std::string mask) {
	int32_t cnt1 = 0, cnt2 = 0, cnt3 = 0;
	for (char c : mask) {
		cnt1 += c == '1';
		cnt2 += c == '2';
		cnt3 += c == '3';
	}
	assert(cnt1 + cnt2 + cnt3 == mask.length() && cnt1 > 0 && cnt2 > 0 && cnt3 > 0);
	int n = mask.length() - 1;
	vvi A(n, std::vector<int>(n, 0));
	vvi B(n, std::vector<int>(n, 0));
	vvi C(n, std::vector<int>(n, 0));
	for (int i = 0; i < n; i++) {
		int next = (i + 1) % n;
		if (mask[i] == '1') {
			A[i][next] = 1;
		}
		else if (mask[i] == '2') {
			B[i][next] = 1;
		}
		else {
			C[i][next] = 1;
		}
	}
	if (mask.back() == '1') {
		A[n - 2][0] = 1;
	}
	else if (mask.back() == '2') {
		B[n - 2][0] = 1;
	}
	else {
		C[n - 2][0] = 1;
	}
	return std::make_tuple(A, B, C);
}

std::string getNextMask(std::string mask) {
	int i = mask.length() - 1;
	while (i >= 0 && mask[i] == '3') {
		mask[i] = '1';
		i--;
	}
	if (i == -1) {
		return "-1";
	}
	mask[i]++;
	return mask;
}

std::string getNext3ColorsMask(std::string mask) {
	while (mask != "-1") {
		while (mask != "-1") {
			mask = getNextMask(mask);
			if (mask.find("1") != -1 && mask.find("2") != -1 && mask.find("3") != -1) {
				break;
			}
		}
		if (mask != "-1") {
			return mask;
		}
	}
	return "-1";
}

void bruteforceWielandtGraphs(int n, bool debug = false) { //set debug=true if you want to see results even when scrambling index is -1
	std::string mask(n + 1, '1');
	while ((mask = getNext3ColorsMask(mask)) != "-1") {
		if (mask[0] != '1') {
			break;
		}
		vvi A, B, C;
		std::tie(A, B, C) = buildWielandt3color(mask);
		diGraph3colors dg3(A, B, C);
		if (dg3.get_scr() != -1 || debug) {
			std::cout << mask << " : " << dg3.get_scr() << ' ' << dg3.get_h() << ' ' << dg3.get_l() << ' ' << dg3.get_w() << '\n';
		}
	}
}
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <iomanip>

#include "digraph.h"
#include "test.h"

typedef std::vector <std::vector <int32_t>> vvi;

int main() {
	/*
	THEOREM 3.2 CHECKER
	int n = 7;
	for (int bad_edge = 0; bad_edge < n - 2; bad_edge++) {
		vvi A, B;
		std::tie(A, B) = build_Wielandt_theorem_32(n, bad_edge);
		diGraph2 dg(A, B);
		std::cout << dg.get_scr() << ' ' << n * n - 2 * n + 1 - (bad_edge + 1) << '\n';
			
	}*/
	/* THEOREM 3.3 CHECKER
	for (int n = 5; n <= 10; n++) {
		vvi A, B;
		std::tie(A, B) = build_Wielandt_theorem_33(n);
		diGraph2 dg(A, B);
		std::cout << dg.get_scr() << ' ' << n * n - 2 * n + 1 << '\n';
	}*/

	/* THEOREM 3.4 CHECKER
	for (int n = 5; n <= 10; n++) {
		vvi A, B;
		std::tie(A, B) = build_Wielandt_theorem_34(n);
		diGraph2 dg(A, B);
		std::cout << dg.get_scr() << ' ' << n * n - 2 * n + 2 << '\n';
	}*/

	int n = 6;
	vvi A, B;
	std::tie(A, B) = build_Wielandt_theorem_34(n);
	diGraph2 dg(A, B);
	dg.describe();
}
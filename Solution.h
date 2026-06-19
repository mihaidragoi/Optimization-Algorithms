#pragma once

#include <vector>
#include <string>
#include <random>

class Solution {
private:
	static std::vector<std::vector<int>> distances;

	static std::vector<std::vector<int>> weights;
	static std::vector<std::vector<int>> values;
	static int capacity;

	static int problemType;

public:
	std::vector<int> data;

	Solution();
	Solution(const std::vector<int>&d);

	static void setTSP(const std::vector<std::vector<int>>& m);
	static void setKnapsack(const std::vector<std::vector<int>>& v, const std::vector<std::vector<int>>& w, int cap);

	double evaluate() const;

	std::vector<std::pair<Solution, std::string>> generateNeighbors() const;

	Solution perturb(int strength) const;
	Solution randomNeighbor() const;

	void print() const;

	bool operator==(const Solution& other) const;
};

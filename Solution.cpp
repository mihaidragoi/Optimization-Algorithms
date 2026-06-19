#include "Solution.h"
#include <iostream>

std::vector<std::vector<int>> Solution::distances;
std::vector<std::vector<int>> Solution::weights;
std::vector<std::vector<int>> Solution::values;
int Solution::capacity;
int Solution::problemType; 

Solution::Solution() 
{
	if(problemType == 0 && distances.size()>0)
	{
		data.resize(distances.size());
		for (size_t i = 0; i < data.size(); i++)
			data[i] = i;
	}
	else 
		if(problemType == 1 && values.size() > 0)
			data.resize(values[0].size(), 0);
}

Solution::Solution(const std::vector<int>& d) : data(d)
{
}

void Solution::setTSP(const std::vector<std::vector<int>>& m)
{
	distances = m;
	problemType = 0;
}

void Solution::setKnapsack(const std::vector<std::vector<int>>& v, const std::vector<std::vector<int>>& w, int cap)
{
	values = v;
	weights = w;
	capacity = cap;
	problemType = 1;
}

double Solution::evaluate() const
{
	if (problemType == 0)
	{
		double totalDistance = 0.0;
		for (size_t i = 0; i < data.size() - 1; i++) 
		{
			int from = data[i];
			int to = data[i + 1];
			totalDistance += distances[from][to];
		}
		totalDistance += distances[data.back()][data[0]];
		return totalDistance;
	}
	else 
		if (problemType == 1)
		{
			double totalValue = 0.0;
			double totalWeight = 0.0;
			for (size_t i = 0; i < data.size(); i++)
			{
				if (data[i] == 1)
				{
					totalValue += values[0][i];
					totalWeight += weights[0][i];
				}
			}
			if (totalWeight > capacity)
			{
				return 1e9;
			}
			return -totalValue;
		}
	return 1e9;
}

std::vector<std::pair<Solution, std::string>> Solution::generateNeighbors() const
{
	if(problemType == 0)
	{
		std::vector<std::pair<Solution, std::string>> neighbors;
		for (size_t i = 1; i < data.size() - 1; i++)
		{
			for (size_t j = i + 1; j < data.size(); j++)
			{
				Solution neighbor = *this;
				std::swap(neighbor.data[i], neighbor.data[j]);
				neighbors.emplace_back(neighbor, "Swap" + std::to_string(i) + "-" + std::to_string(j));
			}
		}
		return neighbors;
	}
	else 
		if(problemType == 1)
	{
		std::vector<std::pair<Solution, std::string>> neighbors;
		for (size_t i = 0; i < data.size(); i++)
		{
			Solution neighbor = *this;
			neighbor.data[i] = 1 - neighbor.data[i];
			neighbors.emplace_back(neighbor, "Flip" + std::to_string(i));
		}
		return neighbors;
	}
}

Solution Solution::randomNeighbor() const
{
	if (data.empty()) return *this; 

	if (problemType == 0) 
	{
		Solution neighbor = *this;
		int n = data.size();

		int i = rand() % (n - 1) + 1;
		int j = rand() % (n - 1) + 1;

		while (i == j && n > 1) 
			j = rand() % (n - 1) + 1;

		std::swap(neighbor.data[i], neighbor.data[j]);
		return neighbor;
	}
	else 
		if (problemType == 1) 
	{
		Solution neighbor = *this;
		size_t i = rand() % data.size();
		neighbor.data[i] = 1 - neighbor.data[i];
		return neighbor;
	}
	return *this;
}

Solution Solution::perturb(int strength) const
{
	Solution s = *this;
	for (int i = 0; i < strength; i++)
		s = s.randomNeighbor();
	return s;
}

void Solution::print() const
{
	if (problemType == 0)
	{
		for (int city : data)
		{
			std::cout << city << " ";
		}
		std::cout << std::endl;
	}
	else
		if (problemType == 1)
		{
			for (int item : data)
			{
				std::cout << item << " ";
			}
			std::cout << std::endl;
		}
}

bool Solution::operator==(const Solution& other) const 
{
	return this->data == other.data;
}

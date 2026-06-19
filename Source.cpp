#include "Solution.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <sstream> 
#include <ctime>   
#include <algorithm> 
#include <map>
#include <numeric> 
#include <iomanip> 

Solution HillClimbingSteepest(const Solution& initial)
{
	Solution current = initial;
	bool improved = true;

	while (improved)
	{
		improved = false;
		auto neighbors = current.generateNeighbors();
		Solution bestNeighbor = current;

		for (const auto& n : neighbors)
		{
			if (n.first.evaluate() < bestNeighbor.evaluate())
			{
				bestNeighbor = n.first;
				improved = true;
			}
		}
		if (improved) current = bestNeighbor;
	}
	return current;
}

Solution TabuSearch(const Solution& initial, int tabuSize, int maxIterations)
{
	Solution current = initial;
	Solution bestSoFar = initial;
	std::map<std::string, int> tabuMap; 

	for (int iter = 0; iter < maxIterations; iter++)
	{
		auto neighbors = current.generateNeighbors();
		Solution bestAdmissibleNeighbor;
		std::string chosenMove = "";
		double bestEval = 1e18;
		bool foundAny = false;

		for (auto& n : neighbors)
		{
			bool isTabu = (tabuMap.find(n.second) != tabuMap.end());

			if (!isTabu || n.first.evaluate() < bestSoFar.evaluate())
			{
				if (n.first.evaluate() < bestEval)
				{
					bestEval = n.first.evaluate();
					bestAdmissibleNeighbor = n.first;
					chosenMove = n.second;
					foundAny = true;
				}
			}
		}

		if (!foundAny) break;

		current = bestAdmissibleNeighbor;
		if (current.evaluate() < bestSoFar.evaluate())
			bestSoFar = current;

		for (auto it = tabuMap.begin(); it != tabuMap.end();) 
		{
			it->second--;
			if (it->second <= 0) it = tabuMap.erase(it);
			else ++it;
		}

		if (!chosenMove.empty())
			tabuMap[chosenMove] = tabuSize;
	}
	return bestSoFar;
}

Solution SimulatedAnnealing(const Solution& initial, double initialTemp, double coolingRate, int maxIterations) 
{
	Solution current = initial;
	double currentEval = current.evaluate();
	double temp = initialTemp;
	for (int iter = 0; iter < maxIterations; iter++) 
	{
		Solution neighbor = current.randomNeighbor();
		double neighborEval = neighbor.evaluate();
		if (neighborEval < currentEval || (temp > 1e-9 && exp((currentEval - neighborEval) / temp) > ((double)rand() / RAND_MAX))) 
		{
			current = neighbor;
			currentEval = neighborEval;
		}
		temp *= coolingRate;
	}
	return current;
}

Solution IteratedLocalSearch(const Solution& initial, int perturbStrength, int maxIterations)
{
	Solution current = HillClimbingSteepest(initial); 
	Solution bestGlobal = current;

	for (int iter = 0; iter < maxIterations; iter++)
	{
		Solution perturbed = current.perturb(perturbStrength);
		Solution localOpt = HillClimbingSteepest(perturbed);

		if (localOpt.evaluate() < current.evaluate())
			current = localOpt;

		if (current.evaluate() < bestGlobal.evaluate())
			bestGlobal = current;
	}
	return bestGlobal;
}

void LoadTSPData(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cerr << "Eroare: Nu s-a putut deschide fisierul TSP: " << filename << std::endl;
		return;
	}

	std::string line;
	int dimension = 0;
	bool coordSection = false;

	struct Point { double x, y; };
	std::vector<Point> points;

	while (std::getline(file, line)) 
	{
		if (line.find("DIMENSION") != std::string::npos) 
		{
			size_t pos = line.find(":");
			if (pos == std::string::npos) pos = line.find_last_of(" ");
			std::stringstream ss(line.substr(pos + 1));
			ss >> dimension;
		}
		else 
			if (line.find("NODE_COORD_SECTION") != std::string::npos) 
			{
			coordSection = true;
			continue;
			}
		else 
			if (line.find("EOF") != std::string::npos) break;

		if (coordSection && !line.empty()) 
		{
			int id;
			double x, y;
			std::stringstream ss(line);
			if (ss >> id >> x >> y) 
				points.push_back({ x, y });
		}
	}

	std::vector<std::vector<int>> matrix(dimension, std::vector<int>(dimension));
	for (int i = 0; i < dimension; ++i) 
	{
		for (int j = 0; j < dimension; ++j) 
		{
			double dx = points[i].x - points[j].x;
			double dy = points[i].y - points[j].y;
			matrix[i][j] = static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy)));
		}
	}

	Solution::setTSP(matrix);
	std::cout << "Instanta TSP incarcata: " << dimension << " orase." << std::endl;
}

void LoadKnapsackData(const std::string& filename)
{
	std::ifstream file(filename);
	if (!file.is_open()) 
	{
		std::cerr << "Eroare: Nu s-a putut deschide fisierul Knapsack: " << filename << std::endl;
		return;
	}

	int n, W;
	if (!(file >> n >> W)) return;

	std::vector<std::vector<int>> v(1, std::vector<int>(n));
	std::vector<std::vector<int>> w(1, std::vector<int>(n));

	for (int i = 0; i < n; ++i)
		file >> v[0][i] >> w[0][i];

	Solution::setKnapsack(v, w, W);
	std::cout << "Instanta Knapsack incarcata: " << n << " obiecte, Capacitate: " << W << std::endl;
}

void RunExperiments(const std::string& instanceName, int numRuns)
{
	std::string outputFileName = "rezultate_" + instanceName + ".txt";
	std::ofstream outFile(outputFileName);

	if (!outFile.is_open()) 
	{
		std::cerr << "Eroare la crearea fisierului de rezultate!" << std::endl;
		return;
	}

	struct Stats 
	{
		std::vector<double> results;
		std::string name;
	};

	Stats hc = { {}, "Hill Climbing" };
	Stats ts = { {}, "Tabu Search" };
	Stats sa = { {}, "Simulated Annealing" };
	Stats ils = { {}, "Iterated Local Search" };

	std::cout << "\nIncepere experimente pentru: " << instanceName << " (" << numRuns << " rulari)...\n";

	for (int i = 0; i < numRuns; ++i)
	{
		std::cout << "Rularea " << i + 1 << "/" << numRuns << "...\r";
		std::cout.flush();

		Solution initial; 

		hc.results.push_back(HillClimbingSteepest(initial).evaluate());
		ts.results.push_back(TabuSearch(initial, 15, 200).evaluate());
		sa.results.push_back(SimulatedAnnealing(initial, 1000.0, 0.95, 1000).evaluate());
		ils.results.push_back(IteratedLocalSearch(initial, 5, 50).evaluate());
	}

	auto writeStats = [&](const Stats& s) 
		{
		double best = *std::min_element(s.results.begin(), s.results.end());
		double sum = std::accumulate(s.results.begin(), s.results.end(), 0.0);
		double mean = sum / s.results.size();

		outFile << "Algoritm: " << s.name << "\n";
		outFile << "---------------------------\n";
		outFile << "Best: " << std::fixed << std::setprecision(2) << best << "\n";
		outFile << "Mean: " << std::fixed << std::setprecision(2) << mean << "\n";
		outFile << "Toate rularile: ";
		for (double val : s.results) outFile << val << " ";
		outFile << "\n\n";
		};

	outFile << "Experimente pentru instanta: " << instanceName << "\n";
	outFile << "Data: " << time(NULL) << "\n\n";

	writeStats(hc);
	writeStats(ts);
	writeStats(sa);
	writeStats(ils);

	outFile.close();
	std::cout << "\nExperiment finalizat! Rezultate salvate in: " << outputFileName << std::endl;
}

int main()
{
	srand((unsigned int)time(NULL));

	LoadTSPData("xqf131.tsp");
	RunExperiments("xqf131_tsp", 30);

	LoadKnapsackData("s001.kp");
	RunExperiments("s001_knapsack", 30);

	return 0;
}

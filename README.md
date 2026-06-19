# Local Optimization Algorithms 🧬

## 📖 Overview
This repository contains a C++ implementation of various local optimization algorithms developed for a Computer Science laboratory focusing on heuristic search techniques. The project tests the performance of multiple algorithms against classic NP-hard computational problems, specifically the Traveling Salesman Problem (TSP) and the Knapsack Problem.

## 🧠 Implemented Algorithms
The core of the project relies on the following local search metaheuristics:
*   **Hill Climbing**
*   **Tabu Search**
*   **Simulated Annealing**
*   **Iterated Local Search (ILS)**

## 🎯 Target Problems & Encoding
The generic `Solution` class is built around a `std::vector<int>` to adapt to different problem constraints:

### 1. Traveling Salesman Problem (TSP)
*   **Encoding:** Uses permutations of city indices, where the first city (index 0) is kept fixed to avoid redundant rotations.
*   **Neighborhood Generation:** Utilizes a **Swap Operator** to exchange the positions of two cities within the permutation.
*   **Evaluation:** Minimizes the total distance between consecutively visited cities, including the return to the starting point.

### 2. Knapsack Problem
*   **Encoding:** Utilizes binary encoding where `0` means an item is excluded and `1` means it is included.
*   **Neighborhood Generation:** Uses a **Bit-Flip Operator** to invert the state of a specific item.
*   **Evaluation:** Designed as a minimization function by returning the negative profit (`-Profit`). A massive penalty (1e9) is applied to strictly discourage invalid solutions that exceed the maximum weight capacity.

## ⚙️ Advanced Heuristic Features
*   **Tabu Memory & Aspiration:** The Tabu Search algorithm uses a short-term memory structure (`std::map<std::string, int>`) to track "tabu tenure" and prevent cyclical evaluations. An aspiration criterion is implemented to override the tabu status if a move yields a result better than the current `bestSoFar`.
*   **Controlled Perturbation:** The Iterated Local Search implementation escapes local optima via a controlled Random Walk (`perturb(strength)`), temporarily allowing moves that worsen the solution to explore new areas of the search space.

## 📂 Project Structure & Usage
*   `Source.cpp` & `Solution.cpp` / `.h`: Contain the main execution logic and the object-oriented implementation of the algorithms and solution encodings.
*   **Datasets:** Includes standard instance files like `xqf131.tsp` (131 cities) and `s001.kp` (50 items) for benchmarking.
*   **Results:** The program runs multiple experiments (e.g., 30 runs per algorithm) and logs the best, mean, and individual results into dedicated text files like `rezultate_xqf131_tsp.txt` and `rezultate_s001_knapsack.txt`.

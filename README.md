# Profit-oriented hub line location problem 

## Overview
In this project, I consider a profit-oriented hub line problem in which the decisions are to determine the location of p hubs facilities connected through a path is found. The algorithms implemented include:
1. **Greedy Algorithm**
2. **GRASP (Greedy Randomized Adaptive Search Procedure)**
3. **Local Search Techniques**
4. **Floyd-Warshall Algorithm** (for finding shortest paths in a weighted graph)

These methods were applied to different datasets to solve problems related to hub location problems.

## Key Algorithms
- **Greedy Algorithm**: A simple but effective algorithm for building solutions step by step by choosing the locally optimal choice at each step.
- **GRASP**: A metaheuristic that constructs a solution by combining greedy algorithms with randomized elements.
- **Local Search**: A method for refining an initial solution by exploring its neighborhood.
- **Floyd-Warshall Algorithm**: Used to find all pairs shortest paths in a weighted graph.

## Project Structure
- **/code**: Contains all the code for implementing the algorithms.
  - `floydWarshall.c`: Implementation of Floyd-Warshall algorithm.
  - `const_greedy.c`: Greedy algorithm for cost minimization.
  - `grasp.c`: GRASP implementation.
  - `localSearch.c`: Local search algorithms for optimizing initial solutions.
- **/data**: Contains various datasets used for testing the algorithms.
  - `data1.tsv`, `data2.tsv`, etc.: Data files for testing.
  
## Setup and Usage
To run the project, follow these steps:

1. **Clone the repository**:
   ```bash
   git clone https://github.com/brendadenisse16/Discrete-Optimization.git
 

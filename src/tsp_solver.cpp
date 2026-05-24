#include "tsp_solver.h"
#include <algorithm>
#include <cmath>

TSPSolver::Result TSPSolver::solve(const std::vector<std::vector<double>>& adjacencyMatrix)
{
    Result result;
    int n = static_cast<int>(adjacencyMatrix.size());
    
    if (n < 2) {
        return result;
    }
    
    // Check if graph is complete enough for TSP
    bool hasSolution = true;
    for (int i = 0; i < n && hasSolution; ++i) {
        for (int j = 0; j < n && hasSolution; ++j) {
            if (i != j && adjacencyMatrix[i][j] >= INF) {
                // Check if there's at least one path
                bool hasPath = false;
                for (int k = 0; k < n; ++k) {
                    if (k != i && adjacencyMatrix[i][k] < INF && 
                        k != j && adjacencyMatrix[k][j] < INF) {
                        hasPath = true;
                        break;
                    }
                }
                if (!hasPath) {
                    hasSolution = false;
                }
            }
        }
    }
    
    if (!hasSolution) {
        return result;
    }
    
    double bestCost = INF;
    std::vector<int> bestPath;
    std::vector<int> path;
    int nodesExplored = 0;
    
    // Start from node 0 (arbitrary choice for symmetric TSP)
    path.push_back(0);
    int visited = 1; // Bitmask with bit 0 set
    
    // Calculate initial lower bound
    auto matrixCopy = adjacencyMatrix;
    double reductionCost = 0;
    auto reducedMatrix = reduceMatrix(matrixCopy, reductionCost);
    
    branchAndBound(adjacencyMatrix, path, visited, reductionCost, 
                   bestCost, bestPath, nodesExplored);
    
    if (bestCost < INF) {
        result.path = bestPath;
        result.totalCost = bestCost;
        result.found = true;
    }
    
    result.nodesExplored = nodesExplored;
    return result;
}

std::vector<std::vector<double>> TSPSolver::reduceMatrix(
    std::vector<std::vector<double>> matrix, 
    double& reductionCost)
{
    int n = static_cast<int>(matrix.size());
    reductionCost = 0;
    
    // Row reduction
    for (int i = 0; i < n; ++i) {
        double minVal = INF;
        for (int j = 0; j < n; ++j) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
        
        if (minVal < INF && minVal > 0) {
            reductionCost += minVal;
            for (int j = 0; j < n; ++j) {
                if (matrix[i][j] < INF) {
                    matrix[i][j] -= minVal;
                }
            }
        }
    }
    
    // Column reduction
    for (int j = 0; j < n; ++j) {
        double minVal = INF;
        for (int i = 0; i < n; ++i) {
            if (matrix[i][j] < minVal) {
                minVal = matrix[i][j];
            }
        }
        
        if (minVal < INF && minVal > 0) {
            reductionCost += minVal;
            for (int i = 0; i < n; ++i) {
                if (matrix[i][j] < INF) {
                    matrix[i][j] -= minVal;
                }
            }
        }
    }
    
    return matrix;
}

double TSPSolver::calculateLowerBound(std::vector<std::vector<double>> matrix,
                                       const std::vector<int>& path)
{
    double bound = 0;
    int n = static_cast<int>(matrix.size());
    
    // Mark visited rows and columns as infinity
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int from = path[i];
        int to = path[i + 1];
        
        // Set row 'from' to INF (can't leave again)
        for (int j = 0; j < n; ++j) {
            matrix[from][j] = INF;
        }
        
        // Set column 'to' to INF (can't enter again)
        for (int i = 0; i < n; ++i) {
            matrix[i][to] = INF;
        }
        
        // Set edge back to start as INF (prevent early cycles)
        matrix[to][path[0]] = INF;
    }
    
    // Add the cost of edges in the path
    for (size_t i = 0; i < path.size() - 1; ++i) {
        bound += matrix[path[i]][path[i + 1]];
    }
    
    // Reduce remaining matrix
    double reductionCost;
    reduceMatrix(matrix, reductionCost);
    bound += reductionCost;
    
    return bound;
}

double TSPSolver::calculateMinimumCompletion(
    const std::vector<std::vector<double>>& matrix,
    int visited,
    int current,
    int n)
{
    double minCost = 0;
    
    // For each unvisited city, find minimum incoming and outgoing edges
    for (int i = 0; i < n; ++i) {
        if (!(visited & (1 << i))) {
            double minIn = INF, minOut = INF;
            
            for (int j = 0; j < n; ++j) {
                if (j != i) {
                    if (matrix[j][i] < minIn) minIn = matrix[j][i];
                    if (matrix[i][j] < minOut) minOut = matrix[i][j];
                }
            }
            
            if (minIn < INF && minOut < INF) {
                minCost += (minIn + minOut) / 2.0;
            }
        }
    }
    
    return minCost;
}

void TSPSolver::branchAndBound(
    const std::vector<std::vector<double>>& matrix,
    std::vector<int>& path,
    int visited,
    double currentBound,
    double& bestCost,
    std::vector<int>& bestPath,
    int& nodesExplored)
{
    nodesExplored++;
    int n = static_cast<int>(matrix.size());
    
    // If all cities visited, check if we can return to start
    if (static_cast<int>(path.size()) == n) {
        double finalCost = currentBound;
        
        // Add cost to return to start
        int lastCity = path.back();
        int firstCity = path.front();
        if (matrix[lastCity][firstCity] < INF) {
            finalCost += matrix[lastCity][firstCity];
        } else {
            return; // No valid tour
        }
        
        if (finalCost < bestCost) {
            bestCost = finalCost;
            bestPath = path;
        }
        return;
    }
    
    int currentCity = path.back();
    
    // Try all unvisited cities
    for (int nextCity = 0; nextCity < n; ++nextCity) {
        if (!(visited & (1 << nextCity)) && matrix[currentCity][nextCity] < INF) {
            // Calculate new bound
            double newBound = currentBound + matrix[currentCity][nextCity];
            
            // Pruning: skip if bound exceeds best known solution
            if (newBound >= bestCost) {
                continue;
            }
            
            // Add heuristic estimate for remaining cities
            double minCompletion = calculateMinimumCompletion(matrix, visited | (1 << nextCity), 
                                                               nextCity, n);
            if (newBound + minCompletion >= bestCost) {
                continue;
            }
            
            // Make the move
            path.push_back(nextCity);
            int newVisited = visited | (1 << nextCity);
            
            // Recurse
            branchAndBound(matrix, path, newVisited, newBound, 
                          bestCost, bestPath, nodesExplored);
            
            // Backtrack
            path.pop_back();
        }
    }
}

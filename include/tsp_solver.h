#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include <vector>
#include <limits>
#include <utility>

/**
 * @class TSPSolver
 * @brief Implements the Branch and Bound algorithm for solving the Traveling Salesman Problem.
 * 
 * This class provides a complete implementation of the Branch and Bound method
 * for finding the optimal Hamiltonian cycle (minimum cost tour) in a weighted graph.
 */
class TSPSolver
{
public:
    /**
     * @brief Result structure containing the solution details
     */
    struct Result {
        std::vector<int> path;          ///< Optimal path (sequence of node indices)
        double totalCost;               ///< Total cost of the tour
        bool found;                     ///< Whether a solution was found
        int nodesExplored;              ///< Number of nodes explored in the search tree
        
        Result() : totalCost(0), found(false), nodesExplored(0) {}
    };
    
    /**
     * @brief Solve the TSP using Branch and Bound algorithm
     * @param adjacencyMatrix The cost/distance matrix (INF for no connection)
     * @return Result The optimal solution
     */
    Result solve(const std::vector<std::vector<double>>& adjacencyMatrix);
    
    /**
     * @brief Get the lower bound for a partial solution
     * @param matrix Reduced cost matrix
     * @param path Current partial path
     * @return double Lower bound estimate
     */
    double calculateLowerBound(std::vector<std::vector<double>> matrix, 
                               const std::vector<int>& path);
    
private:
    static constexpr double INF = 1e9;   ///< Infinity value
    
    /**
     * @brief Reduce the cost matrix by subtracting minimum values from rows and columns
     * @param matrix The matrix to reduce
     * @param reductionCost Reference to store the total reduction cost
     * @return std::vector<std::vector<double>> The reduced matrix
     */
    std::vector<std::vector<double>> reduceMatrix(
        std::vector<std::vector<double>> matrix, 
        double& reductionCost);
    
    /**
     * @brief Calculate the minimum cost to complete a tour from current state
     * @param matrix The cost matrix
     * @param visited Bitmask of visited cities
     * @param current Current city index
     * @param n Number of cities
     * @return double Minimum completion cost
     */
    double calculateMinimumCompletion(
        const std::vector<std::vector<double>>& matrix,
        int visited,
        int current,
        int n);
    
    /**
     * @brief Internal recursive branch and bound function
     * @param matrix Original cost matrix
     * @param path Current path being explored
     * @param visited Bitmask of visited cities
     * @param currentBound Current lower bound
     * @param bestCost Reference to best cost found so far
     * @param bestPath Reference to best path found so far
     * @param nodesExplored Reference to counter of explored nodes
     */
    void branchAndBound(
        const std::vector<std::vector<double>>& matrix,
        std::vector<int>& path,
        int visited,
        double currentBound,
        double& bestCost,
        std::vector<int>& bestPath,
        int& nodesExplored);
};

#endif // TSP_SOLVER_H

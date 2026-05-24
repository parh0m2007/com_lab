#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <map>
#include <memory>
#include "node.h"
#include "edge.h"

/**
 * @class Graph
 * @brief Represents the complete graph structure for the TSP problem.
 * 
 * This class manages all nodes and edges in the graph, providing methods
 * for adding, removing, and querying graph elements. It supports both
 * directed and undirected graphs (default is undirected/bidirectional).
 */
class Graph
{
public:
    /**
     * @brief Construct a new Graph object
     * @param directed Whether the graph is directed (default: false for undirected)
     */
    explicit Graph(bool directed = false);
    
    /**
     * @brief Add a new node to the graph
     * @param pos Position of the new node
     * @param label Optional label for the node
     * @return int The ID of the newly created node
     */
    int addNode(const QPointF& pos, const QString& label = QString());
    
    /**
     * @brief Remove a node and all connected edges
     * @param nodeId ID of the node to remove
     * @return true if the node was found and removed
     */
    bool removeNode(int nodeId);
    
    /**
     * @brief Get a node by ID
     * @param nodeId The node ID
     * @return Node* Pointer to the node, or nullptr if not found
     */
    Node* getNode(int nodeId);
    
    /**
     * @brief Get a node by ID (const version)
     * @param nodeId The node ID
     * @return const Node* Pointer to the node, or nullptr if not found
     */
    const Node* getNode(int nodeId) const;
    
    /**
     * @brief Get all nodes
     * @return const std::vector<std::unique_ptr<Node>>& Vector of node pointers
     */
    const std::vector<std::unique_ptr<Node>>& nodes() const { return m_nodes; }
    
    /**
     * @brief Get the number of nodes
     * @return size_t Number of nodes
     */
    size_t nodeCount() const { return m_nodes.size(); }
    
    /**
     * @brief Add an edge between two nodes
     * @param startId ID of the start node
     * @param endId ID of the end node
     * @param weight Optional weight (if <= 0, calculated from positions)
     * @return true if the edge was created successfully
     */
    bool addEdge(int startId, int endId, double weight = 0.0);
    
    /**
     * @brief Remove an edge between two nodes
     * @param startId ID of the start node
     * @param endId ID of the end node
     * @return true if the edge was found and removed
     */
    bool removeEdge(int startId, int endId);
    
    /**
     * @brief Get an edge between two nodes
     * @param startId ID of the start node
     * @param endId ID of the end node
     * @return Edge* Pointer to the edge, or nullptr if not found
     */
    Edge* getEdge(int startId, int endId);
    
    /**
     * @brief Get an edge between two nodes (const version)
     * @param startId ID of the start node
     * @param endId ID of the end node
     * @return const Edge* Pointer to the edge, or nullptr if not found
     */
    const Edge* getEdge(int startId, int endId) const;
    
    /**
     * @brief Get all edges
     * @return const std::vector<std::unique_ptr<Edge>>& Vector of edge pointers
     */
    const std::vector<std::unique_ptr<Edge>>& edges() const { return m_edges; }
    
    /**
     * @brief Get the number of edges
     * @return size_t Number of edges
     */
    size_t edgeCount() const { return m_edges.size(); }
    
    /**
     * @brief Get all edges connected to a specific node
     * @param nodeId ID of the node
     * @return std::vector<Edge*> Vector of connected edges
     */
    std::vector<Edge*> getConnectedEdges(int nodeId);
    
    /**
     * @brief Check if there's an edge between two nodes
     * @param startId ID of the start node
     * @param endId ID of the end node
     * @return true if an edge exists
     */
    bool hasEdge(int startId, int endId) const;
    
    /**
     * @brief Get the adjacency matrix for the graph
     * @return std::vector<std::vector<double>> 2D matrix of weights (INF for no edge)
     */
    std::vector<std::vector<double>> getAdjacencyMatrix() const;
    
    /**
     * @brief Clear all nodes and edges
     */
    void clear();
    
    /**
     * @brief Check if the graph is directed
     * @return true if directed, false if undirected
     */
    bool isDirected() const { return m_directed; }

private:
    std::vector<std::unique_ptr<Node>> m_nodes;   ///< All nodes in the graph
    std::vector<std::unique_ptr<Edge>> m_edges;   ///< All edges in the graph
    std::map<int, int> m_nodeIndexMap;            ///< Map from node ID to index
    int m_nextNodeId = 0;                         ///< Counter for generating unique IDs
    bool m_directed;                              ///< Whether the graph is directed
    
    static constexpr double INF = 1e9;            ///< Infinity value for no connection
};

#endif // GRAPH_H

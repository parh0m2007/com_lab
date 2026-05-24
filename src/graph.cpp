#include "graph.h"
#include <algorithm>
#include <limits>

Graph::Graph(bool directed)
    : m_directed(directed)
{
}

int Graph::addNode(const QPointF& pos, const QString& label)
{
    int id = m_nextNodeId++;
    auto node = std::make_unique<Node>(id, pos, label);
    m_nodeIndexMap[id] = static_cast<int>(m_nodes.size());
    m_nodes.push_back(std::move(node));
    return id;
}

bool Graph::removeNode(int nodeId)
{
    auto it = m_nodeIndexMap.find(nodeId);
    if (it == m_nodeIndexMap.end()) {
        return false;
    }
    
    // Remove all edges connected to this node
    m_edges.erase(
        std::remove_if(m_edges.begin(), m_edges.end(),
            [nodeId](const std::unique_ptr<Edge>& edge) {
                return edge->connectsTo(nullptr) || 
                       edge->startNode() == nullptr || 
                       edge->endNode() == nullptr ||
                       edge->startNode()->id() == nodeId || 
                       edge->endNode()->id() == nodeId;
            }),
        m_edges.end()
    );
    
    // Remove the node
    size_t index = it->second;
    m_nodes.erase(m_nodes.begin() + index);
    m_nodeIndexMap.erase(it);
    
    // Update indices
    for (size_t i = index; i < m_nodes.size(); ++i) {
        m_nodeIndexMap[m_nodes[i]->id()] = static_cast<int>(i);
    }
    
    return true;
}

Node* Graph::getNode(int nodeId)
{
    auto it = m_nodeIndexMap.find(nodeId);
    if (it != m_nodeIndexMap.end()) {
        return m_nodes[it->second].get();
    }
    return nullptr;
}

const Node* Graph::getNode(int nodeId) const
{
    auto it = m_nodeIndexMap.find(nodeId);
    if (it != m_nodeIndexMap.end()) {
        return m_nodes[it->second].get();
    }
    return nullptr;
}

bool Graph::addEdge(int startId, int endId, double weight)
{
    Node* start = getNode(startId);
    Node* end = getNode(endId);
    
    if (!start || !end) {
        return false;
    }
    
    // Check if edge already exists
    if (hasEdge(startId, endId)) {
        return false;
    }
    
    auto edge = std::make_unique<Edge>(start, end, weight);
    m_edges.push_back(std::move(edge));
    
    // For undirected graphs, the edge works both ways
    // (handled by checking both directions in hasEdge/getEdge)
    
    return true;
}

bool Graph::removeEdge(int startId, int endId)
{
    auto it = std::find_if(m_edges.begin(), m_edges.end(),
        [startId, endId, this](const std::unique_ptr<Edge>& edge) {
            int s = edge->startNode()->id();
            int e = edge->endNode()->id();
            return (s == startId && e == endId) || 
                   (!m_directed && s == endId && e == startId);
        });
    
    if (it != m_edges.end()) {
        m_edges.erase(it);
        return true;
    }
    return false;
}

Edge* Graph::getEdge(int startId, int endId)
{
    auto it = std::find_if(m_edges.begin(), m_edges.end(),
        [startId, endId, this](const std::unique_ptr<Edge>& edge) {
            int s = edge->startNode()->id();
            int e = edge->endNode()->id();
            return (s == startId && e == endId) || 
                   (!m_directed && s == endId && e == startId);
        });
    
    if (it != m_edges.end()) {
        return it->get();
    }
    return nullptr;
}

const Edge* Graph::getEdge(int startId, int endId) const
{
    auto it = std::find_if(m_edges.begin(), m_edges.end(),
        [startId, endId, this](const std::unique_ptr<Edge>& edge) {
            int s = edge->startNode()->id();
            int e = edge->endNode()->id();
            return (s == startId && e == endId) || 
                   (!m_directed && s == endId && e == startId);
        });
    
    if (it != m_edges.end()) {
        return it->get();
    }
    return nullptr;
}

std::vector<Edge*> Graph::getConnectedEdges(int nodeId)
{
    std::vector<Edge*> result;
    for (auto& edge : m_edges) {
        if (edge->connectsTo(getNode(nodeId))) {
            result.push_back(edge.get());
        }
    }
    return result;
}

bool Graph::hasEdge(int startId, int endId) const
{
    return getEdge(startId, endId) != nullptr;
}

std::vector<std::vector<double>> Graph::getAdjacencyMatrix() const
{
    size_t n = m_nodes.size();
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n, INF));
    
    // Set diagonal to 0 (distance from node to itself)
    for (size_t i = 0; i < n; ++i) {
        matrix[i][i] = 0;
    }
    
    // Fill in edge weights
    for (const auto& edge : m_edges) {
        int i = edge->startNode()->id();
        int j = edge->endNode()->id();
        matrix[i][j] = edge->weight();
        if (!m_directed) {
            matrix[j][i] = edge->weight();
        }
    }
    
    return matrix;
}

void Graph::clear()
{
    m_nodes.clear();
    m_edges.clear();
    m_nodeIndexMap.clear();
    m_nextNodeId = 0;
}

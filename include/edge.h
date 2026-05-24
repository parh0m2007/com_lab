#ifndef EDGE_H
#define EDGE_H

#include <QColor>
#include <QLineF>

class Node;

/**
 * @class Edge
 * @brief Represents an edge (connection) between two nodes in the graph.
 * 
 * This class stores information about a connection between two nodes,
 * including the weight (distance) and visual properties.
 */
class Edge
{
public:
    /**
     * @brief Construct a new Edge object
     * @param start Start node
     * @param end End node
     * @param weight Edge weight (distance/cost)
     */
    Edge(Node* start, Node* end, double weight = 0.0);
    
    /**
     * @brief Get the start node
     * @return Node* Pointer to start node
     */
    Node* startNode() const { return m_start; }
    
    /**
     * @brief Get the end node
     * @return Node* Pointer to end node
     */
    Node* endNode() const { return m_end; }
    
    /**
     * @brief Get the edge weight
     * @return double The weight/cost
     */
    double weight() const { return m_weight; }
    
    /**
     * @brief Set the edge weight
     * @param weight New weight
     */
    void setWeight(double weight) { m_weight = weight; }
    
    /**
     * @brief Calculate weight based on node positions (Euclidean distance)
     */
    void calculateWeight();
    
    /**
     * @brief Check if this edge connects to a specific node
     * @param node The node to check
     * @return true if the edge connects to this node
     */
    bool connectsTo(const Node* node) const;
    
    /**
     * @brief Get the other node given one endpoint
     * @param node One endpoint
     * @return Node* The other endpoint, or nullptr if node is not connected
     */
    Node* otherNode(const Node* node) const;
    
    /**
     * @brief Check if the edge is selected
     * @return true if selected
     */
    bool isSelected() const { return m_selected; }
    
    /**
     * @brief Set the selection state
     * @param selected Selection state
     */
    void setSelected(bool selected) { m_selected = selected; }
    
    /**
     * @brief Get the edge color
     * @return QColor The current color
     */
    QColor color() const { return m_color; }
    
    /**
     * @brief Set the edge color
     * @param color New color
     */
    void setColor(const QColor& color) { m_color = color; }
    
    /**
     * @brief Get the line geometry for rendering
     * @return QLineF The line from start to end
     */
    QLineF line() const;

private:
    Node* m_start;               ///< Start node
    Node* m_end;                 ///< End node
    double m_weight = 0.0;       ///< Edge weight/cost
    bool m_selected = false;     ///< Selection state
    QColor m_color = Qt::black;  ///< Edge color
};

#endif // EDGE_H

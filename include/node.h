#ifndef NODE_H
#define NODE_H

#include <QPointF>
#include <QString>
#include <QColor>

/**
 * @class Node
 * @brief Represents a vertex (city) in the graph for the Traveling Salesman Problem.
 * 
 * This class stores information about a single node including its position,
 * label, and visual properties for rendering.
 */
class Node
{
public:
    /**
     * @brief Construct a new Node object
     * @param id Unique identifier for the node
     * @param pos Position of the node in 2D space
     * @param label Optional label displayed on the node
     */
    Node(int id, const QPointF& pos = QPointF(0, 0), const QString& label = QString());
    
    /**
     * @brief Get the node's unique identifier
     * @return int The node ID
     */
    int id() const { return m_id; }
    
    /**
     * @brief Get the node's position
     * @return QPointF The 2D position
     */
    QPointF position() const { return m_pos; }
    
    /**
     * @brief Set the node's position
     * @param pos New position
     */
    void setPosition(const QPointF& pos) { m_pos = pos; }
    
    /**
     * @brief Get the node's label
     * @return QString The label text
     */
    QString label() const { return m_label; }
    
    /**
     * @brief Set the node's label
     * @param label New label text
     */
    void setLabel(const QString& label) { m_label = label; }
    
    /**
     * @brief Check if the node is selected
     * @return true if selected, false otherwise
     */
    bool isSelected() const { return m_selected; }
    
    /**
     * @brief Set the selection state
     * @param selected Selection state
     */
    void setSelected(bool selected) { m_selected = selected; }
    
    /**
     * @brief Get the node's color
     * @return QColor The current color
     */
    QColor color() const { return m_color; }
    
    /**
     * @brief Set the node's color
     * @param color New color
     */
    void setColor(const QColor& color) { m_color = color; }
    
    /**
     * @brief Get the radius for rendering
     * @return qreal The radius
     */
    qreal radius() const { return m_radius; }
    
    /**
     * @brief Set the radius for rendering
     * @param radius New radius
     */
    void setRadius(qreal radius) { m_radius = radius; }
    
    /**
     * @brief Check if a point is inside the node (for hit testing)
     * @param point The point to check
     * @return true if the point is inside the node
     */
    bool contains(const QPointF& point) const;

private:
    int m_id;                    ///< Unique identifier
    QPointF m_pos;               ///< 2D position
    QString m_label;             ///< Display label
    bool m_selected = false;     ///< Selection state
    QColor m_color = Qt::blue;   ///< Node color
    qreal m_radius = 15.0;       ///< Visual radius
};

#endif // NODE_H

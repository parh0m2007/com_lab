#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QOpenGLWidget>
#include <QMouseEvent>
#include <QPainter>
#include <vector>
#include "graph.h"
#include "tsp_solver.h"

/**
 * @class GraphWidget
 * @brief OpenGL-based widget for interactive graph visualization and manipulation.
 * 
 * This widget provides a complete interactive interface for:
 * - Adding nodes by clicking
 * - Creating edges between nodes
 * - Dragging nodes to reposition them
 * - Selecting and deleting nodes/edges
 * - Visualizing the TSP solution path
 * 
 * Designer features include:
 * - Smooth animations and visual feedback
 * - Color-coded elements (nodes, edges, solution path)
 * - Hover effects and selection highlighting
 * - Grid background for better spatial understanding
 */
class GraphWidget : public QOpenGLWidget
{
    Q_OBJECT
    
public:
    /**
     * @brief Interaction modes for the widget
     */
    enum class Mode {
        AddNode,      ///< Click to add new nodes
        AddEdge,      ///< Click two nodes to create an edge
        MoveNode,     ///< Drag nodes to reposition
        Select,       ///< Select nodes/edges for deletion
        Solve         ///< Display solution mode
    };
    
    explicit GraphWidget(QWidget* parent = nullptr);
    ~GraphWidget() override;
    
    /**
     * @brief Get the underlying graph object
     * @return Graph& Reference to the graph
     */
    Graph& graph() { return m_graph; }
    const Graph& graph() const { return m_graph; }
    
    /**
     * @brief Set the interaction mode
     * @param mode New mode
     */
    void setMode(Mode mode);
    
    /**
     * @brief Get current interaction mode
     * @return Mode Current mode
     */
    Mode mode() const { return m_mode; }
    
    /**
     * @brief Solve the TSP and display the result
     */
    void solveTSP();
    
    /**
     * @brief Clear the graph and reset state
     */
    void clearGraph();
    
    /**
     * @brief Load a sample graph for demonstration
     */
    void loadSampleGraph();
    
signals:
    /**
     * @brief Emitted when the graph structure changes
     */
    void graphChanged();
    
    /**
     * @brief Emitted when a solution is found
     * @param path The optimal path
     * @param cost Total cost
     * @param nodesExplored Number of nodes explored
     */
    void solutionFound(const std::vector<int>& path, double cost, int nodesExplored);
    
    /**
     * @brief Emitted when no solution exists
     */
    void noSolutionFound();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    /**
     * @brief Draw the grid background
     */
    void drawGrid();
    
    /**
     * @brief Draw all nodes
     */
    void drawNodes();
    
    /**
     * @brief Draw all edges
     */
    void drawEdges();
    
    /**
     * @brief Draw the solution path
     */
    void drawSolutionPath();
    
    /**
     * @brief Draw weight labels on edges
     */
    void drawEdgeWeights();
    
    /**
     * @brief Find node at screen position
     * @param pos Screen position
     * @return Node* Found node or nullptr
     */
    Node* findNodeAt(const QPoint& pos);
    
    /**
     * @brief Find edge at screen position
     * @param pos Screen position
     * @return Edge* Found edge or nullptr
     */
    Edge* findEdgeAt(const QPoint& pos);
    
    /**
     * @brief Convert screen coordinates to world coordinates
     * @param pos Screen position
     * @return QPointF World position
     */
    QPointF screenToWorld(const QPoint& pos);
    
    /**
     * @brief Project a point onto a line segment
     * @param p1 First endpoint of segment
     * @param p2 Second endpoint of segment
     * @param point Point to project
     * @return QPointF Projection point
     */
    QPointF projectPointOnSegment(const QPointF& p1, const QPointF& p2, const QPointF& point);
    
    /**
     * @brief Check if a point lies on a line segment
     * @param p1 First endpoint of segment
     * @param p2 Second endpoint of segment
     * @param point Point to check
     * @return bool True if point is on segment
     */
    bool isPointOnSegment(const QPointF& p1, const QPointF& p2, const QPointF& point);
    
    Graph m_graph;                          ///< The graph data structure
    Mode m_mode = Mode::AddNode;            ///< Current interaction mode
    Node* m_draggedNode = nullptr;          ///< Node being dragged
    Node* m_selectedNode = nullptr;         ///< Currently selected node
    Edge* m_selectedEdge = nullptr;         ///< Currently selected edge
    Node* m_edgeStartNode = nullptr;        ///< Start node for edge creation
    std::vector<int> m_solutionPath;        ///< Current TSP solution path
    double m_solutionCost = 0;              ///< Cost of current solution
    bool m_hasSolution = false;             ///< Whether a solution exists
    QColor m_gridColor = QColor(200, 200, 200);   ///< Grid line color
    QColor m_nodeColor = QColor(65, 105, 225);    ///< Default node color (Royal Blue)
    QColor m_edgeColor = QColor(50, 50, 50);      ///< Default edge color
    QColor m_solutionColor = QColor(220, 20, 60); ///< Solution path color (Crimson)
    QColor m_hoverColor = QColor(100, 149, 237);  ///< Hover highlight (Cornflower Blue)
};

#endif // GRAPHWIDGET_H

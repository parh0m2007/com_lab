#include "graphwidget.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <cmath>
#include <QPainterPath>

GraphWidget::GraphWidget(QWidget* parent)
    : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(400, 400);
    setAutoFillBackground(false);
}

GraphWidget::~GraphWidget()
{
}

void GraphWidget::setMode(Mode mode)
{
    m_mode = mode;
    m_edgeStartNode = nullptr;
    m_selectedNode = nullptr;
    m_selectedEdge = nullptr;
    update();
}

void GraphWidget::solveTSP()
{
    if (m_graph.nodeCount() < 2) {
        emit noSolutionFound();
        return;
    }
    
    auto matrix = m_graph.getAdjacencyMatrix();
    TSPSolver solver;
    auto result = solver.solve(matrix);
    
    if (result.found) {
        m_solutionPath = result.path;
        m_solutionCost = result.totalCost;
        m_hasSolution = true;
        emit solutionFound(result.path, result.totalCost, result.nodesExplored);
    } else {
        m_hasSolution = false;
        m_solutionPath.clear();
        emit noSolutionFound();
    }
    
    update();
}

void GraphWidget::clearGraph()
{
    m_graph.clear();
    m_solutionPath.clear();
    m_hasSolution = false;
    m_edgeStartNode = nullptr;
    m_selectedNode = nullptr;
    m_selectedEdge = nullptr;
    update();
    emit graphChanged();
}

void GraphWidget::loadSampleGraph()
{
    clearGraph();
    
    // Create a sample graph with 6+ nodes arranged in a circular pattern
    // This represents cities in a region that can be visited
    int n = 8;
    qreal radius = 150.0;
    qreal centerX = width() / 2.0;
    qreal centerY = height() / 2.0;
    
    // Add nodes in a circular pattern
    std::vector<int> nodeIds;
    for (int i = 0; i < n; ++i) {
        qreal angle = (2.0 * M_PI * i) / n - M_PI / 2;
        qreal x = centerX + radius * cos(angle);
        qreal y = centerY + radius * sin(angle);
        int id = m_graph.addNode(QPointF(x, y), QString::number(i + 1));
        nodeIds.push_back(id);
    }
    
    // Create edges to form a connected graph
    // Each node connects to its neighbors and some cross connections
    for (int i = 0; i < n; ++i) {
        // Connect to next node (forms outer ring)
        m_graph.addEdge(nodeIds[i], nodeIds[(i + 1) % n]);
        
        // Add some cross connections for more paths
        if (n > 4) {
            int jump = n / 2;
            m_graph.addEdge(nodeIds[i], nodeIds[(i + jump) % n]);
        }
        
        // Additional connections for better connectivity
        if (i + 2 < n) {
            m_graph.addEdge(nodeIds[i], nodeIds[i + 2]);
        }
    }
    
    m_hasSolution = false;
    m_solutionPath.clear();
    update();
    emit graphChanged();
}

void GraphWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Clear background
    painter.fillRect(rect(), Qt::white);
    
    // Draw grid
    drawGrid();
    
    // Draw edges first (so they appear behind nodes)
    drawEdges();
    
    // Draw solution path on top of regular edges
    if (m_hasSolution) {
        drawSolutionPath();
    }
    
    // Draw nodes
    drawNodes();
    
    // Draw edge weights
    drawEdgeWeights();
    
    // Draw current mode indicator
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 10));
    QString modeText;
    switch (m_mode) {
        case Mode::AddNode: modeText = "Mode: Add Node (Click to add)"; break;
        case Mode::AddEdge: modeText = "Mode: Add Edge (Click two nodes)"; break;
        case Mode::MoveNode: modeText = "Mode: Move Node (Drag nodes)"; break;
        case Mode::Select: modeText = "Mode: Select (Click to select, Del to delete)"; break;
        case Mode::Solve: modeText = "Mode: Solution Display"; break;
    }
    painter.drawText(10, 20, modeText);
    
    // Draw solution info if available
    if (m_hasSolution) {
        QString solText = QString("Solution Cost: %1").arg(m_solutionCost, 0, 'f', 2);
        painter.drawText(10, 40, solText);
    }
}

void GraphWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    // Handle resize if needed
}

void GraphWidget::drawGrid()
{
    QPainter painter(this);
    painter.setPen(QPen(m_gridColor, 1, Qt::DotLine));
    
    int gridSize = 50;
    for (int x = 0; x < width(); x += gridSize) {
        painter.drawLine(x, 0, x, height());
    }
    for (int y = 0; y < height(); y += gridSize) {
        painter.drawLine(0, y, width(), y);
    }
}

void GraphWidget::drawNodes()
{
    QPainter painter(this);
    
    for (const auto& node : m_graph.nodes()) {
        QPointF pos = node->position();
        qreal radius = node->radius();
        
        // Determine color
        QColor color = m_nodeColor;
        if (node.get() == m_draggedNode) {
            color = m_hoverColor;
        } else if (node.get() == m_selectedNode) {
            color = Qt::red;
        } else if (node.get() == m_edgeStartNode) {
            color = Qt::green;
        }
        
        // Draw node circle
        painter.setBrush(color);
        painter.setPen(QPen(Qt::black, 2));
        painter.drawEllipse(pos, radius, radius);
        
        // Draw label
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        painter.drawText(QRectF(pos.x() - radius, pos.y() - radius, 
                                2 * radius, 2 * radius),
                        Qt::AlignCenter, node->label());
    }
}

void GraphWidget::drawEdges()
{
    QPainter painter(this);
    
    for (const auto& edge : m_graph.edges()) {
        Node* start = edge->startNode();
        Node* end = edge->endNode();
        
        if (!start || !end) continue;
        
        QLineF line = edge->line();
        
        // Determine color
        QColor color = m_edgeColor;
        if (edge.get() == m_selectedEdge) {
            color = Qt::red;
        }
        
        // Draw edge line
        painter.setPen(QPen(color, 3, Qt::SolidLine));
        painter.drawLine(line);
        
        // Draw arrowhead for directed graphs or just endpoints
        if (!m_graph.isDirected()) {
            // Draw small circles at endpoints
            painter.setBrush(color);
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(start->position(), 4, 4);
            painter.drawEllipse(end->position(), 4, 4);
        }
    }
    
    // Draw temporary edge when creating
    if (m_edgeStartNode && m_mode == Mode::AddEdge) {
        // This would need mouse position tracking
    }
}

void GraphWidget::drawSolutionPath()
{
    if (m_solutionPath.empty()) return;
    
    QPainter painter(this);
    painter.setPen(QPen(m_solutionColor, 4, Qt::SolidLine));
    painter.setBrush(Qt::NoBrush);
    
    QPolygonF polygon;
    for (int nodeId : m_solutionPath) {
        Node* node = m_graph.getNode(nodeId);
        if (node) {
            polygon << node->position();
        }
    }
    
    // Close the loop
    if (!polygon.isEmpty() && m_solutionPath.size() > 1) {
        Node* firstNode = m_graph.getNode(m_solutionPath.front());
        if (firstNode) {
            polygon << firstNode->position();
        }
    }
    
    // Draw the path
    if (polygon.size() > 2) {
        painter.drawPolygon(polygon);
    } else if (polygon.size() == 2) {
        painter.drawLine(polygon[0], polygon[1]);
    }
    
    // Draw arrows to show direction
    painter.setBrush(m_solutionColor);
    painter.setPen(Qt::NoPen);
    for (int i = 0; i < polygon.size() - 1; ++i) {
        QPointF p1 = polygon[i];
        QPointF p2 = polygon[i + 1];
        
        // Calculate midpoint and angle
        QPointF mid = (p1 + p2) / 2.0;
        qreal angle = QLineF(p1, p2).angle();
        
        // Draw small triangle
        painter.save();
        painter.translate(mid);
        painter.rotate(angle);
        QPainterPath arrow;
        arrow.moveTo(5, 0);
        arrow.lineTo(-3, -3);
        arrow.lineTo(-3, 3);
        arrow.closeSubpath();
        painter.drawPath(arrow);
        painter.restore();
    }
}

void GraphWidget::drawEdgeWeights()
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9));
    
    for (const auto& edge : m_graph.edges()) {
        Node* start = edge->startNode();
        Node* end = edge->endNode();
        
        if (!start || !end) continue;
        
        QPointF mid = (start->position() + end->position()) / 2.0;
        QString weight = QString::number(edge->weight(), 'f', 1);
        
        // Draw background rectangle for readability
        QRectF textRect = painter.fontMetrics().boundingRect(weight);
        textRect.moveCenter(mid);
        textRect.adjust(-2, -2, 2, 2);
        
        painter.setBrush(Qt::white);
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(textRect, 3, 3);
        
        // Draw text
        painter.setPen(Qt::black);
        painter.drawText(textRect, Qt::AlignCenter, weight);
    }
}

Node* GraphWidget::findNodeAt(const QPoint& pos)
{
    for (const auto& node : m_graph.nodes()) {
        if (node->contains(screenToWorld(pos))) {
            return node.get();
        }
    }
    return nullptr;
}

Edge* GraphWidget::findEdgeAt(const QPoint& pos)
{
    const qreal hitThreshold = 10.0;
    
    for (const auto& edge : m_graph.edges()) {
        Node* start = edge->startNode();
        Node* end = edge->endNode();
        
        if (!start || !end) continue;
        
        QPointF p1 = start->position();
        QPointF p2 = end->position();
        QPointF point = screenToWorld(pos);
        
        // Calculate distance from point to line segment manually
        QPointF proj = projectPointOnSegment(p1, p2, point);
        if (isPointOnSegment(p1, p2, proj)) {
            qreal dist = QLineF(proj, point).length();
            if (dist < hitThreshold) {
                return edge.get();
            }
        }
    }
    return nullptr;
}

QPointF GraphWidget::projectPointOnSegment(const QPointF& p1, const QPointF& p2, const QPointF& point)
{
    QPointF vec = p2 - p1;
    qreal len2 = QPointF::dotProduct(vec, vec);
    
    if (len2 == 0) {
        return p1;
    }
    
    qreal t = QPointF::dotProduct(point - p1, vec) / len2;
    t = qBound(0.0, t, 1.0);
    
    return p1 + t * vec;
}

bool GraphWidget::isPointOnSegment(const QPointF& p1, const QPointF& p2, const QPointF& point)
{
    QPointF vec1 = point - p1;
    QPointF vec2 = p2 - p1;
    
    qreal dot = QPointF::dotProduct(vec1, vec2);
    qreal len2 = QPointF::dotProduct(vec2, vec2);
    
    return (dot >= 0 && dot <= len2);
}

QPointF GraphWidget::screenToWorld(const QPoint& pos)
{
    return QPointF(pos.x(), pos.y());
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();
        Node* clickedNode = findNodeAt(pos);
        
        switch (m_mode) {
            case Mode::AddNode:
                if (!clickedNode) {
                    int id = m_graph.addNode(screenToWorld(pos));
                    emit graphChanged();
                    update();
                }
                break;
                
            case Mode::AddEdge:
                if (clickedNode) {
                    if (!m_edgeStartNode) {
                        m_edgeStartNode = clickedNode;
                        clickedNode->setSelected(true);
                    } else if (m_edgeStartNode != clickedNode) {
                        m_graph.addEdge(m_edgeStartNode->id(), clickedNode->id());
                        m_edgeStartNode->setSelected(false);
                        m_edgeStartNode = nullptr;
                        emit graphChanged();
                    }
                    update();
                }
                break;
                
            case Mode::MoveNode:
                m_draggedNode = clickedNode;
                update();
                break;
                
            case Mode::Select:
                m_selectedNode = clickedNode;
                m_selectedEdge = clickedNode ? nullptr : findEdgeAt(pos);
                update();
                break;
                
            case Mode::Solve:
                solveTSP();
                break;
        }
    }
    
    QWidget::mousePressEvent(event);
}

void GraphWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_draggedNode && m_mode == Mode::MoveNode) {
        m_draggedNode->setPosition(screenToWorld(event->pos()));
        update();
    }
    
    QWidget::mouseMoveEvent(event);
}

void GraphWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        if (m_draggedNode) {
            m_draggedNode = nullptr;
            emit graphChanged();
        }
    }
    
    QWidget::mouseReleaseEvent(event);
}

void GraphWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        if (m_mode == Mode::Select) {
            bool changed = false;
            
            if (m_selectedNode) {
                m_graph.removeNode(m_selectedNode->id());
                m_selectedNode = nullptr;
                changed = true;
            } else if (m_selectedEdge) {
                m_graph.removeEdge(m_selectedEdge->startNode()->id(),
                                   m_selectedEdge->endNode()->id());
                m_selectedEdge = nullptr;
                changed = true;
            }
            
            if (changed) {
                m_hasSolution = false;
                m_solutionPath.clear();
                emit graphChanged();
                update();
            }
        }
    }
    
    QWidget::keyPressEvent(event);
}

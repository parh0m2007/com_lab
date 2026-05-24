#include "edge.h"
#include "node.h"
#include <QPointF>
#include <cmath>

Edge::Edge(Node* start, Node* end, double weight)
    : m_start(start)
    , m_end(end)
    , m_weight(weight)
{
    if (m_weight <= 0.0 && start && end) {
        calculateWeight();
    }
}

void Edge::calculateWeight()
{
    if (m_start && m_end) {
        QPointF p1 = m_start->position();
        QPointF p2 = m_end->position();
        qreal dx = p2.x() - p1.x();
        qreal dy = p2.y() - p1.y();
        m_weight = std::sqrt(dx * dx + dy * dy);
    }
}

bool Edge::connectsTo(const Node* node) const
{
    return m_start == node || m_end == node;
}

Node* Edge::otherNode(const Node* node) const
{
    if (m_start == node) {
        return m_end;
    } else if (m_end == node) {
        return m_start;
    }
    return nullptr;
}

QLineF Edge::line() const
{
    if (m_start && m_end) {
        return QLineF(m_start->position(), m_end->position());
    }
    return QLineF();
}

#include "node.h"
#include <QPointF>
#include <cmath>

Node::Node(int id, const QPointF& pos, const QString& label)
    : m_id(id)
    , m_pos(pos)
    , m_label(label.isEmpty() ? QString::number(id) : label)
{
}

bool Node::contains(const QPointF& point) const
{
    qreal dx = point.x() - m_pos.x();
    qreal dy = point.y() - m_pos.y();
    return (dx * dx + dy * dy) <= (m_radius * m_radius);
}

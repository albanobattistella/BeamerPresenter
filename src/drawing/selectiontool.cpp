#include "src/drawing/selectiontool.h"

void SelectionTool::setPos(const QPointF &pos) noexcept
{
    type = Move;
    start_properties.position = pos;
    live_properties.position = pos;
}

QPointF SelectionTool::movePosition(const QPointF &new_position) noexcept
{
    const QPointF diff = new_position - live_properties.position;
    live_properties.position = new_position;
    return diff;
}

QTransform SelectionTool::transform() const
{
    QTransform transform;
    switch (type)
    {
    case Move:
        transform.translate(live_properties.position.x() - start_properties.position.x(), live_properties.position.y() - start_properties.position.y());
        break;
    case Rotate:
        // TODO
        break;
    case ResizeFlexible:
    case ResizeFixed:
    case ResizeHorizontally:
    case ResizeVertically:
        // TODO
        break;
    default:
        break;
    }
    return transform;
}

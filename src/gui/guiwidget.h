#ifndef GUIWIDGET_H
#define GUIWIDGET_H

#include <QSizeF>

/// This whole construction might change in the future.
/// Abstract class for distinguishing different widget types.
/// It is not a widget itself! Without casting to the correct type, all child
/// objects are useless.
class GuiWidget
{
protected:
    /// Prefered size of the widget inside the given geometry of its parent
    /// widget.
    QSizeF preferred_size;

public:
    /// Type of the widget.
    enum WidgetType {
        InvalidType, // QWidget
        ContainerWidget, // ContainerWidget
        StackedWidget,
        Slide, // SlideView
        Overview,
        TOC,
        Notes,
        Button,
        ToolSelector,
        Settings,
        Clock,
        Timer,
        SlideNumber,
    };
    const WidgetType type;

    /// Constructor: only initialize type.
    GuiWidget(const WidgetType type) : type(type) {}

    /// Set preferred size
    void setPreferredSize(const QSizeF& size)
    {preferred_size = size;}

    /// Get preferred size based on parent_size.
    virtual const QSizeF preferredSize(QSizeF const& parent_size) const
    {return preferred_size;}

    /// Set (maximum) widget width.
    virtual void setWidth(const qreal width) = 0;

    /// Set (maximum) widget height.
    virtual void setHeight(const qreal height) = 0;
};

#endif // GUIWIDGET_H
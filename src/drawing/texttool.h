#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include <QFont>
#include "src/drawing/tool.h"

/**
 * @brief Tool for writing text.
 *
 * TODO:
 *   better input handling, the whole construction is rather improvised
 *   ignore empty text boxes
 *   history for text editing
 */
class TextTool : public Tool
{
    QFont _font;
    QColor _color;

public:
    TextTool(const TextTool& other) :
        Tool(TextInputTool, other._device), _font(other._font), _color(other._color) {}

    TextTool(const QFont &font = QFont(), const QColor &color = Qt::black, const int device = 0) noexcept :
        Tool(TextInputTool, device), _font(font), _color(color) {}

    QFont &font() noexcept
    {return _font;}

    const QFont &font() const noexcept
    {return _font;}

    const QColor &color() const noexcept
    {return _color;}

    void setColor(const QColor &color) noexcept
    {_color = color;}

    void setColor(const QFont &font) noexcept
    {_font = font;}
};

#endif // TEXTTOOL_H
#ifndef MUPDFRENDERER_H
#define MUPDFRENDERER_H

#include <QDebug>
#include <QObject>
#include <QMetaType>
#include "src/rendering/mupdfdocument.h"
#include "src/rendering/abstractrenderer.h"

class MuPdfRenderer : public QObject, public AbstractRenderer
{
    Q_OBJECT

public:
    MuPdfRenderer() : AbstractRenderer() {}
    ~MuPdfRenderer() override {}

    /// Render page to a QPixmap.
    /// Resolution is given in dpi.
    const QPixmap renderPixmap(const int page, const qreal resolution) const override;
    /// Render page to PNG image in a QByteArray.
    /// Resolution is given in dpi.
    const PngPixmap * renderPng(const int page, const qreal resolution) const override;

    /// In the current implementation this is always valid.
    bool isValid() const override {return true;}

signals:
    void prepareRendering(fz_context **ctx, fz_rect* bbox, fz_display_list **list, const int pagenumber, const qreal resolution) const;
};

#endif // MUPDFRENDERER_H

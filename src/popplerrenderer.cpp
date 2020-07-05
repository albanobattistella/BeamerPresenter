#include "popplerrenderer.h"

const QPixmap PopplerRenderer::renderPixmap(const int page, const qreal resolution) const
{
    const Poppler::Page * const popplerPage = doc->page(page);
    if (popplerPage == nullptr)
    {
        qWarning() << "Tried to render invalid page" << page;
        return QPixmap();
    }
    return QPixmap::fromImage(popplerPage->renderToImage(resolution, resolution));
}

const QByteArray * PopplerRenderer::renderPng(const int page, const qreal resolution) const
{
    const Poppler::Page * const popplerPage = doc->page(page);
    if (popplerPage == nullptr)
    {
        qWarning() << "Tried to render invalid page" << page;
        return nullptr;
    }
    const QImage image = popplerPage->renderToImage(resolution, resolution);
    if (image.isNull())
    {
        qWarning() << "Rendering page to image failed";
        return nullptr;
    }
    QByteArray* const bytes = new QByteArray();
    QBuffer buffer(bytes);
    buffer.open(QIODevice::WriteOnly);
    if (!image.save(&buffer, "PNG"))
    {
        qWarning() << "Saving page as PNG image failed";
        delete bytes;
        return nullptr;
    }
    return bytes;
}

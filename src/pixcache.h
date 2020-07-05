#ifndef PIXCACHE_H
#define PIXCACHE_H

#include <QObject>
#include <QMap>
#include "src/pngpixmap.h"
#include "src/pixcachethread.h"

#define MAX_RESOLUTION_DEVIATION 1e-9

class PdfMaster;

/// Cache of compressed slides as PNG images.
/// This does the job of rendering slides to images and storing these images
/// in compressed cache.
class PixCache : public QObject
{
    Q_OBJECT

private:
    /// Map page numbers to cached PNG pixmaps.
    /// Pages which are currently being rendered are marked with a nullptr here.
    QMap<int, const PngPixmap*> cache;

    /// List of pages which should be rendered next.
    QList<int> priority;

    /// Boundaries of simply connected region of cache containing current page.
    QPair<int,int> region = {INT_MAX, -1};

    /// Size in which the slides should be rendered.
    QSizeF frame;

    /// Quorum of memory which should be used by this.
    float maxMemory = -1.f;

    /// Current size in bytes
    qint64 usedMemory = 0;

    /// Maximum number of slides in cache
    int maxNumber = -1;

    /// Current page number
    int currentPage = 0;

    /// Threads used to render pages to cache.
    QVector<PixCacheThread*> threads;

    /// Pdf document owning this.
    PdfMaster const* pdfMaster;

    /// Check cache size and delete pages if necessary.
    /// Return estimated number of pages which still fit in cache.
    /// Return -1 if cache is unlimited or empty.
    int limitCacheSize();

    /// Choose a page which should be rendered next.
    /// The page is then marked as "being rendered".
    /// This page must then also be rendered.
    int renderNext();

public:
    explicit PixCache(const int thread_number = 1, QObject *parent = nullptr);
    ~PixCache();

    /// Clear cache, delete all cached pages.
    void clear();

    /// Set maximum allowed bytes of memory used by this->cache.
    /// Clean up memory if necessary.
    void setMaxMemory(const float memory);

    /// Set maximum allowed number of cache slides.
    /// Clean up memory if necessary.
    void setMaxNumber(const int number);

    /// Get pixmap showing page n.
    QPixmap * pixmap(const int n) const;

    /// Total size of all cached pages in bytes
    qint64 getUsedMemory() const {return usedMemory;}

    /// Update current page number.
    /// Update boundary of simply connected region of cached pages.
    /// This does not fully recalculate the region, but assumes that the
    /// currently saved region is indeed simply connected.
    void updatePageNumber(const int page_number);

public slots:
    /// Request rendering a page with low priority
    void requestRenderPage(const int n);

    /// Start rendering the next page(s).
    void startRendering();

    /// Receive a PngPixmap from one of the threads.
    void receiveData(const PngPixmap * const data);

signals:

};

#endif // PIXCACHE_H

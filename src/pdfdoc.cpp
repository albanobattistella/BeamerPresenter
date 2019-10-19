/*
 * This file is part of BeamerPresenter.
 * Copyright (C) 2019  stiglers-eponym

 * BeamerPresenter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * BeamerPresenter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with BeamerPresenter. If not, see <https://www.gnu.org/licenses/>.
 */

#include "pdfdoc.h"

PdfDoc::PdfDoc(QString pathToPdf)
{
    pdfPath = pathToPdf;
}

PdfDoc::~PdfDoc()
{
    qDeleteAll(pdfPages);
    pdfPages.clear();
    labels.clear();
    delete popplerDoc;
}

bool PdfDoc::loadDocument()
{
    // (Re)load the pdf document.
    // Return true if a new document has been loaded and false otherwise.
    // If a document has been loaded before, this function checks whether the file has been updated and reloads it if necessary.

    // Check if the file is valid and readable and save its last modification time
    QFileInfo file = QFileInfo(pdfPath);
    if (!file.exists() || !file.isFile()) {
        qCritical() << "Trying to use a non-existing file:" << pdfPath;
        return false;
    }
    else if (!file.isReadable()) {
        qCritical() << "Trying to use a unreadable file:" << pdfPath;
        return false;
    }
    if (file.suffix().toLower() != "pdf")
        qWarning() << "Interpreting the following file as PDF file:" << pdfPath;

    // Check whether the file has been updated
    if (popplerDoc != nullptr && QFileInfo(pdfPath).lastModified() <= lastModified)
        return false;

    // Load the file
    Poppler::Document* newDoc = Poppler::Document::load(pdfPath);
    if (newDoc == nullptr) {
        qCritical() << "Failed to open document";
        return false;
    }
    // PDF files can be locked.
    // Locked pdf files are not really supported, as you can see:
    if (newDoc->isLocked()) {
        // TODO: use a nicer way of entering passwords (a QDialog?)
        qCritical() << "Support for locked files is HIGHLY EXPERIMENTAL:";
        std::cout << "WARNING: File " << qPrintable(pdfPath) << ":\n"
                  << "This file is locked. Support for locked files is HIGHLY EXPERIMENTAL!" << std::endl
                  << "You can try to enter your password here.\n"
                  << "YOUR PASSWORD WILL BE VISIBLE IF YOU ENTER IT HERE!" << std::endl;
        std::string ownerPassword;
        std::string userPassword;
        std::cout << "Owner password (NOT HIDDEN!): ";
        std::cin >> ownerPassword;
        std::cout << "User password (NOT HIDDEN!): ";
        std::cin >> userPassword;
        if (!newDoc->unlock(QByteArray::fromStdString(ownerPassword), QByteArray::fromStdString(userPassword))) {
            qCritical() << "Failed to unlock document";
            delete newDoc;
            return false;
        }
    }

    // Set rendering hints
    newDoc->setRenderHint(Poppler::Document::TextAntialiasing);
    newDoc->setRenderHint(Poppler::Document::TextHinting);
    newDoc->setRenderHint(Poppler::Document::TextSlightHinting);
    newDoc->setRenderHint(Poppler::Document::Antialiasing);
    newDoc->setRenderHint(Poppler::Document::ThinLineShape);
#ifdef OLD_POPPLER_VERSION
#else
    newDoc->setRenderHint(Poppler::Document::HideAnnotations);
#endif

    // Clear old lists
    qDeleteAll(pdfPages);
    pdfPages.clear();
    labels.clear();

    // Create new lists of pages and labels
    for (int i=0; i < newDoc->numPages(); i++) {
        Poppler::Page* p = newDoc->page(i);
        pdfPages.append(p);
        labels.append(p->label());
    }

    // Check document contents and print warnings if unimplemented features are found.
    if (newDoc->hasOptionalContent())
        qWarning() << "This file has optional content. Optional content is not supported.";
    if (newDoc->hasEmbeddedFiles())
        qWarning() << "This file contains embedded files. Embedded files are not supported.";
    if (newDoc->scripts().size() != 0)
        qWarning() << "This file contains JavaScript scripts. JavaScript is not supported.";

    delete popplerDoc;
    popplerDoc = newDoc;
    lastModified = file.lastModified();
    return true;
}

QSize PdfDoc::getPageSize(int const pageNumber) const
{
    // Return page size in point = inch/72 ≈ 0.353mm (I hate these units...)
    if (pageNumber < 0)
        return pdfPages[0]->pageSize();
    if (pageNumber >= popplerDoc->numPages())
        return pdfPages[popplerDoc->numPages()-1]->pageSize();
    return pdfPages[pageNumber]->pageSize();
}

Poppler::Page* PdfDoc::getPage(int pageNumber) const
{
    // Check if page number is valid and return page.
    if (pageNumber < 0)
        return pdfPages[0];
    if (pageNumber >= popplerDoc->numPages())
        return pdfPages[popplerDoc->numPages()-1];
    return pdfPages[pageNumber];
}

int PdfDoc::getNextSlideIndex(int const index) const
{
    // Return the index of the next slide, which is not just an overlay of the current slide.
    QString label = labels[index];
    for (int i=index; i<popplerDoc->numPages(); i++) {
        if (label != labels[i])
            return i;
    }
    return popplerDoc->numPages()-1;
}

int PdfDoc::getPreviousSlideEnd(int const index) const
{
    // Return the index of the last overlay of the previous slide.
    QString label = labels[index];
    for (int i=index; i>0; i--) {
        if (label != labels[i]) {
            double duration = pdfPages[i]->duration();
            int j=i;
            // Don't return the index of a slides which is shown for less than one second
            while (duration > -0.01 && duration < 1. && (labels[j] == labels[i]))
                duration = pdfPages[--j]->duration();
            return j;
        }
    }
    return 0;
}

int PdfDoc::destToSlide(QString const & dest) const
{
    // Return the index of the page, which is bookmarked as dest in the pdf.
    Poppler::LinkDestination* linkDest = popplerDoc->linkDestination(dest);
    if (linkDest==nullptr)
        return -1;
    int const page = linkDest->pageNumber() - 1;
    delete linkDest;
    return page;
}

QString const& PdfDoc::getLabel(int const pageNumber) const
{
    if (pageNumber<0)
        return labels.first();
    else if (pageNumber>popplerDoc->numPages())
        return labels.last();
    else
        return labels[pageNumber];
}

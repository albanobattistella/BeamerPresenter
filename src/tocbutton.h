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

#ifndef TOCBUTTON_H
#define TOCBUTTON_H

#include <QtDebug>
#include <QPushButton>

class TocButton : public QPushButton
{
    Q_OBJECT

private:
    QString dest;

public:
    TocButton(QString const& text = "", QString const& dest = "", QWidget * parent = nullptr);

signals:
    void activated(QString const& dest);
};

#endif // TOCBUTTON_H
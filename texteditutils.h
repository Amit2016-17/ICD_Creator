/**************************************************************************
 *  This file is part of QXmlEdit                                         *
 *  Copyright (C) 2011 by Luca Bellonda and individual contributors       *
 *    as indicated in the AUTHORS file                                    *
 *  lbellonda _at_ gmail.com                                              *
 *                                                                        *
 * This library is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU Library General Public            *
 * License as published by the Free Software Foundation; either           *
 * version 2 of the License, or (at your option) any later version.       *
 *                                                                        *
 * This library is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      *
 * Library General Public License for more details.                       *
 *                                                                        *
 * You should have received a copy of the GNU Library General Public      *
 * License along with this library; if not, write to the                  *
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,       *
 * Boston, MA  02110-1301  USA                                            *
 **************************************************************************/
#ifndef TEXTEDITUTILS_H
#define TEXTEDITUTILS_H

#include <QTextEdit>

class TextEditUtils
{
private:
    TextEditUtils() {}
public:
    static void gotoPos(QTextEdit *textEdit, const int column, const int row);
    static void hiliteCurrentPos(QTextEdit *textEdit);
};

#endif // TEXTEDITUTILS_H

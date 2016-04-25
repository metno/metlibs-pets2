/*
  libpets2 - presentation and editing of time series

  Copyright (C) 2006-2016 met.no

  Contact information:
  Norwegian Meteorological Institute
  Box 43 Blindern
  0313 OSLO
  NORWAY
  email: diana@met.no

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "ptPainter.h"

namespace pets2 {

ptPainter::ptPainter(ptCanvas* canvas)
  : mCanvas(canvas)
{
}

void ptPainter::setLine(const ptColor& color, int width, pets2::ptLineStyle style)
{
  setColor(color);
  setLineWidth(width);
  setLineStyle(style);
}

void ptPainter::setFill(const ptColor& color, pets2::ptFillStyle style)
{
  setColor(color);
  setFillStyle(style);
}

void ptPainter::drawText(const QString& text, float x, float y)
{
  drawText(QPointF(x, y), text);
}

void ptPainter::drawText(const std::string& text, float x, float y)
{
  drawText(QString::fromStdString(text), x, y);
}

void ptPainter::drawText(const char* text, float x, float y)
{
  drawText(QString::fromLatin1(text), x, y);
}

} // namespace pets2

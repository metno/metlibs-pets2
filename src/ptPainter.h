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

#ifndef PETS2_PTPAINTER_H
#define PETS2_PTPAINTER_H 1

#include "ptFontDefs.h"
#include "ptPatterns.h"

#include <QRectF>
#include <QString>

class QImage;
class QPolygonF;

namespace pets2 {

class ptColor;

class ptCanvas {
public:
  virtual ~ptCanvas() { }

  virtual int width() = 0;
  virtual int height() = 0;

  virtual float pixWidth() const = 0;
  virtual float pixHeight() const = 0;

  virtual void update() = 0;

  virtual void setFontSize(ptFontSize fs) = 0;

  virtual QSizeF getTextSize(const QString& text) = 0;
  virtual int getTextWidth(const QString& text) = 0;
  virtual int getTextHeight() = 0;
  virtual int getCharWidth(QChar ch) = 0;
};


class ptPainter {
protected:
  ptPainter(ptCanvas* canvas);

public:
  virtual ~ptPainter() { }

  ptCanvas* canvas()
    { return mCanvas; }

  float pixWidth() const
    { return mCanvas->pixWidth(); }

  float pixHeight() const
    { return mCanvas->pixHeight(); }

  void setFontSize(ptFontSize fs)
    { mCanvas->setFontSize(fs); }

  QSizeF getTextSize(const QString& text)
    { return mCanvas->getTextSize(text); }

  int getTextWidth(const QString& text)
    { return mCanvas->getTextWidth(text); }

  int getTextHeight()
    { return mCanvas->getTextHeight(); }

  int getCharWidth(QChar ch)
    { return mCanvas->getCharWidth(ch); }

  virtual void save() = 0;
  virtual void restore() = 0;
  virtual void translate(float dx, float dy) = 0;

  virtual void setColor(const ptColor& c) = 0;
  virtual void setLineStyle(ptLineStyle style) = 0;
  virtual void setLineWidth(int lw) = 0;
  virtual void setFillStyle(ptFillStyle style) = 0;

  void setLine(const ptColor& color, int width=1, ptLineStyle style=FULL);
  void setFill(const ptColor& color, ptFillStyle style = SOLID);

  virtual void drawText(const QPointF& pos, const QString& text) = 0;
  void drawText(const QString& text, float x, float y);
  void drawText(const std::string& text, float x, float y);
  void drawText(const char* text, float x, float y);

  virtual void drawLine(float x1, float y1, float x2, float y2) = 0;
  virtual void drawPolyline(const QPolygonF& points) = 0;
  virtual void drawPolygon(const QPolygonF& points) = 0;

  virtual void drawRect(float x1, float y1, float x2, float y2) = 0;

  virtual void drawCircle(float centerx, float centery, float radius) = 0;

  /** Draw an image. The bounding rect is
   * drawRect(x, y, x+scale*image.width()*pixWidth(), y+sscale*image.height()*pixHeight());
   */
  virtual void drawImage(float x, float y, const QImage& image, float scale) = 0;

  virtual void clear(const ptColor& bg) = 0;

private:
  ptCanvas* mCanvas;
};

} // namespace pets2

#endif // PETS2_PTPAINTER_H

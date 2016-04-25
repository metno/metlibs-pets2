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

#ifndef PETS2_PTQPAINTER_H
#define PETS2_PTQPAINTER_H 1

#include "ptPainter.h"

#include <QBrush>
#include <QPainter>
#include <QPen>

#ifndef Q_DECL_OVERRIDE
#define Q_DECL_OVERRIDE
#endif

namespace pets2 {

class ptQCanvas : public ptCanvas {
public:
  ptQCanvas(QPaintDevice* device);

  int width();
  int height() Q_DECL_OVERRIDE;

  void setFontSize(ptFontSize fs) Q_DECL_OVERRIDE;

  QSizeF getTextSize(const QString& text) Q_DECL_OVERRIDE;
  int getTextWidth(const QString& text) Q_DECL_OVERRIDE;
  int getTextHeight() Q_DECL_OVERRIDE;
  int getCharWidth(QChar ch) Q_DECL_OVERRIDE;

  const QFont& font()
    { return mFont; }

  const QTransform& transform()
    { return mTransform; }

  float pixWidth() const Q_DECL_OVERRIDE
    { return mPixWidth; }

  float pixHeight() const Q_DECL_OVERRIDE
    { return mPixHeight; }

  QPaintDevice* device()
    { return mDevice; }

  void update() Q_DECL_OVERRIDE;

private:
  QPaintDevice* mDevice;
  QFont mFont;
  ptFontSize mFontSize;
  float mPixWidth, mPixHeight;
  QTransform mTransform;
};


class ptQPainter : public ptPainter {
public:
  ptQPainter(ptQCanvas* qcanvas);
  ~ptQPainter();

  QPainter& qPainter()
    { return qpainter; }

  void save() Q_DECL_OVERRIDE;
  void restore() Q_DECL_OVERRIDE;
  void translate(float dx, float dy) Q_DECL_OVERRIDE;

  void setColor(const ptColor& color) Q_DECL_OVERRIDE;
  void setLineStyle(ptLineStyle style) Q_DECL_OVERRIDE;
  void setLineWidth(int lw) Q_DECL_OVERRIDE;
  void setFillStyle(ptFillStyle style) Q_DECL_OVERRIDE;

  using ptPainter::drawText;
  void drawText(const QPointF& pos, const QString& text) Q_DECL_OVERRIDE;

  void drawLine(float x1, float y1, float x2, float y2) Q_DECL_OVERRIDE;
  void drawPolyline(const QPolygonF& points) Q_DECL_OVERRIDE;
  void drawPolygon(const QPolygonF& points) Q_DECL_OVERRIDE;

  void drawRect(float x1, float y1, float x2, float y2) Q_DECL_OVERRIDE;

  void drawCircle(float centerx, float centery, float radius) Q_DECL_OVERRIDE;

  void drawImage(float x, float y, const QImage& image, float scale) Q_DECL_OVERRIDE;

  void clear(const ptColor& bg) Q_DECL_OVERRIDE;

private:
  void updateLineDashes();
  void updateBrushPattern();

  ptQCanvas* qcanvas()
    { return static_cast<ptQCanvas*>(canvas()); }

  QPointF map(const QPointF& p)
    { return qcanvas()->transform().map(p); }

  QPointF map(float x, float y)
    { return map(QPointF(x, y)); }

  QPolygonF map(const QPolygonF& p)
    { return qcanvas()->transform().map(p); }

private:
  QPainter qpainter;

  QPen qpen;
  QBrush qbrush;

  QColor qcolor;
  ptLineStyle lineStyle;
  ptFillStyle fillStyle;
};

} // namespace pets2

#endif // PETS2_PTQPAINTER_H

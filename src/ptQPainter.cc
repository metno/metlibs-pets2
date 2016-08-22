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

#include "ptQPainter.h"

#include "ptColor.h"

#include <numeric>

namespace {

QColor qColor(const pets2::ptColor& c)
{
  return QColor::fromRgbF(c.colTable[0], c.colTable[1], c.colTable[2], c.colTable[3]);
}

#if 1
const unsigned int LineStyle[][2] = {
  {1,0xFFFF}, // FULL
  {1,0x1F1F}, // DASHED
  {1,0x0C3F}, // DASHDOTTED
  {1,0x1C47}, // DASHDASHDOTTED
  {1,0x3333}, // DOTTED
  {0,0}
};

// basically a copy of DiPaintGLPainter::LineStipple with a bit of DiPaintGLPainter::setPen
void setDashes(QPen& qpen, pets2::ptLineStyle style)
{
  const float factor = LineStyle[style][0];
  const unsigned int pattern = LineStyle[style][1];

  if (factor == 0 || pattern == 0) {
    qpen.setStyle(Qt::NoPen);
    return;
  } else if (pattern == 0xFFFF) {
    qpen.setStyle(Qt::SolidLine);
    return;
  }

  QVector<qreal> dashes;
  float dashOffset = 0;

  const bool state0 = pattern & 1;
  bool state = state0;
  int number = 0;
  for (unsigned int ii = 0, i = 1; ii < 16; ++ii, i <<= 1, number += 1) {
    const bool statei = pattern & i;
    if (statei != state) {
      dashes << number * factor;
      state = statei;
      number = 0;
    }
  }
  if (number > 1) {
    dashes << (number-1) * factor;
  }

  // Ensure that the pattern has an even number of elements by inserting
  // a zero-size element if necessary.
  if (dashes.size() % 2 == 1)
    dashes << 0;

  /* If the pattern starts with a gap then move it to the end of the
     vector and adjust the starting offset to its location to ensure
     that it appears at the start of the pattern. (This is because
     QPainter's dash pattern rendering assumes that the first element
     is a line. */
  if (!state0) {
    const qreal total = std::accumulate(dashes.begin(), dashes.end(), 0);
    dashes << dashes.first();
    dashes.pop_front();
    dashOffset = total - dashes.last();
  }
  qpen.setDashPattern(dashes);
  qpen.setDashOffset(dashOffset);
}
#else
void setDashes(QPen& qpen, pets2::ptLineStyle style)
{
  if (style == pets2::FULL)
    qpen.setStyle(Qt::SolidLine);
  else if (style == pets2::DASHED)
    qpen.setStyle(Qt::DashLine);
  else if (style == pets2::DASHDOTTED)
    qpen.setStyle(Qt::DashDotLine);
  else if (style == pets2::DASHDASHDOTTED)
    qpen.setStyle(Qt::DashDotDotLine);
  else if (style == pets2::DOTTED)
    qpen.setStyle(Qt::DotLine);
  else if (style == pets2::DASHED)
    qpen.setStyle(Qt::DashLine);
  else
    qpen.setStyle(Qt::NoPen);
}
#endif

static const float gl_width = 1500, gl_height = 1000;

float mapFontSize(pets2::ptFontSize size)
{
  if (size == pets2::HEADER1)
    return 20;
  else if (size == pets2::HEADER2)
    return 14;
  else if (size == pets2::HEADER3)
    return 11;
  else if (size == pets2::SUBSCRIPT)
    return 6;
  else // if (size == pets2::NORMAL)
    return 9;
}

} // namespace

// ------------------------------------------------------------------------

namespace pets2 {

ptQCanvas::ptQCanvas(QPaintDevice* device)
  : mDevice(device)
  , mFont(QFont(), mDevice)
  , mFontSize(NORMAL)
{
  update();
}

int ptQCanvas::width()
{
  return mDevice->width();
}

int ptQCanvas::height()
{
  return mDevice->height();
}

void ptQCanvas::update()
{
  mPixWidth = gl_width / width();
  mPixHeight = gl_height / height();
  mFont.setPointSizeF(mapFontSize(mFontSize));
  mTransform = QTransform().translate(0, height()).scale(1/mPixWidth, -1/mPixHeight);
}

void ptQCanvas::setFontSize(ptFontSize fs)
{
  if (fs != mFontSize) {
    mFontSize = fs;
    update();
  }
}

QSizeF ptQCanvas::getTextSize(const QString& text)
{
  QFontMetricsF fm(mFont, mDevice);
  const QSizeF size = fm.tightBoundingRect(text).size();
  return QSizeF(size.width() * mPixWidth, size.height() * mPixHeight);
}

int ptQCanvas::getTextWidth(const QString& text)
{
  QFontMetricsF fm(mFont, mDevice);
  return fm.width(text) * mPixWidth;
}

int ptQCanvas::getTextHeight()
{
  QFontMetricsF fm(mFont, mDevice);
  return fm.height() * mPixHeight;
}

int ptQCanvas::getCharWidth(QChar ch)
{
  QFontMetricsF fm(mFont, mDevice);
  return fm.width(ch) * mPixWidth;
}

//------------------------------------------------------------------------

ptQPainter::ptQPainter(ptQCanvas* qcanvas)
  : ptPainter(qcanvas)
  , qpainter(qcanvas->device())
  , lineStyle(pets2::FULL)
  , fillStyle(pets2::SOLID)
{
  qpen.setCapStyle(Qt::FlatCap); // important for dash patterns
}

ptQPainter::~ptQPainter()
{
}

void ptQPainter::save()
{
  qpainter.save();
}

void ptQPainter::restore()
{
  qpainter.restore();
}

void ptQPainter::translate(float dx, float dy)
{
  qpainter.translate(dx, dy);
}

void ptQPainter::setLineStyle(ptLineStyle style)
{
  if (style != lineStyle) {
    lineStyle = style;
    updateLineDashes();
  }
}

void ptQPainter::updateLineDashes()
{
  if (lineStyle == pets2::FULL) {
    qpen.setStyle(Qt::SolidLine);
  } else if (lineStyle == pets2::NOLINE) {
    qpen.setStyle(Qt::NoPen);
  } else {
    setDashes(qpen, lineStyle);
  }
}

void ptQPainter::setColor(const ptColor& c)
{
  const QColor qc = qColor(c);
  if (qcolor != qc) {
    qcolor = qc;
    qpen.setColor(qcolor);
    updateBrushPattern();
  }
}

void ptQPainter::setLineWidth(int lw)
{
  if (lw != qpen.width()) {
    qpen.setWidth(lw);
    updateLineDashes();
  }
}

void ptQPainter::setFillStyle(pets2::ptFillStyle style)
{
  if (style != fillStyle) {
    fillStyle = style;
    updateBrushPattern();
  }
}

void ptQPainter::updateBrushPattern()
{
  if (fillStyle == pets2::SOLID) {
    qbrush.setStyle(Qt::SolidPattern);
    qbrush.setColor(qcolor);
  } else if (fillStyle == pets2::NONE) {
    qbrush.setStyle(Qt::NoBrush);
  } else {
    // this is based on diana DiPaintGLPainter
    QImage mask(pets2::fillPattern(fillStyle), 32, 32, QImage::Format_Mono);
    QVector<QRgb> colours;
    colours << qRgba(0, 0, 0, 0) << qcolor.rgba();
    mask.setColorTable(colours);
    qbrush.setTextureImage(mask.mirrored());
  }
}

void ptQPainter::drawText(const QPointF& pos, const QString& text)
{
  qpainter.setPen(qpen);
  qpainter.setFont(qcanvas()->font());
  qpainter.drawText(map(pos), text);
}

void ptQPainter::drawLine(float x1, float y1, float x2, float y2)
{
  qpainter.setPen(qpen);
  qpainter.drawLine(map(x1, y1), map(x2, y2));
}

void ptQPainter::drawPolyline(const QPolygonF& points)
{
  qpainter.setPen(qpen);
  qpainter.setBrush(Qt::NoBrush);
  qpainter.drawPolyline(map(points));
}

void ptQPainter::drawPolygon(const QPolygonF& points)
{
  qpainter.setPen(qpen);
  qpainter.setBrush(qbrush);
  qpainter.drawPolygon(map(points));
}

void ptQPainter::drawRect(float x1, float y1, float x2, float y2)
{
  qpainter.setPen(qpen);
  qpainter.setBrush(qbrush);
  qpainter.drawRect(QRectF(map(x1, y1), map(x2, y2)));
}

void ptQPainter::drawCircle(float centerx, float centery, float radius)
{
  qpainter.setPen(qpen);
  qpainter.setBrush(qbrush);

  const float r = radius / qcanvas()->pixWidth();
  qpainter.drawEllipse(map(centerx, centery), r, r);
}

void ptQPainter::drawImage(float x, float y, const QImage& image, float scale)
{
  const QSizeF rs = scale * image.size();
  const QPointF xy = map(x, y) + QPointF(0, -rs.height());
  qpainter.drawImage(QRectF(xy, rs), image);
}

void ptQPainter::clear(const ptColor& bg)
{
  qpainter.fillRect(0, 0, canvas()->width(), canvas()->height(), qColor(bg));
}

} // namespace pets2


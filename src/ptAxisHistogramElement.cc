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

// ptAxisHistogramElement.cc : Definitions for AxisHistogramElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptAxisHistogramElement.h"

#include "ptPatterns.h"

#include <puTools/miTime.h>
#include <cfloat>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

AxisHistogramElement::AxisHistogramElement(yAxisElement* ya,
    const DataSpec cds, const ptVertFieldf& field, const Layout& layout,
    XAxisInfo* xtime)
    : AxisChildElement(ya, cds, field, layout, xtime)
    , lineWidth(layout.lineWidth)
    , fstyle(layout.fillstyle)
    , hstart(layout.histStart)
    , hstop(layout.histStop)
    , drawlabel(layout.label)
{
#ifdef DEBUG
  cout << "Inside AxisHistogramElement's constructor" << endl;
#endif
  type = AXISHIST;
}

void AxisHistogramElement::plot(ptPainter& painter)
{
  if (enabled && Yaxis && visible) {
#ifdef DEBUG
    cout << "AxisHistogramElement::plot(ptPainter& painter)" <<endl;
#endif
    _prePlot();
    painter.setFontSize(fontSize);
    const float tw = painter.getCharWidth(QLatin1Char('0'));
    painter.setColor(color);
    painter.setLineWidth(lineWidth);
    painter.setFillStyle(fstyle);

    int j = datastart();

    float oldx = xtime->xcoord[startT];
    for (int i = startT; i <= stopT; i++)
      if (valid(i)) {
        float newx = xtime->xcoord[i];
        if (dval(j) >= 0.05) { // don't draw if is ignorable
          float y = yval(j);
          float del = (newx - oldx) / 10.0;
          float x1 = oldx + hstart * del;
          float x2 = oldx + hstop * del;
          painter.drawRect(x1, startY, x2, y);
          if (drawlabel) {
            const QPointF pos(x1 + (x2 - x1) / 2 - tw, y + YSPACE);
            const QString text = QString("%1").arg(dval(j), 2, 'f', 1);
            painter.drawText(pos, text);
          }
        }
        oldx = newx;
        j++;
      }
  }
}

} // namespace pets2

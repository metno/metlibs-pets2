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


// ptAxesElement.cc : Definitions for AxesElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptAxesElement.h"

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

AxesElement::AxesElement(const ptVertFieldf& field,
    const Layout& layout, const bool& hor, XAxisInfo* xtime)
  : PlotElement(layout, field, xtime)
  , y1(field.y1+layout.tickLen), y2(field.y2-layout.labelSpace)
  , axisWidth(layout.axisWidth), tickLineWidth(layout.tickWidth)
  , tickColor(layout.color2)
  , rectangle(layout.axisRectangle)
  , horizontal(hor)
{
#ifdef DEBUG
  cout << "Inside AxesElement's constructor" << endl;
#endif
  type=XAXIS;
}


void AxesElement::plot(ptPainter& painter)
{
  if (!enabled)
    return;

#ifdef DEBUG
  cout << "AxesElement::plot(ptPainter& painter)" <<endl;
#endif
  if (horizontal) {
    x1 = xtime->x1;
    x2 = xtime->x2;
  }
  painter.setLine(color, axisWidth);
  painter.setFillStyle(pets2::NONE);

  if (rectangle) {
    painter.drawRect(x1, y1, x2, y2);
  }

  if (horizontal) {
    // draw the x-axis with tick marks
    painter.drawLine(x1, y1, x2, y1);   // axis

    painter.setLine(tickColor, tickLineWidth);
    for(int i=startT; i<=stopT; i++) {            // tick marks
      painter.drawLine(xtime->xcoord[i], startY, xtime->xcoord[i], y1);
    }
  }
}

} // namespace pets2

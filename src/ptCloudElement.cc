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


// ptCloudElement.cc : Definitions for CloudElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptCloudElement.h"

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

CloudElement::CloudElement(const DataSpec cds,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : dataPlotElement(cds, layout, field, xtime),
    style(layout.fillstyle), width(layout.lineWidth),
    text(layout.text)
{
#ifdef DEBUG
  cout << "Inside CloudElement's constructor" << endl;
#endif
  type = CLOUDBOX;
}

// Plotting of cloud boxes could probably be made more effective
void CloudElement::plot(ptPainter& painter)
{
  if(enabled && visible) {
#ifdef DEBUG
    cout << "CloudElement::plot(ptPainter& painter)" <<endl;
#endif

    painter.setLine(color, width);

    // find minimum delta T for calculation of cloud box width
    int k = startT;
    float minx = 10000;
    for (int i = startT+1; i <= stopT; i++) {
      if (valid(i)) {
        float testx = xtime->xcoord[i]-xtime->xcoord[k];
        if (testx < minx)
          minx = testx;
        k = i;
      }
    }
    float deltaX = minx/3.0;
    float mostleft= 10000;

    int j = datastart();
    for (int i=startT; i<=stopT; i++) {
      if (valid(i)) {
        float x = xtime->xcoord[i];
        float left  = x - deltaX;
        float right = x + deltaX;
        // draw rectangular box
        painter.setFillStyle(pets2::NONE);
        painter.drawRect(right, startY, left, stopY);
        // draw each cloud box
        painter.setFillStyle(style);
        painter.drawRect(left, startY, left+(2.0*deltaX*dval(j)/100.0), stopY);
        j++;
        if (left < mostleft)
          mostleft= left;
      }
    }
    // print text
    if (!text.empty()){
      const QString qtext = QString::fromStdString(text);
      const float tw = painter.getTextWidth(qtext);
      painter.drawText(QPointF(mostleft - tw -2, startY), qtext);
    }
  }
}

} // namespace pets2

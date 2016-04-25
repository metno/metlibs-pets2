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

// ptHistogramElement.cc : Definitions for HistogramElement class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptHistogramElement.h"

#include "ptPatterns.h"

#include <puTools/miTime.h>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

HistogramElement::HistogramElement(const DataSpec cds,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime,
    std::vector<miTime>& timeLine)
    : dataPlotElement(cds, layout, field, xtime)
    , deltaY(field.y2 - startY - layout.labelSpace)
    , lineWidth(layout.lineWidth)
    , style(layout.fillstyle)
    , unit(layout.unit)
    , hstart(layout.histStart)
    , hstop(layout.histStop)
    , drawlabel(layout.label)
{
#ifdef DEBUG
  cout << "Inside HistogramElement's constructor" << endl;
#endif
  type = HIST;
  deltaY -= YSPACE;


//make array of time differences
  int nt = xtime->xcoord.size();
  deltaT = new int[nt];

//find scaling of boxes. 12mm is default maximum if delta T = 3
  max = 12.0;
  int j = datastart();

  for (int i = 1; i < nt; i++) {
    if (valid(i)) {
      deltaT[i] = miTime::hourDiff(timeLine[i], timeLine[j]);
#ifdef DEBUG
      cout << "Times: " << timeLine[i] << " - " << timeLine[j] << "  Histogram value:" << dval(j) << " deltaT:" << deltaT[i] << endl;
#endif
      if (dval(j) > deltaT[i] * max / 3.0)
        max = dval(j) * 3.0 / (deltaT[i] * 1.0);
      j++;
    }
  }
// hack..don't know timedifference for first timepoint
  if (nt > 1)
    deltaT[0] = deltaT[1];
  else
    deltaT[0] = 1;
#ifdef DEBUG
  cout << "Histogram constructor: max:"<<max<<endl;
  cout << "deltaT:"<<endl;
  for (int i=0; i<nt;i++)
    cout<<" DELTAT["<<i<<"] = " << deltaT[i]<<endl;
#endif
}

HistogramElement::~HistogramElement()
{
  delete[] deltaT;
}

// Currently assuming only one histogramelement (should use numHists to control
// plotting)
void HistogramElement::plot(ptPainter& painter)
{
  if (enabled && visible) {
#ifdef DEBUG
    cout << "HistogramElement::plot(ptPainter& painter)" <<endl;
#endif
    painter.setFontSize(fontSize);
    const QSizeF bbx0 = painter.getTextSize("0");
    float deltay = deltaY - bbx0.height();

    float DELTAX = bbx0.width();

    painter.setLine(color, lineWidth);
    painter.setFillStyle(style);

    int j = datastart();

    float oldx = xtime->xcoord[startT];
    for (int i = startT; i <= stopT; i++)
      if (valid(i)) {
        float newx = xtime->xcoord[i];
        if (dval(j) >= 0.05) { // don't draw if is ignorable
          float y = startY + deltay * dval(j) * 3 / (deltaT[i] * max);
#ifdef DEBUG
          cout << "histogram value:" << dval(j) << " startY:"<<startY
               <<" deltaY:"<<deltay<<" deltaT["<<i<<"]:"
               <<deltaT[i]<<" max:"<<max
               <<" verdi:"<<y<<endl;
#endif
          float del = (newx - oldx) / 10.0;
          float x1 = oldx + hstart * del;
          float x2 = newx - (10 - hstop) * del;
          painter.drawRect(x1, startY, x2, y);
          if (drawlabel) {
            QPointF pos(x1 + (x2 - x1) / 2 - DELTAX, y + YSPACE);
            QString text = QString("%1").arg(dval(j), 3, 'f', 1);
            painter.drawText(pos, text);
          }
        }
        oldx = newx;
        j++;
      }
  }
}

} // namespace pets2

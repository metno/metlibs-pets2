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


// ptGridlineElement.cc : Definitions for GridlineElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptGridlineElement.h"

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

GridlineElement::GridlineElement(const std::vector<miTime>& tline,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : PlotElement(layout,field,xtime)
  , numTickMajor(layout.numTickMajor)
  , numTickMinor(layout.numTickMinor)
  , axeStopY(field.y2)
  , labelSpace(layout.labelSpace)
  , lineWidth(layout.lineWidth)
  , style(layout.linePattern)
  , gridxonly(layout.gridxonly)
  , plotDayPattern(layout.plotDayPattern)
  , useTimes(layout.useTimes)
  , daycolor(layout.color2)
  , timeLine(tline)
{
#ifdef DEBUG
  cout << "Inside GridlineElement's constructor" << endl;
#endif
  type=GRIDLINE;
  deltaY = axeStopY - startY - labelSpace;

  if (plotDayPattern){
    // find all daychanges in timeline
    int n= timeLine.size();
    daychanges.push_back(0);
    for (int i=1; i<n; i++)
      if (timeLine[i].day()!=timeLine[i-1].day())
        daychanges.push_back(i);
    if (daychanges[daychanges.size()-1]!=n-1)
      daychanges.push_back(n-1);
  }
}


void GridlineElement::plot(ptPainter& painter)
{
  if(enabled && visible) {
#ifdef DEBUG
    cout << "GridlineElement::plot(ptPainter& painter)" <<endl;
#endif
    painter.setLine(color, lineWidth, style);
    painter.setFillStyle(pets2::SOLID);

    bool plotday= true;
    std::vector<int> daych;
    int i;

    if (!gridxonly){
      float matick = deltaY/(numTickMajor*1.0);
      float fy;

      fy = startY;
      for (i=0; i<numTickMajor;i++){
        fy+= matick;

        painter.drawLine(xtime->x1, fy, xtime->x2, fy);
      }
    }

    int d,n= daychanges.size();
    int d1, d2;
    if (plotDayPattern)
      for (d=0; d<n && daychanges[d]<=startT; d++)
        plotday= !plotday;

    int i1= (xtime->xcoord[startT] == xtime->x1 ? startT+1 : startT);
    int i2= (xtime->xcoord[stopT]  == xtime->x2 ? stopT-1 : stopT);

    painter.setLine(daycolor);
    for (i=i1; i<=i2; i++) {
      // plot alternating day-backgrounds
      if (plotDayPattern){
        d2= daychanges[d];
        if (i==d2){
          if (plotday) {
            d1= daychanges[d-1];
            if (d1<startT) d1= startT;
            painter.drawRect(xtime->xcoord[d1], startY+2, xtime->xcoord[d2], startY+deltaY-2);
          }
          plotday= !plotday;
          d++;
        }
      }
    }
    // plot last day-background
    if (plotDayPattern && plotday){
      d2= stopT;
      if (daychanges[d]<d2)
        d1= daychanges[d];
      else
        d1= daychanges[d-1];
      if (d1<startT)
        d1= startT;

      painter.drawRect(xtime->xcoord[d1], startY+2, xtime->xcoord[d2], startY+deltaY-2);
    }

    painter.setFillStyle(pets2::NONE);
    painter.setLine(color, lineWidth, style);
    for (i=i1; i<=i2; i++) {
      // plot lines on hours modulo useTimes
      if (useTimes >= 0)
        if ((timeLine[i].hour() % useTimes)==0 && timeLine[i].min()==0) {
          painter.drawLine(xtime->xcoord[i], startY, xtime->xcoord[i], startY+deltaY);
        }
    }
  }
}

} // namespace pets2

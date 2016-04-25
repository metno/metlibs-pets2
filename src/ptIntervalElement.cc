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


// ptIntervalElement.cc : Definitions for IntervalElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptIntervalElement.h"

#include <puTools/miStringFunctions.h>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

IntervalElement::IntervalElement(const std::vector<miTime>& tline,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : PlotElement(layout, field, xtime)
  , label(layout.text)
  , timetext(layout.text2)
  , arrows(layout.horsides)
  , lineWidth(layout.lineWidth)
  , timeLine(tline)
{
#ifdef DEBUG
  cout << "Inside IntervalElement's constructor" << endl;
#endif
  type=INTERVAL;

  if (!timetext.empty())
    vt= miutil::split(timetext, ";");
}


void IntervalElement::setTimes(const std::vector<tinterval>& t)
{
  intervals= t;
  if(vt.size()){
    int n= intervals.size();
    int m= vt.size();
    for (int i=0; i<n; i++){
      if (i<m){
        intervals[i].text = miutil::split(vt[i], "|");
      }
    }
  }
}


void IntervalElement::plot(ptPainter& painter)
{
  if(enabled && visible) {
#ifdef DEBUG
    cout << "IntervalElement::plot(ptPainter& painter)" <<endl;
#endif
    painter.setFontSize(fontSize);
    painter.setLine(color, lineWidth, style);

    int i;
    float th,tw;
    float xstart, midY= (startY+stopY)/2;
    float deltaY= stopY-startY;
    for (size_t j=0; j<intervals.size(); j++){
      xstart= -1;
      for (i=startT; i<=stopT; i++){
        if (timeLine[i]==intervals[j].start)
          xstart= xtime->xcoord[i];
        if (timeLine[i]==intervals[j].stop &&
            xstart>=0){
          float t1;
          int numt= intervals[j].text.size();
          float maxw=0;
          if (numt>0){
            float dy= deltaY/(numt*2.0);
            float tY= stopY-dy;
            for (int k=0; k<numt; k++){
              const QString qtext = QString::fromStdString(intervals[j].text[k]);
              const QSizeF bbx = painter.getTextSize(qtext);

              t1= (xstart + xtime->xcoord[i] - bbx.width())/2;
              painter.drawText(QPointF(t1,tY-bbx.height()/2), qtext);
              if (bbx.width() > maxw)
                maxw = bbx.width();
              tY-= dy*2;
            }
          }
          if (miTime::hourDiff(intervals[j].stop,intervals[j].start)<4)
            continue;
          t1= (xstart + xtime->xcoord[i] - maxw)/2;
          float a11,a12,a21,a22;
          a11= xstart; a12= t1 - 10;
          a21= t1 + maxw + 10; a22= xtime->xcoord[i];
          painter.drawLine(a11, midY, a12, midY);
          painter.drawLine(a21, midY, a22, midY);
          break;
        }
      }
    }
  }
}

} // namespace pets2

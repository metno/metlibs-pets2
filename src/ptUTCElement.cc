/*
  libpets2 - presentation and editing of time series

  Copyright (C) 2013-2016 met.no

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


// ptUTCElement.cc : Definitions for UTCElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptUTCElement.h"

#include <iomanip>
#include <sstream>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

UTCElement::UTCElement(const std::vector<miTime>& tline,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : PlotElement(layout, field, xtime)
  , minSkipX(layout.minSkipX)
  , label(layout.label)
  , text(layout.text)
  , modhours(layout.modhours)
{
#ifdef DEBUG
  cout << "Inside UTCElement's constructor" << endl;
#endif
  type=UTC;
  timeLine = tline;
  if (label && text.empty())
    text= "UTC";
  if (modhours.empty()) {
    modhours.push_back(12);
    modhours.push_back(6);
    modhours.push_back(3);
    modhours.push_back(1);
  }
}


float UTCElement::plottime(ptPainter& painter, const miTime& t, int i, bool minute, float cwid)
{
  std::ostringstream ost;
  float offset;
  if (minute){
    ost << std::setw(2) << std::setfill('0') << t.hour() << ":" << std::setw(2) << t.min();
    offset= cwid*2.5;
  } else {
    ost << std::setw(2) << std::setfill('0') << t.hour();
    offset= cwid;
  }
  painter.drawText(QPointF(xtime->xcoord[i]-offset,startY), QString::fromStdString(ost.str()));
  return (xtime->xcoord[i]+offset);
}

void UTCElement::plot(ptPainter& painter)
{
  if (enabled && visible) {
    painter.setFontSize(fontSize);
    const float tw = painter.getCharWidth('0');

    int interv= miTime::minDiff(timeLine[stopT],timeLine[startT]);
    bool plotmin= (interv < 3*60);
    bool plotallmin= (interv < 3*60 && (stopT-startT) < 20);
    float prev;
    float minw= painter.pixWidth();//minSkipX*pixWidth;
    float guess= tw*(plotmin ? 2.5 : 1.0);

    painter.setLine(color);
    bool *taken= new bool[stopT+2];
    for (int i=0; i<stopT+2; i++) taken[i]= false;

    for (int k=0; k<modhours.size(); k++) {
      prev= -1000;
      for (int i=startT; i<=stopT; i++) {
        if (!taken[i]){
          miTime t= timeLine[i];
          if (t.hour() % modhours[k] == 0){
            if (!plotallmin)
              if (t.min()!=0)
                continue;
            if ((xtime->xcoord[i]-guess-prev) < minw)
              continue;
            if ( k!=0 ){
              // find next taken
              int nextt=i+1;
              for (int j=i+1; j<stopT; j++){
                if (taken[j]){
                  nextt=j;
                  break;
                }
              }
              if (taken[nextt])
                if (((xtime->xcoord[nextt]-guess)-(xtime->xcoord[i]+guess)) < minw)
                  continue;
            }
            prev= plottime(painter, t,i,plotmin,tw);
            taken[i]= true;
          }
        } else
          prev= xtime->xcoord[i] + guess;
      }
    }

    delete[] taken;
    painter.drawText(QPointF(20,startY), QString::fromStdString(text));
  }
}

} // namespace pets2

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


// ptDayElement.cc : Definitions for DayElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptDayElement.h"
#include <puTools/miStringFunctions.h>

#include <list>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;
using namespace std;

namespace pets2 {

DayElement::DayElement(const vector<miTime>& tline,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : PlotElement(layout, field, xtime),
    timeLine(tline),
    language(layout.language), asNumber(layout.asNumber)
{
#ifdef DEBUG
  cout << "Inside DayElement's constructor" << endl;
#endif
  type=DAY;
  lang= (language=="NO" ? miDate::Norwegian :  miDate::English);
}


std::string DayElement::dataAsString(const miDate& date)
{
  if (asNumber)
    return miutil::from_number(date.dayOfYear());
  else
    return date.shortweekday(lang);
}


void DayElement::plot(ptPainter& painter)
{
  miDate curDate = timeLine[startT].date();
  std::string curDay;
  float offset,th;
  float prevf;       // end of previous text
#ifdef DEBUG
  cout << "DayElement::plot(ptPainter& painter)" << endl;
#endif
  if (enabled && visible) {
    painter.setFontSize(fontSize);
    painter.setLine(color);
    prevf= 0;

    // print date for each new day
    // (centered on each day - or preferably under 12-hour)
    bool active= false;
    float startx=startT, stopx;
    list<int> li12;
    // data may have seconds sampling time in the future
    for (int i=startT; i<=stopT; i++){
      // check data time, 12:00:00, 12:00:01, 12:30:00
      if (timeLine[i].hour() == 12){
        if (i > 0) {
          if (miTime::hourDiff(timeLine[i], timeLine[i-1]) > 0) {
            li12.push_back(i);
          }
        } else {
          li12.push_back(i);
        }
      }
      if (i!=startT && timeLine[i].date()==timeLine[i-1].date())
        continue;

      if (active){
        // print previous day
        float sx;
        if (li12.size() > 0){
          int i12= *(li12.begin());
          li12.pop_front();
          sx= xtime->xcoord[i12] - offset;
        } else {
          stopx= xtime->xcoord[i];
          sx= (stopx+startx)/2.0 - offset;
        }
        // print if room
        if (sx >= prevf){
          painter.drawText(curDay, sx, startY);
          prevf= sx+2*offset;
        }
      }
      curDate = timeLine[i].date();
      curDay = dataAsString(curDate);

      const QString qtext = QString::fromStdString(curDay);
      const float tw = painter.getTextWidth(qtext);

      startx= xtime->xcoord[i];
      offset = tw/2;
      active=true;
    }

    // print last day also
    if (active){
      // print previous day
      float sx;
      if (li12.size() > 0){
        int i12= *(li12.begin());
        sx= xtime->xcoord[i12] - offset;
      } else {
        stopx= xtime->xcoord[stopT];
        sx= (stopx+startx)/2.0 - offset;
      }
      // print if room
      if (sx >= prevf){
        painter.drawText(curDay, sx, startY);
      }
    }

  }
}

} // namespace pets2

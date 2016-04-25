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


// ptDateElement.cc : Definitions for DateElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptDateElement.h"

#include <puTools/miStringFunctions.h>

#include <list>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace std;
using namespace miutil;

namespace pets2 {

DateElement::DateElement(const std::vector<miTime>& tline,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)

  : PlotElement(layout, field, xtime)
  , timeLine(tline)
  , labeltext(layout.text)
  , language(layout.language)
  , asNumber(layout.asNumber)
  , datestyle(layout.datestyle)
  , plotlabel(layout.label)
  , minSkipX(layout.minSkipX)
{
#ifdef DEBUG
  cout << "Inside DateElement's constructor" << endl;
#endif
  type=DATE;
  lang= (language=="NO" ? miDate::Norwegian :  miDate::English);

  if (plotlabel && labeltext.empty()) {
    if (datestyle == DS_DATE)
      labeltext= (lang==miDate::English ? "Date" : "Dato");
    else if (datestyle == DS_WEEK)
      labeltext= (lang==miDate::English ? "Week" : "Uke");
    else if (datestyle == DS_MONTH)
      labeltext= (lang==miDate::English ? "Month" : "Måned");
    else if (datestyle == DS_DAY)
      labeltext= (lang==miDate::English ? "Day" : "Dag");
    else if (datestyle == DS_YEAR)
      labeltext= (lang==miDate::English ? "Year" : "År");
  }
}

std::string DateElement::dataAsString(const miDate& date)
{
  std::string txt;
  if (datestyle == DS_DATE) {
    txt = miutil::from_number(date.day()) + "/" + miutil::from_number(date.month());
  } else if (datestyle == DS_WEEK){
    txt = miutil::from_number(date.weekNo());
  } else if (datestyle == DS_MONTH){
    if (asNumber)
      txt= miutil::from_number(date.month());
    else
      txt= date.shortmonthname(lang);
  } else if (datestyle == DS_DAY){
    if (asNumber)
      txt= miutil::from_number(date.dayOfYear());
    else
      txt= date.shortweekday(lang);
  } else if (datestyle == DS_YEAR){
    txt= miutil::from_number(date.year());
  }
  return txt;
}

void DateElement::plot(ptPainter& painter)
{
  std::string txt;
  miDate curDate = timeLine[startT].date();
  float offset,th;
  float prevf; // end of previous text

  if (enabled && visible) {
#ifdef DEBUG
    cout << "DateElement::plot(ptPainter& painter)" << endl;
#endif
    painter.setFontSize(fontSize);
    painter.setLine(color);

    // print label on left margin
    prevf= 0;
    if (plotlabel){
      txt= labeltext;
      if (labeltext=="$YEAR")
        txt = miutil::from_number(curDate.year());

      const QString qtext = QString::fromStdString(txt);
      const float tw = painter.getTextWidth(qtext);
      painter.drawText(qtext, 20, startY);
      prevf= 20 + tw;
    }

    // print date for each new day/week/month/year
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

      if (i!=startT){
        if (timeLine[i].date()==timeLine[i-1].date())
          continue;
        if (datestyle==DS_WEEK){
          li12.clear();
          if (timeLine[i].weekNo()==timeLine[i-1].weekNo() &&
            timeLine[i].year()==timeLine[i-1].year())
              continue;
        } else if (datestyle==DS_MONTH){
          li12.clear();
          if (timeLine[i].month()==timeLine[i-1].month() &&
            timeLine[i].year()==timeLine[i-1].year())
              continue;
        } else if (datestyle==DS_YEAR){
          li12.clear();
          if (timeLine[i].year()==timeLine[i-1].year())
            continue;
        }
      }

      if (active){
        // print previous day/week/month/year
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
          painter.drawText(txt, sx, startY);
          prevf= sx+2*offset;
        }
      }

      curDate = timeLine[i].date();
      txt= dataAsString(curDate);

      const QString qtext = QString::fromStdString(txt);
      const float tw = painter.getTextWidth(qtext);

      startx= xtime->xcoord[i];
      offset = tw / 2;
      active=true;
    }

    // print last date also
    if (active){
      // print previous day...
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
        painter.drawText(txt, sx, startY);
      }
    }
  }
}

} // namespace pets2

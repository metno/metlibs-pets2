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


// ptProgElement.cc : Definitions for ProgElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptProgElement.h"

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

ProgElement::ProgElement(std::vector<int>& data,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : PlotElement(layout, field, xtime),
    timeData(data), minSkipX(layout.minSkipX)
{
#ifdef DEBUG
  cout << "Inside ProgElement's constructor" << endl;
#endif
  type=PROG;
}


void ProgElement::plot(ptPainter& painter)
{
  char text[6], tmp[4];
  painter.setFontSize(fontSize);
  const float tw = painter.getCharWidth('0');

  float numWidth  = tw/2.0;
  float numWidth2 = numWidth*2;
  float numWidth3 = numWidth*3;
  float numWidth4 = numWidth*4;
  float signWidth = numWidth*0.8;

  float offset, accum=0, signw;
  float deltaT=(xtime->xcoord.size() ? xtime->xcoord[1]-xtime->xcoord[0] : 1);

  if(enabled && visible) {
#ifdef DEBUG
    cout << "ProgElement::plot(ptPainter& painter)" <<endl;
#endif
    if (timeData[0]>=0) {
      strcpy(text,"+");
      signw = signWidth;
    }
    else {
      text[0] = '\0';
      signw = 0;
    }

    painter.setLine(color);
    //print first prog time
    snprintf(tmp,sizeof(tmp),"%d",timeData[0]);
    if (strlen(tmp)==1)
      offset = signw + numWidth;
    else if (strlen(tmp)==2)
      offset = signw + numWidth2;
    else if (strlen(tmp)==3)
      offset = signw + numWidth3;
    else if (strlen(tmp)==4)
      offset = signw + numWidth4;
    strcat(text,tmp);
    painter.drawText(text, xtime->xcoord[0]-offset, startY);

    text[1] = '\0';

    for (int i=1; i<xtime->xcoord.size() && i<timeData.size(); i++) {
      // if step is too small, don't print value
      if (accum+deltaT < minSkipX * painter.pixWidth()) {
	accum += deltaT;
	deltaT = xtime->xcoord[i+1]-xtime->xcoord[i];
	continue;
      }
      if (timeData[i]>=0) {
	strcpy(text,"+");
	signw = signWidth;
      }
      else {
	text[0] = '\0';
	signw = 0;
      }
      snprintf(tmp,sizeof(tmp),"%d",timeData[i]);
      if (strlen(tmp)==1)
	offset = signw + numWidth;
      else if (strlen(tmp)==2)
	offset = signw + numWidth2;
      else if (strlen(tmp)==3)
	offset = signw + numWidth3;
      else if (strlen(tmp)==4)
	offset = signw + numWidth4;

      strcat(text,tmp);
      painter.drawText(text, xtime->xcoord[i]-offset, startY);

      if (i == xtime->xcoord.size()-1)
	break;
      deltaT = xtime->xcoord[i+1]-xtime->xcoord[i];
      accum = 0;
    }
    painter.drawText("prog", 20, startY);
  }
}

} // namespace pets2

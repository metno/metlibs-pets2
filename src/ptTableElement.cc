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


// ptTableElement.cc : Definitions for TableElement class

#include "ptTableElement.h"

#define MILOGGER_CATEGORY "metlibs.pets2.TableElement"
#include <miLogger/miLogging.h>

using namespace miutil;

namespace pets2 {

TableElement::TableElement(const DataSpec cds,
    const ptVertFieldf& field, const Layout& layout,     XAxisInfo* xtime)
  : dataPlotElement(cds, layout, field, xtime)
  , label(layout.text)
  , pformat(layout.pformat)
  , skipX(layout.minSkipX)
  , vectorF(layout.vectorF)
  , cutoff(layout.cutoff)
{
  METLIBS_LOG_SCOPE();
  type=TABLE;
}

void TableElement::plot(ptPainter& painter)
{
  METLIBS_LOG_SCOPE();
  if(enabled && visible) {
    int i;
    int j=-1, skip=0;
    char text[6];
    float dir, str; // direction and strength
    int idir;       // integer direction
    const char direction[8][3] =
    {"N","NE","E","SE","S","SW","W","NW"};
    char temp[20];

    painter.setFontSize(fontSize);
    painter.setColor(color);

    j = datastart();

    for (i=startT; i<=stopT; i++)
      if (valid(i)) {
        if (++skip >= skipX){
          skip=0;
          if (vectorF) {
            str = dval(j,0);
            if (str>cutoff) {
              dir = dval(j,1);
              idir = int((dir+22.5) / 45.0);
              snprintf(temp, sizeof(temp), pformat.c_str(), str);
              if (idir >= 8) idir = 7;
              if (idir>=0)
                strcpy(text,direction[idir]);
              else strcpy(text,"");
              strcat(text,temp);
            } else
              strcpy(text,"-");
          } else {
            str = dval(j);
            if (str>cutoff)
              snprintf(text, sizeof(text), pformat.c_str(), str);
            else
              strcpy(text,"-");
          }
          const QString qtext = QString::fromStdString(text);
          const float tw = painter.getTextWidth(qtext);
          painter.drawText(QPointF(xtime->xcoord[i] - tw/2, startY), qtext);
        }
        j++;
      }
    painter.drawText(QPointF(20,startY), QString::fromStdString(label));
  }
}

} // namespace pets2

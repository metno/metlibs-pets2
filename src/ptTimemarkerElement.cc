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

// ptTimemarkerElement.cc : Definitions for TimemarkerElement class

#include "ptTimemarkerElement.h"

#define MILOGGER_CATEGORY "metlibs.pets2.TimemarkerElement"
#include <miLogger/miLogging.h>

using namespace miutil;

namespace pets2 {

TimemarkerElement::TimemarkerElement(const std::vector<miTime>& tline,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : PlotElement(layout,field,xtime)
  , axeStopY(field.y2)
  , labelSpace(layout.labelSpace)
  , lineWidth(layout.lineWidth)
  , style(layout.linePattern)
  , timeLine(tline)
{
  METLIBS_LOG_SCOPE();
  type= TIMEMARKER;
  deltaY= axeStopY - startY - labelSpace;
}


void TimemarkerElement::plot(ptPainter& painter)
{
  METLIBS_LOG_SCOPE();
  if(enabled && markTimes.size() && visible) {
    painter.setLine(color, lineWidth, style);

    int i,j,n=markTimes.size();
    float xc;
    for (j=0; j<n; j++){
      for (i=startT; i<=stopT; i++){

        if (timeLine[i]==markTimes[j]){
          // exact match
          xc= xtime->xcoord[i];
        } else if (i<stopT && timeLine[i] < markTimes[j] &&
            timeLine[i+1] > markTimes[j]){
          // between two timepoints
          int a= abs(miTime::minDiff(timeLine[i],timeLine[i+1]));
          int b= abs(miTime::minDiff(markTimes[j],timeLine[i]));
          float c= xtime->xcoord[i+1] - xtime->xcoord[i];
          xc = xtime->xcoord[i] + c * (float(b)/float(a));
        } else if (timeLine[i] > markTimes[j]){
          // outside region - skip this one
          break;
        } else {
          // try next timestep
          continue;
        }

        painter.drawLine(xc,startY, xc,startY+deltaY);
        break;
      }
    }
  }
}

} // namespace pets2

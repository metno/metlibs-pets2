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


// ptQBoxElement.cc : Definitions for QBoxElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptQBoxElement.h"

#include <QPolygonF>

#include <cmath>
#include <cfloat>

namespace pets2 {

QBoxElement::QBoxElement(yAxisElement* ya, const DataSpec cds,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : AxisChildElement(ya,cds,field,layout,xtime)
  , lineWidth(layout.lineWidth)
  , hstart(layout.histStart), hstop(layout.histStop)
  , tickLen(layout.tickLen)
  , linestyle(layout.linePattern)
  , linecolor(layout.color)
  , boxcolor(layout.color2)
  , boxfill(layout.fillstyle)
{
#ifdef DEBUG
  cout << "Inside QBOXElement's constructor" << endl;
#endif
  type=QBOX;
}


void QBoxElement::dataInfo(float &min, float &max)
{
  float mi= FLT_MAX, ma= -FLT_MAX;
  int comp, ndim= datadimension();
  for (comp=0; comp<ndim; comp++){
    min = datamin(comp);
    max = datamax(comp);
    if (min<mi) mi= min;
    if (max>ma) ma= max;
  }
  min= mi;
  max= ma;
}

void QBoxElement::plot(ptPainter& painter)
{
  if(enabled && Yaxis) {
#ifdef DEBUG
    cout << "QBoxElement::plot(ptPainter& painter)" << endl;
#endif

    _prePlot();

    if (boxfill != pets2::NONE) {
      painter.setFill(boxcolor, boxfill);
      painter.setLineStyle(pets2::NOLINE);
    } else {
      painter.setLine(linecolor, lineWidth, linestyle);
      painter.setFillStyle(pets2::NONE);
    }

    float prevx = xtime->x1; //xval(startT);
    float newx;
    int i, j = datastart();

    for (i=startT; i<=stopT; i++) {
      if (valid(i)) {
        newx= xval(i);
        float del= (newx-prevx)/10.0;
        float x1 = prevx+hstart*del;
        float x2 = prevx+hstop*del;
        float xm = (x1 + x2)/2.0;
        float tx1= xm- (xm-x1)*tickLen;
        float tx2= xm+ (x2-xm)*tickLen;

        float y0 = yval(j,0);
        float y1 = yval(j,1);
        float y2 = yval(j,2);
        float y3 = yval(j,3);
        float y4 = yval(j,4);

        // draw vertical lines
        painter.setLine(linecolor, lineWidth, linestyle);
        painter.drawLine(xm, y0, xm, y1);
        painter.drawLine(xm, y3, xm, y4);

        // draw horisontal lines
        painter.drawLine(tx1,y0,tx2,y0);
        painter.drawLine(tx1,y4,tx2,y4);

        // draw box
        painter.setFill(boxcolor, boxfill);
        painter.setLineStyle(pets2::NOLINE);
        painter.drawRect(x1, y1, x2, y3);

        // draw box frame
        painter.setLine(linecolor, lineWidth, linestyle);
        QPolygonF line;
        line << QPointF(x1,y1)
             << QPointF(x1,y3)
             << QPointF(x2,y3)
             << QPointF(x2,y1)
             << QPointF(x1,y1);
        painter.drawPolyline(line);

        // draw 50-line
        line.clear();
        line << QPointF(x1,y2)
             << QPointF(x2,y2);
        painter.drawPolyline(line);

        prevx= newx;
        j++;
      }
    }

#ifdef DEBUG
    cout << "QBoxElement::plot(ptPainter& painter) finished" << endl;
#endif
  }
}

} // namespace pets2

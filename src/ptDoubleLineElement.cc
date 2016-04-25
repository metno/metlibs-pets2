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


// ptLineElement.cc : Definitions for LineElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptDoubleLineElement.h"

#include <QPolygonF>

#include <cmath>
#include <cfloat>

using namespace std;

namespace pets2 {

DoubleLineElement::DoubleLineElement(yAxisElement* ya, const DataSpec cds,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : AxisChildElement(ya,cds,field,layout,xtime)
  , fillColor(layout.color2)
{
#ifdef DEBUG
  cout << "Inside DoubleLineElement's constructor" << endl;
#endif
  type=DOUBLE_LINE;
}

void DoubleLineElement::dataInfo(float &min, float &max)
{
  float mi= FLT_MAX, ma= -FLT_MAX;
  int comp1=0, comp2=1;
  min = datamin(comp1);
  max = datamax(comp1);
  mi  = datamin(comp2);
  ma  = datamax(comp2);
  if (mi<min) min= mi;
  if (ma>max) max= ma;
}

void DoubleLineElement::plot(ptPainter& painter)
{
  if(enabled && Yaxis && visible) {
#ifdef DEBUG
    cout << "DoubleLineElement::plot(ptPainter& painter)" << endl;
#endif
    _prePlot();
    painter.setLine(color, lineWidth, style);

    // plot curve

    // find x-startcoordinate for line-label
    float labelx, labely1, labely2;
    if (labelOnLine)
      labelx=lineLabelPos*float((stopT-startT)/100.0);

    vector<float> Xval, Y1val, Y2val;
    int j,i;
    j = datastart();
    for (i=startT; i<=stopT; i++) {
      if (valid(i)) {
        // find correct position for line-label
        if (labelOnLine)
          if (i>=labelx) {
            labelx = xval(i);
            labely1 = yval(j,0)*1.04;
            labely2 = yval(j,1)*1.04;
          }

        Xval.push_back(xval(i));
        Y1val.push_back(yval(j,0));
        Y2val.push_back(yval(j,1));
        j++;
      }
    }

    // fill the area between the two lines
    painter.setFill(fillColor);
    QPolygonF between, line1, line2;
    for (i=0; i<Xval.size();i++) {
      QPointF p1(Xval[i], Y1val[i]);
      between << p1;
      line1 << p1;
    }
    for (int i=Xval.size()-1; i>=0; --i) {
      QPointF p2(Xval[i], Y2val[i]);
      between << p2;
      line2 << p2;
    }
    painter.drawPolygon(between);
    painter.drawPolyline(line1);
    painter.drawPolyline(line2);

    if (labelOnLine){
      painter.setFontSize(fontSize);
      float bx1, bx2, by1, by2;

      const QString qtext = QString::fromStdString(labelText);
      const QSizeF bbx = painter.getTextSize(qtext);

      float marg= bbx.height()*0.15;
      float labely= (labely1>labely2)?labely1:labely2;
      bx1= labelx - marg;
      bx2= bx1 + 2*marg + bbx.width();
      by1= labely - marg - bbx.height()*0.3;
      by2= by1 + bbx.height() + 2*marg;
      painter.setFill(ptColor("WHITE"));
      painter.drawRect(bx1, by1, bx2, by2);

      painter.setLine(color, 1);
      painter.drawText(QPointF(labelx,labely), qtext);
    }
#ifdef DEBUG
    cout << "DoubleLineElement::plot(ptPainter& painter) finished" << endl;
#endif
  }
}

} // namespace pets2

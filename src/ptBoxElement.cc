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

// ptBoxElement.cc : Definitions for BoxElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptBoxElement.h"

#include <QPolygonF>

using namespace miutil;
using namespace std;

namespace pets2 {

BoxElement::BoxElement(const DataSpec cds, const vector<miTime> tline,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
    : dataPlotElement(cds, layout, field, xtime)
    , label(layout.text)
    , linewidth(layout.lineWidth)
    , pattern(layout.fillstyle)
    , colorlist(layout.colorlist)
    , patternlist(layout.patternlist)
    , hsides(layout.horsides)
    , vsides(layout.versides)
    , style(layout.linePattern)
    , useTimes(layout.useTimes)
    , timeLine(tline)
{
#ifdef DEBUG
  cout << "Inside BoxElement's constructor" << endl;
#endif
  type = TIMEBOX;

  int nc = colorlist.size();
  int np = patternlist.size();
  // make sure colorlist and patternlist have equal sizes
  if (nc < np) {
    ptColor c;
    if (nc > 0)
      c = colorlist[nc - 1];
    else
      c = ptColor("BLACK");
    for (int i = nc; i <= np; i++)
      colorlist.push_back(c);

  } else if (nc > np) {
    ptFillStyle f;
    if (np > 0)
      f = patternlist[np - 1];
    else
      f = pets2::SOLID;
    for (int i = np; i <= nc; i++)
      patternlist.push_back(f);
  }
}

void BoxElement::plot(ptPainter& painter)
{
  if (enabled && visible) {
#ifdef DEBUG
    cout << "BoxElement::plot(ptPainter& painter)" <<endl;
#endif

    painter.setFontSize(fontSize);

    painter.setLineWidth(linewidth);
    painter.setFillStyle(pets2::NONE);
    painter.drawRect(xtime->xcoord[startT], startY, xtime->xcoord[stopT], stopY);

    painter.setLineStyle(pets2::NOLINE);

    int j = datastart();
    int ldat = -1, ndat = -1, lidx = 0, nidx = 0;
    for (int i = startT; i <= stopT; i++) {
      if (valid(i)) {
        nidx = i;
        ndat = static_cast<int> (dval(j));
        if (ndat != ldat || i == stopT) {
          if (ldat >= 0 && ldat < colorlist.size()) {
            painter.setFill(colorlist[ldat], patternlist[ldat]);
            painter.drawRect(xtime->xcoord[lidx], startY, xtime->xcoord[nidx], stopY);
          }
          ldat = ndat;
          lidx = nidx;
        }
        j++;
      }
      if (i == stopT && lidx != nidx) {
        if (ldat >= 0 && ldat < colorlist.size()) {
          painter.setFill(colorlist[ldat], patternlist[ldat]);
          painter.drawRect(xtime->xcoord[lidx], startY, xtime->xcoord[nidx], stopY);
        }
      }
    }
    painter.setColor(color);
    painter.setFillStyle(pets2::NONE);
    painter.drawRect(xtime->xcoord[startT], startY, xtime->xcoord[stopT], stopY);

    painter.drawText(label, 20, startY);

    if (useTimes > 0) {
      // make time-grid
      QPolygonF line;
      for (int i = startT + 1; i < stopT; i++) {
        if ((timeLine[i].hour() % useTimes) == 0 && timeLine[i].min() == 0) {
          line << QPointF(xtime->xcoord[i], startY);
        }
      }
      painter.setLine(color, linewidth, style);
      painter.drawPolyline(line);
    }
  }
}

} // namespace pets2

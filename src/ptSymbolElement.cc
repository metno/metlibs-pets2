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

// ptSymbolElement.cc : Definitions for SymbolElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptPlotElement.h>
#include <ptSymbolElement.h>

using namespace miutil;

namespace pets2 {

SymbolElement::SymbolElement(const DataSpec cds, const ptVertFieldf& field,
    const Layout& layout, XAxisInfo* xtime) :
  dataPlotElement(cds, layout, field, xtime), minidx(0), maxidx(0),
      maxwidth(1), maxheight(1), scalewidth(layout.scalewidth)
{
#ifdef DEBUG
  cout << "Inside SymbolElement's constructor" << endl;
#endif
  type = SYMBOL;
}

void SymbolElement::setImages(const int min, const std::vector<std::string>& files)
{
  minidx = min;
  maxidx = min + files.size() - 1;
  maxwidth = maxheight = 0;
  for (int i = 0; i < files.size() && i < MAXIMAGES; i++) {
    images[i].setimage(files[i]);
    if (images[i].Width() > maxwidth)
      maxwidth = images[i].Width();
    if (images[i].Height() > maxheight)
      maxheight = images[i].Height();
  }
}

void SymbolElement::plot(ptPainter& painter)
{
  if (enabled && visible) {
#ifdef DEBUG
    cout << "SymbolElement::plot(ptPainter& painter)" <<endl;
#endif

    // find minimum delta T for calculation of image width
    float minx = 10000;
    int k = startT;
    for (int i = startT + 1; i <= stopT; i++) {
      if (valid(i)){
        if (valid(k)) {
          float testx = xtime->xcoord[i] - xtime->xcoord[k];
          if (testx < minx)
            minx = testx;
        }
        k = i;
      }
    }
    float scalex = 1, scaley = 1;
    if (scalewidth && maxwidth * painter.pixWidth() > minx)
      scalex = minx / (painter.pixWidth() * maxwidth);
    if (maxheight * painter.pixHeight() > (stopY - startY))
      scaley = (stopY - startY) / (painter.pixHeight() * maxheight);
    const float scale = std::min(scalex, scaley);

    int j = datastart();

    for (int i = startT; i <= stopT; i++) {
      if (valid(i)) {
        if (dval(j) >= minidx && dval(j) <= maxidx) {
          int index = int(dval(j)) - minidx;
          if (index < 0 || index >= MAXIMAGES) {
            continue;
          }
          float halfw = images[index].Width() * painter.pixWidth() * scale / 2.0;
          images[index].plot(painter, xtime->xcoord[i] - halfw, startY, scale);
        }
        j++;
      }
    }
  }
}

} // namespace pets2

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

// ptTextElement.cc : Definitions for TextElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptTextElement.h"

#include "ptPatterns.h"

#include <puTools/miStringFunctions.h>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

TextElement::TextElement(const std::string& pText,
    const std::map<std::string, std::string>& keymap,
    const ptVertFieldf& field, const Layout& layout, XAxisInfo* xtime)
  : PlotElement(layout, field, xtime)
  , bcolor(layout.color2)
  , fillstyle(layout.fillstyle)
  , drawbackground(layout.drawbackground)
{
#ifdef DEBUG
  cout << "Inside TextElement's constructor" << endl;
  cout << "font: " << fontSize << " align: " << align <<
    " startY: " << startY << endl;
#endif
  type=TEXT;

  int n= xtime->xcoord.size();
  startX= stopX= 0;
  if (n>0){
    startX= xtime->xcoord[startT];
    stopX = xtime->xcoord[stopT];
  }

  text= pText;

  if (keymap.size()>0) {
    std::map<std::string, std::string>::const_iterator p;
    for (p=keymap.begin(); p!=keymap.end(); p++){
      miutil::replace(text, p->first, p->second);
    }
  }
}


void TextElement::plot(ptPainter& painter)
{
  if(enabled && text.length() && visible) {
#ifdef DEBUG
    cout << "TextElement::plot(ptPainter& painter): " << text << endl;
#endif
    std::vector<float> vth, vtw;
    float th, tw, allth=0;
    float x;
    painter.setFontSize(fontSize);

    std::vector<std::string> vs;
    if (miutil::contains(text, "\n")){
      vs= miutil::split(text, "\n");
      reverse(vs.begin(),vs.end());
    } else
      vs.push_back(text);

    int n= vs.size();
    for (int i=0; i<n; i++){
      const QSizeF bbx = painter.getTextSize(QString::fromStdString(vs[i]));
      vtw.push_back(bbx.width());
      vth.push_back(bbx.height());
      allth+= th;
    }

    if (drawbackground){
      startX= globalWindow.x1;
      stopX = globalWindow.x2;
      painter.setFill(bcolor, fillstyle);
      painter.setLineStyle(NOLINE);
      painter.drawRect(startX, startY, stopX, stopY);
    }

    float deltaY= (stopY - startY)/n;

    painter.setColor(color);
    for (int i=0; i<n; i++){
      switch(align) {
      case LEFT :
        x= 20;
        break;
      case RIGHT :
        x = globalWindow.x2-20-vtw[i];
        break;
      case CENTER :
        x = ((globalWindow.x2 - globalWindow.x1) - vtw[i])/2.0;
        break;
      }
      painter.drawText(vs[i], x, startY+deltaY*i+(deltaY-vth[i]*0.75)/2.0);
    }
  }
}

} // namespace pets2

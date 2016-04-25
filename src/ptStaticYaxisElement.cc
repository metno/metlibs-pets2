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

// ptStaticYaxisElement.cc : Definitions for staticYaxisElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptYaxisElement.h"

#include <cmath>
#include <cfloat>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

staticYaxisElement::staticYaxisElement(const ptVertFieldf& field,
    const Layout& layout, XAxisInfo* xtime) :
  yAxisElement(field, layout, xtime)
  , numTickMajor(layout.numTickMajor)
  , numTickMinor(layout.numTickMinor)
  , labelSpace(layout.labelSpace)
{
#ifdef DEBUG
  cout << "Inside staticYaxisElement's constructor" << endl;
#endif
  type = YAXIS_STATIC;
}

staticYaxisElement::~staticYaxisElement()
{
}

void staticYaxisElement::calcPlotVal()
{
  const float SMALLSPACE = 5;
  float maxwid = 0, lx;
  int i;
  float totalwidth = 0, lstartx = 0;

  float charHeight_2 = 10;
  if (canvas) {
    canvas->setFontSize(fontSize);
    charHeight_2 = canvas->getTextHeight()/2.0;

    for (i = 0; i < numChild + 1; i++) {
      const QSizeF bbx = canvas->getTextSize(QString::fromStdString(Labels[i]));
      labelW[i] = bbx.width();
      labelH[i] = bbx.height();
      if (labelW[i] > maxwid)
        maxwid = labelW[i];
      // calculate total width for horizontal legends
      if (i > 0)
        totalwidth += labelW[i] + linlen + charWidth;
    }
  } else {
    const float V = 10;
    charHeight_2 = V/2;
    for (i=0; i<numChild+1; i++)
      labelW[i] = labelH[i] = V;
    totalwidth = V + numChild*(V + linlen + charWidth);
    maxwid = 10;
  }
  if (!horLabels) {// vertical printed legends
    labelY[numChild] = axeStopY - labelH[numChild];
    for (i = numChild - 1; i >= 0; i--) {
      labelY[i] = labelY[i + 1] - labelH[i] * 1.0;
    }
  } else {// horizontally printed legends
    labelY[numChild] = axeStopY - (1 + horLabelOffset) * labelH[numChild];
    lstartx = xtime->x1 + (xtime->x2 - xtime->x1) / 2 - totalwidth / 2;
    for (i = numChild - 1; i > 0; i--)
      labelY[i] = labelY[i + 1];
    labelY[0] = axeStopY - labelSpace + charHeight_2;
  }
  deltaY = axeStopY - startY - labelSpace;

  switch (axis) {
  case pets2::LEFTLEFT:
    lsign = sign = -1;
    if (legendlineinside)
      lsign = 1;
    zerox = xtime->x1;
    tickX = zerox - tickLen;
    lx = (zerox > maxwid + SMALLSPACE) ? (zerox - maxwid - SMALLSPACE)
        : SMALLSPACE;
    if (fittopage && zerox < (maxwid + linlen))
      lx = linlen;
    if (!horLabels) {
      for (i = 0; i < numChild + 1; i++) {
        labelX[i] = lx;
        lineSt[i] = (legendlineinside ? zerox : lx);
      }
    } else {
      labelX[0] = lx;
      for (i = numChild; i > 0; i--) {
        labelX[i] = lstartx;
        lstartx += labelW[i];
        lstartx += linlen;
        lineSt[i] = lstartx;
        lstartx += charWidth;
      }
    }
    break;
  case pets2::RIGHTRIGHT:
    lsign = sign = 1;
    if (legendlineinside)
      lsign = -1;
    zerox = xtime->x2;
    tickX = zerox + tickLen;
    lx = zerox + SMALLSPACE;
    if (fittopage && (zerox + maxwid + linlen) > globalWindow.x2)
      lx = globalWindow.x2 - (maxwid + linlen + SMALLSPACE);
    if (!horLabels) {
      for (i = 0; i < numChild + 1; i++) {
        labelX[i] = lx;
        lineSt[i] = (legendlineinside ? zerox : lx + labelW[i]);
      }
    } else {
      labelX[0] = lx;
      for (i = numChild; i > 0; i--) {
        labelX[i] = lstartx;
        lstartx += labelW[i];
        lineSt[i] = lstartx;
        lstartx += linlen + charWidth;
      }
    }
    break;
  case pets2::LEFTRIGHT:
    lsign = sign = 1;
    zerox = xtime->x1;
    tickX = zerox + tickLen;
    lx = zerox + SMALLSPACE;
    if (!horLabels) {
      for (i = 0; i < numChild + 1; i++) {
        labelX[i] = lx;
        lineSt[i] = lx + labelW[i] + SMALLSPACE;
      }
    } else {
      labelX[0] = lx;
      for (i = numChild; i > 0; i--) {
        labelX[i] = lstartx;
        lstartx += labelW[i];
        lineSt[i] = lstartx;
        lstartx += linlen + charWidth;
      }
    }
    break;
  case pets2::RIGHTLEFT:
    lsign = sign = -1;
    zerox = xtime->x2;
    tickX = zerox - tickLen;
    lx = zerox - maxwid - SMALLSPACE;
    if (!horLabels) {
      for (i = 0; i < numChild + 1; i++) {
        labelX[i] = lx;
        lineSt[i] = lx - SMALLSPACE;
      }
    } else {
      labelX[0] = lx;
      for (i = numChild; i > 0; i--) {
        labelX[i] = lstartx;
        lstartx += labelW[i];
        lstartx += linlen;
        lineSt[i] = lstartx;
        lstartx += charWidth;
      }
    }
    break;
  }
}

void staticYaxisElement::calcDims()
{
  // new : get min,max,delta each time
  float Max=-100000, Min=100000;
  for (int j=0; j<numChild; j++) {
    if (child[j]->isEnabled()) {
      float cMax, cMin;
      child[j]->dataInfo(cMin, cMax);
      if (cMax > Max)
        Max = cMax;
      if (cMin < Min)
        Min = cMin;
    }
  }
  if (useMinMax) {
    if (Min < minValue)
      Min = minValue;
    if (Max > maxValue)
      Max = maxValue;
  }
  float Delta = Max - Min, newDelta;

  if (!minIsSet && !maxIsSet) {
    if (delta > 0.0001) {
      float modres, dy, sy;
      int ntick = numTickMajor;
      modres = fmodf(Min, delta);
      Min -= (modres < 0 ? delta + modres : modres);
      modres = fmodf(Max, delta);
      Max += (modres > 0 ? delta - modres : -modres);
      dy = (Max - Min);
      sy = dy / float(ntick);
      if (sy < delta)
        sy = delta;
      else {
        modres = fmodf(sy, delta);
        sy += delta - modres;
      }
      Max = Min + ntick * sy;
      minPlotY = Min;
      maxPlotY = Max;
      plotRange = Max - Min;
    } else {
      plotRange = (Delta < minRange)
          ? minRange + 2 * minMargin
          : Delta + 2 * minMargin;

      minPlotY = Min + (Delta - plotRange) / 2;
      if (minValue < minPlotY) {
        plotRange += (minPlotY - minValue);
        minPlotY = minValue;
      }
      maxPlotY = minPlotY + plotRange;
      if (maxValue > maxPlotY) {
        plotRange += (maxValue - maxPlotY);
        maxPlotY = maxValue;
      }
    }

  } else if (minIsSet && maxIsSet) {
    minPlotY = minValue;
    maxPlotY = maxValue;
    plotRange = maxPlotY - minPlotY;

  } else if (minIsSet) {
    if (delta > 0.0001) {
      float modres, dy, sy;
      int ntick = numTickMajor;
      Min = minValue;
      modres = fmodf(Max, delta);
      Max += (modres > 0 ? delta - modres : -modres);
      dy = (Max - Min);
      sy = dy / float(ntick);
      if (sy < delta)
        sy = delta;
      else {
        modres = fmodf(sy, delta);
        sy += delta - modres;
      }
      Max = Min + ntick * sy;
      minPlotY = Min;
      maxPlotY = Max;
      plotRange = Max - Min;
    } else {
      minPlotY = minValue;
      newDelta = Max - minPlotY;
      plotRange = (newDelta < minRange)
          ? minRange + minMargin
          : newDelta + minMargin;
      maxPlotY = minPlotY + plotRange;
      if (maxValue > maxPlotY) {
        plotRange += (maxValue - maxPlotY);
        maxPlotY = maxValue;
      }
    }

  } else { // maxIsSet
    maxPlotY = maxValue;
    newDelta = maxPlotY - Min;
    plotRange = (newDelta < minRange)
        ? minRange + minMargin
        : newDelta + minMargin;
    minPlotY = maxPlotY - plotRange;
    if (minValue < minPlotY) {
      plotRange += (minPlotY - minValue);
      minPlotY = minValue;
    }
  }

  minorticks.erase(minorticks.begin(), minorticks.end());
  majorticks.erase(majorticks.begin(), majorticks.end());
  majorphys.erase(majorphys.begin(), majorphys.end());

  float matick = float(deltaY) / numTickMajor;
  float mitick = matick / numTickMinor;

  float fy = startY - mitick;
  for (int i = 0; i <= numTickMajor; i++) {
    fy += mitick;
    majorticks.push_back(int(fy));
    float value = minPlotY + (plotRange / float(deltaY)) * (fy - startY);
    majorphys.push_back(value);
    if (i < numTickMajor)
      for (int j = 0; j < numTickMinor - 1; j++) {
        fy += mitick;
        minorticks.push_back(int(fy));
      }
  }
  recalcDims = false;
}

void staticYaxisElement::setTimeInterval(const int start, const int stop)
{
  yAxisElement::setTimeInterval(start,stop);

  //cerr << "staticYaxisElement::setTimeInterval for axis:" << id << endl;
}

} // namespace pets2

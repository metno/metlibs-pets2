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


// ptYaxisElement.cc : Definitions for yAxisElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptYaxisElement.h"

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

namespace pets2 {

yAxisElement::yAxisElement(const ptVertFieldf& field,
    const Layout& layout, XAxisInfo* xtime)
  : PlotElement(layout, field, xtime)
  , numChild(0)
  , id(layout.yaid)
  , charHeight(10)
  , charWidth(10)
  , firstPlot(true)
  , axeStopY(field.y2)
  , tickLen(layout.tickLen)
  , lineWidth(layout.lineWidth)
  , axisLineWidth(layout.axisWidth)
  , tickWidth(layout.tickWidth)
  , interval(layout.interval)
  , minRange(layout.minRange)
  , delta(layout.delta)
  , minMargin(layout.minMargin)
  , style(layout.linePattern)
  , axis(layout.axis)
  , minIsSet(layout.minIsSet)
  , maxIsSet(layout.maxIsSet)
  , minValue(layout.minValue)
  , maxValue(layout.maxValue)
  , recalcDims(true)
  , userlabels(false)
  , userLabels(layout.textlabels)
  , userValueLabels(layout.valuetextlabels)
  , fittopage(layout.fittopage)
  , linlen(48)
  , plotlegends(layout.label)
  , horLabels(layout.horLabels)
  , horLabelOffset(layout.horLabelOffset)
  , legendlineinside(layout.legendlineinside)
  , axisgrid(layout.axisgrid)
  , gridwidth(layout.gridwidth)
  , gridcolor(layout.gridcolor)
  , gridstyle(layout.gridstyle)
  , useMinMax(layout.useMinMax)
{
#ifdef DEBUG
  cout << "Inside yAxisElement's constructor" << endl;
#endif
  type = YAXIS;

  for (int i=0; i<MAX_YAXIS_CHILDREN; i++) child[i]=0;

  //axisText = (layout.text.length()) ? layout.text : data->axisText;
  Labels[0] = layout.text;

  userlabels= (userLabels.size() > 0);
  if ( userlabels && userValueLabels.size() == 0 )
    userValueLabels = userLabels;
}

void yAxisElement::plotAxis(ptPainter& painter)
{
  if (!numChild || !visible ) return;
#ifdef DEBUG
    cout << "yAxisElement::plotAxis()" << endl;
#endif
  float txtX, tmpX, tmpY;
  const float SMALLSPACE = 5;
  char txt[20];

  painter.setFontSize(fontSize);

  txtX= tickX + sign*SMALLSPACE;
  tmpX= txtX;

  // plot y-axis
  painter.setLine(color, axisLineWidth);
  painter.drawLine(zerox, startY, zerox, startY+deltaY);

  // plot tickmarks for each interval step, and write value for each delta
  painter.setLine(color, tickWidth);
  size_t i,n= minorticks.size();
  for (i=0; i<n; i++){
    painter.drawLine(tickX, minorticks[i], zerox, minorticks[i]);  // minor tick mark
  }
  n= majorticks.size();
  for (i=0; i<n; i++){
    painter.drawLine(tickX, majorticks[i], zerox, majorticks[i]);  // major tick mark
    if (!userlabels){
      // print value
      float val = majorphys[i];
      if ( delta >= 5 && fmodf(delta,1.0)==0)
        snprintf(txt,sizeof(txt),"%1.0f",val);
      else if (delta >= 0.1 ){
        snprintf(txt,sizeof(txt),"%1.1f",val);
      } else {
        snprintf(txt,sizeof(txt),"%1.2f",val);
      }
      QString qtext(txt);
      const QSizeF bbx = painter.getTextSize(qtext);
      const float swid = bbx.width(), shei = bbx.height();

      if (align==pets2::OVER)
        tmpY=majorticks[i];
      else if (align==pets2::BELOW)
        tmpY=majorticks[i]-shei;
      else
        tmpY= majorticks[i]-shei/2.0;

      if (sign==-1)
        tmpX= txtX-swid;
      painter.drawText(QPointF(tmpX,tmpY), qtext);
    } else{
      if (i<userLabels.size()){
        QString qtext = QString::fromStdString(userLabels[i]);
        const QSizeF bbx = painter.getTextSize(qtext);
        const float swid = bbx.width(), shei = bbx.height();

        if (align==pets2::OVER)
          tmpY=majorticks[i];
        else if (align==pets2::BELOW)
          tmpY=majorticks[i]-shei;
        else
          tmpY= majorticks[i]-shei/2.0;

        if (sign==-1)
          tmpX= txtX-swid;
        painter.drawText(QPointF(tmpX, tmpY), qtext);
      }
    }
  }
  plotLabels(painter);
}


void yAxisElement::plotLabels(ptPainter& painter)
{
  // write axis label
  painter.setLine(color);
  painter.drawText(QPointF(labelX[0],labelY[0]), QString::fromStdString(Labels[0]));

  if (!plotlegends)
    return;
  for (int j=1; j<numChild+1; j++) {
    if (child[j-1]->isEnabled()) {
      painter.setLine(childCol[j-1], lineWidth, childStyle[j-1]);
      painter.drawText(Labels[j], labelX[j], labelY[j]);
      painter.drawLine(lineSt[j], labelY[j], lineSt[j]+lsign*linlen, labelY[j]);
    }
  }
}

void yAxisElement::plotGrid(ptPainter& painter)
{
  if (!axisgrid)
    return;
  int n= majorticks.size();
  painter.setLine(gridcolor, gridwidth, gridstyle);
  for (int i=0; i<n; i++){
    float fy = majorticks[i];
    painter.drawLine(xtime->x1, fy, xtime->x2, fy);
  }
}

void yAxisElement::plot(ptPainter& painter)
{
  if(enabled && numChild) {
#ifdef DEBUG
    cout << "yAxisElement::plot(ptPainter& painter)" << endl;
#endif
    calcPlotVal();
    firstPlot = false;
    if (recalcDims) {
      calcDims();
    }
    plotGrid(painter);
    plotAxis(painter);
    recalcDims= true;
  }
}

void yAxisElement::dataInfo(float &dY, float & minPY, float &pR)
{
  if (firstPlot)
    calcPlotVal();
  firstPlot = false;
  if (recalcDims) {
    calcDims();
  }

  dY = deltaY;
  minPY = minPlotY;
  pR = plotRange;
}


void yAxisElement::calcPlotVal()
{
  const float SMALLSPACE = 5;
  float maxwid = 0, lx;
  int i;

  if (canvas) {
    std::cout << "++++++++++++++++++++> calcPlotVal with canvas" << std::endl;
    canvas->setFontSize(fontSize);
    const QSizeF bbx0 = canvas->getTextSize("M");
    charWidth = bbx0.width();
    charHeight = bbx0.height();
    for (i=0; i<numChild+1; i++) {
      const QSizeF bbx = canvas->getTextSize(QString::fromStdString(Labels[i]));
      labelW[i] = bbx.width();
      labelH[i] = bbx.height();
      if (labelW[i] > maxwid)
        maxwid = labelW[i];
    }
  } else {
    std::cout << "--------------------> calcPlotVal without canvas" << std::endl;
    charWidth = 10;
    charHeight = 10;
    for (i=0; i<numChild+1; i++)
      labelW[i] = labelH[i] = 10;
    maxwid = 10;
  }
  labelY[numChild] = axeStopY - labelH[numChild];
  for (i=numChild-1; i>=0; i--) {
    labelY[i] = labelY[i+1] - labelH[i]*1.5;
  }
  deltaY = labelY[0]-charHeight-startY;

  switch(axis) {
  case pets2::LEFTLEFT:
    sign=-1;
    zerox = xtime->x1;
    tickX = zerox - tickLen;
    lx = (zerox > maxwid+SMALLSPACE) ?
        (zerox-maxwid-SMALLSPACE) : SMALLSPACE;
    if (fittopage && zerox < (maxwid+linlen))
      lx = linlen;
    for (i=0; i<numChild+1; i++){
      labelX[i] = lx;
      lineSt[i] = lx;// - SMALLSPACE;
    }
    break;
  case pets2::RIGHTRIGHT:
    sign=1;
    zerox = xtime->x2;
    tickX = zerox + tickLen;
    lx = zerox + SMALLSPACE;
    if (fittopage && (zerox+maxwid+linlen)>globalWindow.x2)
      lx = globalWindow.x2 - (maxwid+linlen);
    for (i=0; i<numChild+1; i++){
      labelX[i] = lx;
      lineSt[i] = lx + labelW[i];// + SMALLSPACE;
    }
    break;
  case pets2::LEFTRIGHT:
    sign=1;
    zerox = xtime->x1;
    tickX = zerox + tickLen;
    lx = zerox+SMALLSPACE;
    for (i=0; i<numChild+1; i++){
      labelX[i] = lx;
      lineSt[i] = lx + labelW[i] + SMALLSPACE;
    }
    break;
  case pets2::RIGHTLEFT:
    sign=-1;
    zerox = xtime->x2;
    tickX = zerox - tickLen;
    lx = zerox-maxwid-SMALLSPACE;
    for (i=0; i<numChild+1; i++){
      labelX[i] = lx;
      lineSt[i] = lx - SMALLSPACE;
    }
    break;
  }

}

bool yAxisElement::callinn(AxisChildElement* pc, float& minlegal, float& maxlegal)
{
  if (numChild < MAX_YAXIS_CHILDREN) {
    child[numChild] = pc;
    numChild++;
    // save child label and color
    Labels[numChild] = pc->labelName();
    childCol[numChild-1] = pc->Color();
    childStyle[numChild-1] = pc->lStyle();
    minlegal = (minIsSet) ? minValue : -FLT_MAX;
    maxlegal = (maxIsSet) ? maxValue :  FLT_MAX;
    return true;
  }
  return false;
}

void yAxisElement::setTimeInterval(const int start, const int stop)
{
  PlotElement::setTimeInterval(start,stop);
}

void yAxisElement::calcDims()
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
    plotRange = (Delta < minRange)
        ? minRange + 2*minMargin
        : Delta + 2*minMargin;
    minPlotY = Min + (Delta - plotRange)/2;
    if (minValue < minPlotY) {
      plotRange += (minPlotY - minValue);
      minPlotY = minValue;
    }
    maxPlotY = minPlotY + plotRange;
    if (maxValue>maxPlotY) {
      plotRange += (maxValue-maxPlotY);
      maxPlotY = maxValue;
    }
  } else if (minIsSet && maxIsSet) {
    minPlotY = minValue;
    maxPlotY = maxValue;
    plotRange = maxPlotY - minPlotY;
  } else if (minIsSet) {
    minPlotY = minValue;
    newDelta = Max - minPlotY;
    plotRange = (newDelta < minRange)
        ? minRange + minMargin
        : newDelta + minMargin;
    maxPlotY = minPlotY + plotRange;
    if (maxValue>maxPlotY) {
      plotRange += (maxValue-maxPlotY);
      maxPlotY = maxValue;
    }
  } else { // maxIsSet
    maxPlotY = maxValue;
    newDelta = maxPlotY - Min;
    plotRange = (newDelta < minRange)
        ? minRange + minMargin
        : newDelta + minMargin;
    minPlotY = maxPlotY - plotRange;
    if (minValue < minPlotY) {
      plotRange += (minPlotY-minValue);
      minPlotY = minValue;
    }
  }

  minorticks.erase(minorticks.begin(),minorticks.end());
  majorticks.erase(majorticks.begin(),majorticks.end());
  majorphys.erase(majorphys.begin(),majorphys.end());
  const float eps2 = 0.01;

  float interv= interval;
  float delt= delta;
  const int maxticks= 30;
  while ((maxPlotY-minPlotY)/interv>maxticks) {
    interv *= 2;
    delt   *= 2;
  }
  float starti = (minPlotY < 0)
      ? minPlotY-interv-fmod(minPlotY,interv)
      : minPlotY+interv-fmod(minPlotY,interv);
  if (starti < 0)
    starti+=interv;

  for(float i = starti; i <= maxPlotY; i+=interv) {
    float y = startY + (deltaY*(i-minPlotY))/plotRange;
    if(fabs(fmod(i,delt)) < eps2 || delt-fabs(fmod(i,delt)) < eps2 ) {
      majorticks.push_back(y);
      majorphys.push_back(i);
    } else {
      minorticks.push_back(y);
    }
  }

  recalcDims = false;
}

std::string yAxisElement::userValueLabel(const float value)
{
  int n = majorticks.size();

  for (int i=0; i<n; i++) {
    if (value <= majorphys[i] && i < userValueLabels.size()) {
      return userValueLabels[i];
    }
  }

  return std::string();
}

} // namespace pets2

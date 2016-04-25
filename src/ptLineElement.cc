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


// ptLineElement.cc : Definitions for LineElement class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptLineElement.h"

#include <QPolygonF>

#include <cmath>
#include <cfloat>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
#endif // DEBUG

namespace pets2 {

LineElement::LineElement(yAxisElement* ya,
    const DataSpec cds,
    const ptVertFieldf& field,
    const Layout& layout, XAxisInfo* xtime, bool ismulti)
  : AxisChildElement(ya,cds,field,layout,xtime)
  , marker(layout.marker)
  , markercolor(layout.color2)
  , arrowcolor(layout.color2)
  , markerfill(layout.markerFill)
  , markersize(layout.size)
  , barsize(layout.delta)
  , shadow(layout.shadow)
  , lineBar(layout.lineBar)
  , lineStep(layout.lineStep)
  , lineArrows(layout.lineArrows)
  , arrowLength(layout.arrowLength)
  , arrowSize(layout.arrowSize)
  , arrowfill(layout.fillstyle)
  , arrow(layout.vectorArrow)
  , reverse(layout.reverse)
  , colorbyvalue(layout.colorbyvalue)
  , colorlist(layout.colorlist)
  , smoothing(layout.smoothing)
  , smoothdiv(layout.smoothdiv)
  , wrapdegrees(layout.wrapdegrees)
  , wraplimit(layout.wraplimit)
  , isMultiLine(ismulti)

{
#ifdef DEBUG
  cout << "Inside LineElement's constructor" << endl;
#endif
  type=LINE;
}


void LineElement::dataInfo(float &min, float &max)
{
  if(!isMultiLine) {
    AxisChildElement::dataInfo(min,max);
    return;
  }

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






void LineElement::plot(ptPainter& painter)
{
  if(enabled && Yaxis && visible) {
#ifdef DEBUG
    cout << "LineElement::plot(ptPainter& painter)" << endl;
#endif
    // vertex points (legal datapoints)
    std::vector<float> vertexx, vertexy;
    // vertex point data index (legal datapoints)
    std::vector<int>   vertexi;

    // marker types
    float mSizeX = markersize * painter.pixWidth();
    float mSizeY = markersize * painter.pixHeight();
    bool noc = (lineWidth < 0);

    _prePlot();
    painter.setLine(color, lineWidth, style);

    // plot curve
    float prevx=-2000,prevy,newx,newy;
    int previ1=-2000;

    // find x-startcoordinate for line-label
    float labelx, labely;
    if (labelOnLine)
      labelx = lineLabelPos*1.0*(stopT-startT)/100.0;

    float cx, cy; // crossing point
    std::vector<float> plotx, ploty; // plot points
    std::vector<int> ploti; // plot color index (for colorbyvalue)

    int j,i;



    int numOfLines=1;
    if(isMultiLine)
      numOfLines=datadimension();
    int numOfPoints = (stopT-startT);

    for(int n=0;n<numOfLines;n++) {
      previ1=-2000;
      prevx=-2000;
      j = datastart();

      for (i=startT; i<=stopT; i++) {
        if (valid(i)) {
          newx= xval(i);
          if(isMultiLine) {
            newy= yval(j,n);
          } else {
            newy= yval(j,ds.comp);
          }

          bool pointbad=false;

          if (newy > stopY || newy < startY){ // illegal point
            pointbad = true;
          }

          // find correct position for line-label
          if ( labelOnLine )
            if (i>=labelx) {
              labelx = newx;
              labely = newy;
              if ( keepinaxis ){
                if (newy > stopY) labely= stopY-10;
                else if (newy < startY) labely= startY;
              }
            }

          // types of line drawing
          if (lineBar){  // draw horisontal bars
            if (!noc && !(keepinaxis && pointbad)){
              int i1 = dataLimitIndex(dval(j));
              plotx.push_back(newx-barsize);
              ploty.push_back(newy);
              ploti.push_back(i1);
              plotx.push_back(newx+barsize);
              ploty.push_back(newy);
              ploti.push_back(i1);
            }

          } else if (lineStep) { // draw stairs
            // find points
            if (prevx < 0){
              prevx= xtime->x1;
              prevy= newy;
            }
            float d2;
            if (align == LEFT)
              d2= prevx;
            else if (align == RIGHT)
              d2= newx;
            else
              d2= (prevx+newx)/2.0;
            if (!noc) {
              if (colorbyvalue) {
                int i1 = dataLimitIndex(dval(j));
                if ( previ1 > -1000 ){
                  if ( i1 == previ1 ){
                    plotx.push_back(prevx);
                    ploty.push_back(prevy);
                    ploti.push_back(i1);
                    plotx.push_back(d2);
                    ploty.push_back(prevy);
                    ploti.push_back(i1);

                    plotx.push_back(d2);
                    ploty.push_back(prevy);
                    ploti.push_back(i1);
                    plotx.push_back(d2);
                    ploty.push_back(newy);
                    ploti.push_back(i1);

                    plotx.push_back(d2);
                    ploty.push_back(newy);
                    ploti.push_back(i1);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(i1);

                  } else {
                    plotx.push_back(prevx);
                    ploty.push_back(prevy);
                    ploti.push_back(previ1);
                    plotx.push_back(d2);
                    ploty.push_back(prevy);
                    ploti.push_back(previ1);
                    // find crossing points to data limits
                    float cx,cy;
                    int neg = ( previ1 > i1 ? -1 : 1);
                    int ncr = abs(previ1-i1);
                    float xc1=d2, xc2=d2;
                    float yc1=prevy, yc2=newy;
                    for ( int k = 1; k<=ncr; k++ ){
                      int ind1= previ1+neg*(k-1);
                      int ind2= previ1+neg*k;
                      float d = dataLimit( std::max(ind1,ind2) );

                      // find crossing
                      if (dataCrossPoint(d2,yc1,d2,yc2,d,cx,cy)){
                        // make line

                        plotx.push_back(d2);
                        ploty.push_back(yc1);
                        ploti.push_back(ind1);
                        plotx.push_back(d2);
                        ploty.push_back(cy);
                        ploti.push_back(ind2);

                        xc1= d2; xc2= d2;
                        yc1= cy; yc2= newy;
                      }
                    }
                    plotx.push_back(cx);
                    ploty.push_back(cy);
                    ploti.push_back(i1);
                    plotx.push_back(d2);
                    ploty.push_back(newy);
                    ploti.push_back(i1);

                    plotx.push_back(d2);
                    ploty.push_back(newy);
                    ploti.push_back(i1);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(i1);
                  }
                }
                previ1= i1;
              } else { // no colorbyvalue
                if ( prevx > -1000 ){
                  plotx.push_back(prevx);
                  ploty.push_back(prevy);
                  ploti.push_back(0);
                  plotx.push_back(d2);
                  ploty.push_back(prevy);
                  ploti.push_back(0);

                  plotx.push_back(d2);
                  ploty.push_back(prevy);
                  ploti.push_back(0);
                  plotx.push_back(d2);
                  ploty.push_back(newy);
                  ploti.push_back(0);

                  plotx.push_back(d2);
                  ploty.push_back(newy);
                  ploti.push_back(0);
                  plotx.push_back(newx);
                  ploty.push_back(newy);
                  ploti.push_back(0);
                }
              }
            }

          } else { // normal line
            if ( !noc ){
              if ( colorbyvalue ) {
                int i1 = dataLimitIndex(dval(j));
                if ( previ1 > -1000 ){
                  if ( i1 == previ1 ){
                    plotx.push_back(prevx);
                    ploty.push_back(prevy);
                    ploti.push_back(i1);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(i1);

                  } else {
                    // find crossing points to data limits
                    float cx,cy;
                    int neg = ( previ1 > i1 ? -1 : 1);
                    int ncr = abs(previ1-i1);
                    float xc1=prevx, xc2=newx;
                    float yc1=prevy, yc2=newy;
                    for ( int k = 1; k<=ncr; k++ ){
                      int ind1= previ1+neg*(k-1);
                      int ind2= previ1+neg*k;
                      float d = dataLimit( std::max(ind1,ind2) );
                      // find crossing
                      if (dataCrossPoint(xc1,yc1,xc2,yc2,d,cx,cy)){
                        // make line
                        plotx.push_back(xc1);
                        ploty.push_back(yc1);
                        ploti.push_back(ind1);
                        plotx.push_back(cx);
                        ploty.push_back(cy);
                        ploti.push_back(ind2);

                        xc1= cx; xc2= newx;
                        yc1= cy; yc2= newy;
                      }
                    }
                    plotx.push_back(cx);
                    ploty.push_back(cy);
                    ploti.push_back(i1);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(i1);
                  }
                }
                previ1= i1;
              } else {
                if ( prevx > -1000 ){
                  if ( !wrapdegrees || fabsf(dval(j) - dval(j-1)) < wraplimit ){
                    plotx.push_back(prevx);
                    ploty.push_back(prevy);
                    ploti.push_back(0);
                    plotx.push_back(newx);
                    ploty.push_back(newy);
                    ploti.push_back(0);
                  }
                }
              }
            }
          }

          // keep vertex point
          if ( !(keepinaxis && pointbad) ){
            vertexx.push_back(newx);
            vertexy.push_back(newy);
            vertexi.push_back(j);
          }
          // previous point
          prevx= newx;
          prevy= newy;

          // increase data pointer
          j++;
        } // valid time
      } // time loop
    }// dimension loop

    int npos= plotx.size();

    // spline smoothing of line
    if ( smoothing && npos > 2 ){
      float * x = new float[npos];
      float * y = new float[npos];
      int   * z = new int[npos];
      float * x_s = new float[(npos-1)*smoothdiv+npos];
      float * y_s = new float[(npos-1)*smoothdiv+npos];

      j=0;
      for ( int i=0; i<npos; i++ ){
        if ( j==0 || (plotx[i]!=x[j-1] || ploty[i]!=y[j-1]) ){
          x[j] = plotx[i];
          y[j] = ploty[i];
          z[j] = ploti[i];
          j++;
        }
      }
      npos = j;

      int s_length = smoothline(npos, &x[0], &y[0] , 0, npos-1, smoothdiv, &x_s[0], &y_s[0]);
      if ( s_length > 0 ) { // spline worked..
        plotx.clear(); ploty.clear(); ploti.clear();
        int co=0, tyc=0;
        for ( int i=0; i<s_length; i++, co++ ){
          int index = z[tyc];
          if ( co == smoothdiv ){
            tyc++;
            co=-1;
          }
          plotx.push_back(x_s[i]);
          ploty.push_back(y_s[i]);
          ploti.push_back(index);
          if ( i!= 0 && i!= s_length-1 ){
            plotx.push_back(x_s[i]);
            ploty.push_back(y_s[i]);
            ploti.push_back(index);
          }
        }
      }
      npos= plotx.size();
      delete[] x;
      delete[] y;
      delete[] z;
      delete[] x_s;
      delete[] y_s;
    }

    int currentPoint=0;

    // plot line segments
    for (int i=0; i<npos; i+=2) {
      // for multilines - stop drawing at the end of everly line restart at start
      // end multilines

      if (colorbyvalue)
        painter.setColor(colorfromindex(ploti[i]));

      if (keepinaxis) {
        if ((ploty[i] > stopY && ploty[i+1] > stopY) ||
            (ploty[i] < startY && ploty[i+1] < startY)) { // both outside
          continue;
        } else if (ploty[i] > stopY
            || ploty[i+1] > stopY
            || ploty[i] < startY
            || ploty[i+1] < startY)
        { // one outside
          // find crossing
          if (crossPoint(plotx[i],ploty[i],plotx[i+1],ploty[i+1],cx,cy)) {
            if (ploty[i] < startY ||  ploty[i] > stopY) { // first bad
              painter.drawLine(cx, cy, plotx[i+1], ploty[i+1]);
            } else if (ploty[i+1] < startY || ploty[i+1] > stopY) { // second bad
              painter.drawLine(plotx[i], ploty[i], cx, cy);
            }
          }
        } else { // all inside
          painter.drawLine(plotx[i], ploty[i], plotx[i+1], ploty[i+1]);
        }
      } else { // no keepinaxis checking
        painter.drawLine(plotx[i], ploty[i], plotx[i+1], ploty[i+1]);
      }
    }

    // draw a marker at each point
    if (marker != NO_MARKER) {
      int n=vertexx.size();
      painter.setColor(markercolor);
      painter.setFillStyle(markerfill);

      switch (marker) {
      case NO_MARKER :
        break;
      case M_RECTANGLE :
        for (int i=0; i<n; i++) {
          painter.drawRect(vertexx[i]-mSizeX, vertexy[i]-mSizeY, vertexx[i]+mSizeX, vertexy[i]+mSizeY);
        }
        break;
      case M_TRIANGLE : {
        mSizeX*= 1.2;
        mSizeY*= 1.2*0.866;
        QPolygonF triangle;
        triangle << QPointF(-mSizeX, +mSizeY)
                 << QPointF(+mSizeX, +mSizeY)
                 << QPointF(0, -mSizeY);
        for (int i=0; i<n; i++) {
          painter.save();
          painter.translate(vertexx[i], vertexy[i]);
          painter.drawPolygon(triangle);
          painter.restore();
        }
        break; }
      case M_DIAMOND : {
        mSizeX*= 1.2;
        mSizeY*= 1.2;
        QPolygonF diamond;
        diamond << QPointF(-mSizeX, 0)
                << QPointF(0, +mSizeY)
                << QPointF(+mSizeX, 0)
                << QPointF(0, -mSizeY);
        for (int i=0; i<n; i++) {
          painter.save();
          painter.translate(vertexx[i], vertexy[i]);
          painter.drawPolygon(diamond);
          painter.restore();
        }
        break; }
      case M_STAR : {
        for (int i=0; i<n; i++) {
          painter.save();
          painter.translate(vertexx[i], vertexy[i]);
          painter.drawLine(-mSizeX, 0, +mSizeX, 0);
          painter.drawLine(0, -mSizeY, 0, +mSizeY);
          painter.drawLine(-mSizeX*0.75, -mSizeY*0.75, +mSizeX*0.75, +mSizeY*0.75);
          painter.drawLine(-mSizeX*0.75, +mSizeY*0.75, +mSizeX*0.75, -mSizeY*0.75);
          painter.restore();
        }
        break; }
      case M_CIRCLE :
        for (int i=0; i<n; i++) {
          painter.drawCircle(vertexx[i], vertexy[i], mSizeX);
        }
        break;
      default:
        break;
      }
    }

    // draw an arrow at each point
    if (datadimension() > 1 && lineArrows) {
      float pixf = painter.pixWidth() / painter.pixHeight();
      // arrow types
      float YLEN=arrowLength;
      float XLEN=YLEN;
      float AFAC= -arrowSize;
      float SFAC= AFAC*0.5;

      int n=vertexi.size();
      bool closed=false;
      painter.setColor(arrowcolor);
      painter.setFillStyle(arrowfill);

      for (int i=0; i<n; i++) {
        float alpha;
        if (!reverse)
          alpha = M_PI_2 - M_PI*dval(vertexi[i],1)/180;
        else // angle rotated 180 degrees
          alpha = 3*M_PI_2 - M_PI*dval(vertexi[i],1)/180;

        float uff = XLEN*cos(alpha);
        float vff = YLEN*sin(alpha);
        float x0 = vertexx[i];
        float y0 = vertexy[i];

        if (colorbyvalue)
          painter.setColor(colorfromvalue(dval(vertexi[i])));

        // draw main arrow
        painter.drawLine(x0, y0, x0+uff*pixf, y0+vff);

        // draw arrowhead
        if (arrow) {
          if (closed) {
            QPolygonF head;
            head << QPointF(x0+uff*pixf,y0+vff)
                 << QPointF(x0+(uff+uff*AFAC+vff*SFAC)*pixf, y0+vff+vff*AFAC-uff*SFAC)
                 << QPointF(x0+(uff+uff*AFAC-vff*SFAC)*pixf, y0+vff+vff*AFAC+uff*SFAC);
            painter.drawPolygon(head);
          } else {
            painter.drawLine(x0+uff*pixf, y0+vff, x0+(uff+uff*AFAC+vff*SFAC)*pixf, y0+vff+vff*AFAC-uff*SFAC);
            painter.drawLine(x0+uff*pixf, y0+vff, x0+(uff+uff*AFAC-vff*SFAC)*pixf, y0+vff+vff*AFAC+uff*SFAC);
          }
        }
      }
    }

    if (labelOnLine){
      painter.setFontSize(fontSize);
      float bx1, bx2, by1, by2;

      const QString qtext = QString::fromStdString(labelText);
      const QSizeF bbx = painter.getTextSize(qtext);
      const float th = bbx.height(), tw=bbx.width();

      float marg= th*0.15;
      bx1= labelx - marg;
      bx2= bx1 + 2*marg + tw;
      if (labelside==SV_BOTTOM)
        labely -= (marg + 5 + th);
      else
        labely += marg+5;
      by1= labely - marg;
      by2= by1 + th + 2*marg;
      painter.setFill(ptColor("WHITE"));
      painter.setLineStyle(NOLINE);
      painter.drawRect(bx1, by1, bx2, by2);

      painter.setColor(color);
      painter.drawText(qtext, labelx, labely);
    }
#ifdef DEBUG
    cout << "LineElement::plot(ptPainter& painter) finished" << endl;
#endif
  }
}

} // namespace pets2

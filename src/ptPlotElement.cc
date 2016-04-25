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


// ptPlotElement.cc : Class definition for generic plotelement

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptPlotElement.h"
#include <puTools/miStringFunctions.h>

#include <cmath>

// #define DEBUG
#ifdef DEBUG
#include <iostream>
#endif // DEBUG

using namespace miutil;

namespace pets2 {

PlotElement::PlotElement(const Layout& layout, const ptVertFieldf& field,
    XAxisInfo *pXtime, const bool& en, PlotElement* pNext)
  : type(DUM_PRIMITIVE)
  , name(layout.name)
  , enabled(en)
  , visible(layout.visible)
  , startY(field.y1)
  , stopY(field.y2)
  , color(layout.color)
  , xtime(pXtime)
  , fontSize(layout.font)
  , align(layout.align)
  , canvas(0)
  , startT(0)
  , stopT(pXtime->xcoord.size()-1)
  , AllTimesAxisScale(true)
  , pInColour(layout.patternInColour)
  , datalimits(layout.datalimits)
  , colorlist(layout.colorlist)
  , next(pNext) // FIXME public
{
#ifdef DEBUG
  cout << "Inside PlotElement's constructor" << endl;
#endif
}

PlotElement::~PlotElement()
{
}

void PlotElement::setViewport(ptCanvas* c)
{
  canvas = c;
}


void PlotElement::setTimeInterval(const int start, const int stop)
{
  if (start >= 0 && start < xtime->xcoord.size())
    startT = start;
  if (stop >= 0 && stop < xtime->xcoord.size())
    stopT = stop;
}

void PlotElement::getRectangle(float& x1,float& y1,
    float& x2,float& y2)
{
  x1= globalWindow.x1;
  x2= globalWindow.x2;
  y1= startY;
  y2= stopY;
}

void PlotElement::getTimeRectangle(float& x1,float& y1,
    float& x2,float& y2)
{
  x1= xtime->xcoord[startT];
  x2= xtime->xcoord[stopT];
  y1= startY;
  y2= stopY;
}

int PlotElement::dataLimitIndex(const float v)
{
  int nd= datalimits.size();
  int i;
  for (i=0; i<nd; i++){
    if ( v < datalimits[i] ){
      break;
    }
  }
  return ( (i==0 ? 0 : i - 1 ) );
}

ptColor PlotElement::colorfromvalue(const float v)
{
  ptColor c;
  int nd= datalimits.size();
  int nc= colorlist.size();
  if ( nc > 0 ) c = colorlist[0];
  int i;
  for (i=0; i<nd; i++){
    if ( v < datalimits[i] ){
      break;
    } else {
      c = ( (i < nc) ? colorlist[i] : colorlist[nc-1] );
    }
  }
  return c;
}

ptColor PlotElement::colorfromindex(const int i)
{
  ptColor c;
  int nc= colorlist.size();
  if ( nc > 0 ) c = colorlist[0];
  if ( i >= 0 ) c = ( (i < nc) ? colorlist[i] : colorlist[nc-1] );
  return c;
}

/*
  B-spline smooth
 */
int PlotElement::smoothline(int npos, float x[], float y[], int nfirst, int nlast,
    int ismooth, float xsmooth[], float ysmooth[])
{
  // Smooth line, make and return spline through points.
  //
  //  input:
  //     x(n),y(n), n=1,npos:   x and y in "window" coordinates
  //     x(nfrst),y(nfsrt):     first point
  //     x(nlast),y(nlast):     last  point
  //     ismooth:               number of points spline-interpolated
  //                            between each pair of input points
  //
  //  method: 'hermit interpolation'
  //     nfirst=0:      starting condition for spline = relaxed
  //     nfirst>0:      starting condition for spline = clamped
  //     nlast<npos-1:  ending   condition for spline = clamped
  //     nlast=npos-1:  ending   condition for spline = relaxed
  //        relaxed  -  second derivative is zero
  //        clamped  -  derivatives computed from nearest points

  int   ndivs, n, ns, i;
  float rdivs, xl1, yl1, s1, xl2, yl2, s2, dx1, dy1, dx2, dy2;
  float c32, c42, c31, c41, fx1, fx2, fx3, fx4, fy1, fy2, fy3, fy4;
  float tstep, t, t2, t3;

  if (npos<3 || nfirst<0 || nfirst>=nlast
      || nlast>npos-1 || ismooth<1) {
    nfirst = (nfirst>0)     ? nfirst : 0;
    nlast  = (nlast<npos-1) ? nlast  : npos-1;
    ns = 0;
    for (n=nfirst; n<=nlast; ++n) {
      xsmooth[ns] = x[n];
      ysmooth[ns] = y[n];
      ++ns;
    }
    return ns;
  }

  ndivs = ismooth;
  rdivs = 1./float(ismooth+1);

  n = nfirst;
  if (n > 0)
  {
    xl1 = x[n]-x[n-1];
    yl1 = y[n]-y[n-1];
    s1  = sqrtf(xl1*xl1+yl1*yl1);
    xl2 = x[n+1]-x[n];
    yl2 = y[n+1]-y[n];
    s2  = sqrtf(xl2*xl2+yl2*yl2);
    dx2 = (xl1*(s2/s1)+xl2*(s1/s2))/(s1+s2);
    dy2 = (yl1*(s2/s1)+yl2*(s1/s2))/(s1+s2);
  }
  else
  {
    xl2 = x[n+1]-x[n];
    yl2 = y[n+1]-y[n];
    s2  = sqrtf(xl2*xl2+yl2*yl2);
    dx2 = xl2/s2;
    dy2 = yl2/s2;
  }

  xsmooth[0] = x[nfirst];
  ysmooth[0] = y[nfirst];
  ns = 0;

  for (n=nfirst+1; n<=nlast; ++n)
  {
    xl1 = xl2;
    yl1 = yl2;
    s1  = s2;
    dx1 = dx2;
    dy1 = dy2;

    if (n < npos-1) {
      xl2 = x[n+1]-x[n];
      yl2 = y[n+1]-y[n];
      s2  = sqrtf(xl2*xl2+yl2*yl2);
      dx2 = (xl1*(s2/s1)+xl2*(s1/s2))/(s1+s2);
      dy2 = (yl1*(s2/s1)+yl2*(s1/s2))/(s1+s2);
    }
    else {
      dx2 = xl1/s1;
      dy2 = yl1/s1;
    }

    // four spline coefficients for x and y
    c32 =  1./s1;
    c42 =  c32*c32;
    c31 =  c42*3.;
    c41 =  c42*c32*2.;
    fx1 =  x[n-1];
    fx2 =  dx1;
    fx3 =  c31*xl1-c32*(2.*dx1+dx2);
    fx4 = -c41*xl1+c42*(dx1+dx2);
    fy1 =  y[n-1];
    fy2 =  dy1;
    fy3 =  c31*yl1-c32*(2.*dy1+dy2);
    fy4 = -c41*yl1+c42*(dy1+dy2);

    // make 'ismooth' straight lines, from point 'n-1' to point 'n'

    tstep = s1*rdivs;
    t = 0.;

    for (i=0; i<ndivs; ++i) {
      t += tstep;
      t2 = t*t;
      t3 = t2*t;
      ns++;
      xsmooth[ns] = fx1 + fx2*t + fx3*t2 + fx4*t3;
      ysmooth[ns] = fy1 + fy2*t + fy3*t2 + fy4*t3;
    }

    ns++;
    xsmooth[ns] = x[n];
    ysmooth[ns] = y[n];
  }

  ns++;

  return ns;
}

// ================ dataPlotElement

void dataPlotElement::setTimeInterval(const int start, const int stop)
{
  PlotElement::setTimeInterval(start,stop);
  if (AllTimesAxisScale) return;
  // scale Y-axis to the data inside this time-interval
  int i1=0;
  for (int i=0; i<startT; i++) if (valid(i)) i1++;
  int i2=i1;
  for (int i=i2; i<stopT; i++) if (valid(i)) i2++;
  i2--;
  if (fdata){
    fdata->setTimeInterval(i1,i2);
  }
}

int dataPlotElement::datastart()// find number of points before startT
{
  int j = 0;
  for (int i = 0; i < startT; i++)
    if (valid(i))
      j++;
  return j;
}

int dataPlotElement::dataend()// find number of points before stopT
{
  int j = 0;
  for (int i = 0; i < stopT; i++)
    if (valid(i))
      j++;
  return j;
}

} // namespace pets2

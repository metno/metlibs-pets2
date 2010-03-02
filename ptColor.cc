/*
  libpets2 - presentation and editing of time series

  $Id$

  Copyright (C) 2006 met.no

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptColor.h>

using namespace miutil;
using namespace std;

map<miString,ptColor> ptColor::colorlist;
bool ptColor::initialised= false;

ptColor::ptColor()
{
  if (!initialised)
    initColorlist();
  name = "BLACK";
  colTable[0] = 0.0;
  colTable[1] = 0.0;
  colTable[2] = 0.0;
  colTable[3] = 1.0;
}

ptColor::ptColor(const miString& n)
{
  if (!initialised)
    initColorlist();
  fromStr(n);
}

ptColor::ptColor(const miString& n, float r, float g, float b, float a)
  : name(n.upcase())
{
  colTable[0] = r;
  colTable[1] = g;
  colTable[2] = b;
  colTable[3] = a;

  colorlist[name] = *this;
}

void ptColor::initColorlist()
{
  initialised = true;
  ptColor("BLACK"        , 0.00, 0.00, 0.00, 1.00);
  ptColor("BLUE"         , 0.00, 0.00, 1.00, 1.00);
  ptColor("GREEN"        , 0.00, 1.00, 0.00, 1.00);
  ptColor("CYAN"         , 0.00, 1.00, 1.00, 1.00);
  ptColor("RED"          , 1.00, 0.00, 0.00, 1.00);
  ptColor("MAGENTA"      , 1.00, 0.00, 1.00, 1.00);
  ptColor("YELLOW"       , 1.00, 1.00, 0.00, 1.00);
  ptColor("WHITE"        , 1.00, 1.00, 1.00, 1.00);
  ptColor("GREY25"       , 0.25, 0.25, 0.25, 1.00);
  ptColor("GREY40"       , 0.40, 0.40, 0.40, 1.00);
  ptColor("GREY45"       , 0.45, 0.45, 0.45, 1.00);
  ptColor("GREY50"       , 0.50, 0.50, 0.50, 1.00);
  ptColor("GREY55"       , 0.55, 0.55, 0.55, 1.00);
  ptColor("GREY60"       , 0.60, 0.60, 0.60, 1.00);
  ptColor("GREY65"       , 0.65, 0.65, 0.65, 1.00);
  ptColor("GREY70"       , 0.70, 0.70, 0.70, 1.00);
  ptColor("GREY75"       , 0.75, 0.75, 0.75, 1.00);
  ptColor("GREY80"       , 0.80, 0.80, 0.80, 1.00);
  ptColor("GREY85"       , 0.85, 0.85, 0.85, 1.00);
  ptColor("GREY90"       , 0.90, 0.90, 0.90, 1.00);
  ptColor("GREY95"       , 0.95, 0.95, 0.95, 1.00);
  ptColor("MIST_RED"     , 0.94, 0.86, 0.86, 1.00);
  ptColor("MIST_GREEN"   , 0.86, 0.94, 0.86, 1.00);
  ptColor("MIST_BLUE"    , 0.86, 0.94, 0.94, 1.00);
  ptColor("DARK_GREEN"   , 0.00, 0.50, 0.50, 1.00);
  ptColor("BROWN"        , 0.70, 0.20, 0.00, 1.00);
  ptColor("ORANGE"       , 1.00, 0.35, 0.00, 1.00);
  ptColor("PURPLE"       , 0.30, 0.00, 0.30, 1.00);
  ptColor("LIGHT_BLUE"   , 0.20, 0.20, 1.00, 1.00);
  ptColor("DARK_YELLOW"  , 0.70, 0.70, 0.00, 1.00);
  ptColor("DARK_RED"     , 0.50, 0.00, 0.00, 1.00);
  ptColor("DARK_BLUE"    , 0.00, 0.00, 0.50, 1.00);
  ptColor("DARK_CYAN"    , 0.00, 0.50, 0.50, 1.00);
  ptColor("DARK_MAGENTA" , 0.50, 0.00, 0.50, 1.00);
  ptColor("MIDNIGHT_BLUE", 0.10, 0.10, 0.43, 1.00);
  ptColor("DNMI_GREEN"   , 0.17, 0.47, 0.14, 1.00);
  ptColor("DNMI_BLUE"    , 0.00, 0.21, 0.49, 1.00);
  ptColor("RUST_RED"     , 0.67, 0.15, 0.11, 1.00);
  ptColor("DAWN_RED"     , 1.00, 0.20, 0.00, 1.00);
  ptColor("SUN_YELLOW"   , 1.00, 0.91, 0.20, 1.00);
  ptColor("SPRING_GREEN" , 0.71, 0.83, 0.28, 1.00);
  ptColor("IRR_GREEN"    , 0.67, 0.75, 0.03, 1.00);
  ptColor("MOSS_GREEN"   , 0.60, 0.60, 0.00, 1.00);
  ptColor("GRASS_GREEN"  , 0.00, 0.40, 0.21, 1.00);
  ptColor("THUNDER_GREY" , 0.12, 0.10, 0.29, 1.00);
  ptColor("SEA_BLUE"     , 0.00, 0.20, 0.40, 1.00);
  ptColor("SKY_BLUE"     , 0.40, 0.60, 0.80, 1.00);
  ptColor("ICE_BLUE"     , 0.60, 0.80, 0.80, 1.00);
  ptColor("GLACIER_WHITE", 0.95, 0.95, 0.80, 1.00);
  ptColor("RAIN_GREY"    , 0.56, 0.52, 0.46, 1.00);
  ptColor("IVORY"        , 0.93, 0.93, 0.88, 1.00);
  ptColor("DARK_IVORY"   , 0.88, 0.89, 0.84, 1.00);

  ptColor("BLUEGREY"     , 0.788,0.796,0.810,1.00);
  ptColor("GREYBROWN"    , 0.705,0.660,0.600,1.00);
  ptColor("OLIVE"        , 0.705,0.540,0.660,1.00);
  ptColor("MUDDYGREEN"   , 0.545,0.557,0.407,1.00);
  ptColor("GREYBLUE"     , 0.370,0.640,0.815,1.00);
  ptColor("DARK_BLUEGREY", 0.560,0.619,0.705,1.00);
  ptColor("DARKOLIVE"    , 0.540,0.396,0.490,1.00);

  ptColor("RED_YELLOW"   , 1.000,0.870,0.376,1.00);
  ptColor("YELLOW_RED"   , 0.920,0.176,0.180,1.00);

}

void ptColor::fromStr(const miString& s)
{
  miString col = s.upcase(), alpha;
  vector<miString> vs= col.split(":");

  if (vs.size() == 5){         // full registration
    float r= atof(vs[1].cStr());
    float g= atof(vs[2].cStr());
    float b= atof(vs[3].cStr());
    float a= atof(vs[4].cStr());
    *this = ptColor(vs[0],r,g,b,a);
    return;

  } else if (vs.size() == 4 && !vs[0].isNumber()){// registration, no alpha
    float r= atof(vs[1].cStr());
    float g= atof(vs[2].cStr());
    float b= atof(vs[3].cStr());
    *this = ptColor(vs[0],r,g,b,1.0);
    return;

  } else if (vs.size() == 4){  // colourcomponents + alpha
    float r= atof(vs[0].cStr());
    float g= atof(vs[1].cStr());
    float b= atof(vs[2].cStr());
    float a= atof(vs[3].cStr());
    *this = ptColor("UNKNOWN",r,g,b,a);
    return;

  } else if (vs.size() == 3){  // only colourcomponents
    float r= atof(vs[0].cStr());
    float g= atof(vs[1].cStr());
    float b= atof(vs[2].cStr());
    *this = ptColor("UNKNOWN",r,g,b,1.0);
    return;

  } else if (vs.size() == 2){  // name + alpha
    col=   vs[0];
    alpha= vs[1];

  } else if (vs.size() == 1){  // only name
    col=   vs[0];
  }

  if (col.exists() && colorlist.count(col) > 0){
    *this = colorlist[col];
  } else {
    colTable[0]=colTable[1]=colTable[2]=0.0;
    colTable[3]=1.0;
  }

  if (alpha.length() > 0){
    float a= atof(alpha.cStr());
    colTable[3] = a;
  }
}

miString ptColor::Color2Str() const
{
  return name;
}

vector<ptColor> ptColor::Str2Colorlist(const miString& s)
{
  vector<ptColor> list;
  vector<miString> slist= s.split(",");

  int n= slist.size();
  for (int i=0; i<n; i++){
    ptColor c;
    c.fromStr(slist[i]);
    list.push_back(c);
  }

  return list;
}

miString ptColor::Colorlist2Str(const vector<ptColor>& list)
{
  miString buf;
  int n= list.size();
  for (int i=0; i<n; i++){
    if (i>0) buf+= ",";
    buf+= list[i].Color2Str();
  }

  return buf;
}


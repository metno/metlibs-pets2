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


// ptLayout.cc

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptLayout.h"

using namespace miutil;

ptAxis Str2Axis(miString buf){
  miString axis = buf.upcase();
  axis.trim(true,true);
  if (axis=="LEFTLEFT")        return LEFTLEFT;
  else if (axis=="RIGHTRIGHT") return RIGHTRIGHT;
  else if (axis=="LEFTRIGHT")  return LEFTRIGHT;
  else if (axis=="RIGHTLEFT")  return RIGHTLEFT;
  else if (axis=="NO_AXIS")    return NO_AXIS;
  else return NO_AXIS;
}

ptAlign Str2Align(miString buf){
  miString align = buf.upcase();
  align.trim(true,true);
  if (align=="CENTER")     return CENTER;
  else if (align=="LEFT")  return LEFT;
  else if (align=="RIGHT") return RIGHT;
  else if (align=="OVER")  return OVER;
  else if (align=="BELOW") return BELOW;
  else return CENTER;
}

ptSize Str2Size(miString buf){
  miString size = buf.upcase();
  size.trim(true,true);
  if (size=="X_SMALL")      return X_SMALL;
  else if (size=="SMALL")   return SMALL;
  else if (size=="MEDIUM")  return MEDIUM;
  else if (size=="LARGE")   return LARGE;
  else if (size=="X_LARGE") return X_LARGE;
  else if (size=="ZERO")    return ZERO;
  else return ZERO;
}

ptHorSides Str2HorSides(miString buf){
  miString side = buf.upcase();
  side.trim(true,true);
  if (side=="NONE")       return SH_NONE;
  else if (side=="LEFT")  return SH_LEFT;
  else if (side=="RIGHT") return SH_RIGHT;
  else if (side=="BOTH")  return SH_BOTH;
  else return SH_NONE;
}

ptVerSides Str2VerSides(miString buf){
  miString side = buf.upcase();
  side.trim(true,true);
  if (side=="NONE")        return SV_NONE;
  else if (side=="TOP")    return SV_TOP;
  else if (side=="BOTTOM") return SV_BOTTOM;
  else if (side=="BOTH")   return SV_BOTH;
  else return SV_NONE;
}

ptEditStyle Str2EditStyle(miString buf){
  miString style = buf.upcase();
  style.trim(true,true);
  if (style=="LINE")           return ES_LINE;
  else if (style=="HISTOGRAM") return ES_HISTOGRAM;
  else if (style=="SWITCH")    return ES_SWITCH;
  else return ES_LINE;
}

std::vector<miString> Str2TextLabels(miString buf){
  std::vector<miString> vs;
  vs= buf.split("|",false);
  return vs;
}

std::vector<float> Str2FloatList(miString buf){
  std::vector<miString> vs;
  vs= buf.split(",",true);
  std::vector<float> vf;
  for ( int i=0; i<vs.size(); i++){
    if ( vs[i].isNumber() )
      vf.push_back( atof(vs[i].c_str()) );
    else
      vf.push_back(0.0);
  }
  return vf;
}

std::vector<int> Str2IntList(miString buf){
  std::vector<miString> vs;
  vs= buf.split(",",true);
  std::vector<int> vf;
  for ( int i=0; i<vs.size(); i++){
    if ( vs[i].isNumber() )
      vf.push_back( atoi(vs[i].c_str()) );
    else
      vf.push_back(0);
  }
  return vf;
}

ptDateStyle Str2DateStyle(miString buf){
  miString style = buf.upcase();
  style.trim(true,true);
  if (style=="DATE")        return DS_DATE;
  else if (style=="DAY")    return DS_DAY;
  else if (style=="WEEK")   return DS_WEEK;
  else if (style=="MONTH") return DS_MONTH;
  else if (style=="YEAR")   return DS_YEAR;
  else return DS_DATE;
}

miString Axis2Str(ptAxis axis){
  if (axis==LEFTLEFT)        return "LEFTLEFT";
  else if (axis==RIGHTRIGHT) return "RIGHTRIGHT";
  else if (axis==LEFTRIGHT)  return "LEFTRIGHT";
  else if (axis==RIGHTLEFT)  return "RIGHTLEFT";
  else if (axis==NO_AXIS)    return "NO_AXIS";
  else return "NO_AXIS";
}

miString Align2Str(ptAlign align){
  if (align==CENTER)     return "CENTER";
  else if (align==LEFT)  return "LEFT";
  else if (align==RIGHT) return "RIGHT";
  else if (align==OVER)  return "OVER";
  else if (align==BELOW) return "BELOW";
  else return "CENTER";
}

miString Size2Str(ptSize size){
  if (size==X_SMALL)      return "X_SMALL";
  else if (size==SMALL)   return "SMALL";
  else if (size==MEDIUM)  return "MEDIUM";
  else if (size==LARGE)   return "LARGE";
  else if (size==X_LARGE) return "X_LARGE";
  else if (size==ZERO)    return "ZERO";
  else return "ZERO";
}

miString HorSides2Str(ptHorSides side){
  if (side==SH_NONE)       return "NONE";
  else if (side==SH_LEFT)  return "LEFT";
  else if (side==SH_RIGHT) return "RIGHT";
  else if (side==SH_BOTH)  return "BOTH";
  else return "NONE";
}

miString VerSides2Str(ptVerSides side){
  if (side==SV_NONE)        return "NONE";
  else if (side==SV_TOP)    return "TOP";
  else if (side==SV_BOTTOM) return "BOTTOM";
  else if (side==SV_BOTH)   return "BOTH";
  else return "NONE";
}

miString EditStyle2Str(ptEditStyle style){
  if (style==ES_LINE)           return "LINE";
  else if (style==ES_HISTOGRAM) return "HISTOGRAM";
  else if (style==ES_SWITCH)    return "SWITCH";
  else return "LINE";
}

miString TextLabels2Str(const std::vector<miString>& vs)
{
  miString s;
  for (int i=0; i<vs.size(); i++)
    s+= (vs[i] + "|");
  return s;
}

miString FloatList2Str(const std::vector<float>& vf)
{
  miString s;
  for (int i=0; i<vf.size(); i++)
    s+= (miString(vf[i]) + ",");
  return s;
}

miString IntList2Str(const std::vector<int>& vf)
{
  miString s;
  for (int i=0; i<vf.size(); i++)
    s+= (miString(vf[i]) + ",");
  return s;
}

miString DateStyle2Str(ptDateStyle style){
  if (style==DS_DATE)        return "DATE";
  else if (style==DS_DAY)    return "DAY";
  else if (style==DS_WEEK)   return "WEEK";
  else if (style==DS_MONTH)  return "MONTH";
  else if (style==DS_YEAR)   return "YEAR";
  else return "DATE";
}

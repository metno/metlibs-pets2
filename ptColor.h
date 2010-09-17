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


#ifndef _ptColor_h
#define _ptColor_h

#include <puTools/miString.h>

#include <vector>
#include <map>

struct ptColor {
  miutil::miString name;
  float colTable[4];
  static std::map<miutil::miString,ptColor> colorlist;
  static bool initialised;

  ptColor();
  ptColor(const miutil::miString& n);
  ptColor(const miutil::miString& n, float r, float g, float b, float a=1);

  static void initColorlist();
  void fromStr(const miutil::miString&);
  miutil::miString Color2Str() const;
  static std::vector<ptColor> Str2Colorlist(const miutil::miString&);
  static miutil::miString Colorlist2Str(const std::vector<ptColor>&);

  bool operator==(const ptColor &rhs) const
  {
    return (name == rhs.name);
  }
  bool operator!=(const ptColor &rhs) const
  {
    return (name != rhs.name);
  }
  friend std::ostream& operator<<(std::ostream& out, const ptColor& rhs)
  {
    return out << rhs.name;
  }
};


#endif

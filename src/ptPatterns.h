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


// ptPatterns.h : Definitions of polygon and line patterns

#ifndef PETS2_PATTERNS_H
#define PETS2_PATTERNS_H

#include <string>
#include <vector>

namespace pets2 {

enum ptFillStyle {
  NONE,
  DIAGRIGHT,
  DIAGLEFT,
  DIAGCROSS,
  HORIZONTAL,
  VERTICAL,
  SHORIZONTAL,
  SVERTICAL,
  SQUARE,
  SOLID
};

enum ptLineStyle {
  FULL,
  DASHED,
  DASHDOTTED,
  DASHDASHDOTTED,
  DOTTED,
  NOLINE
};

extern const unsigned char* fillPattern(ptFillStyle);

extern ptFillStyle Str2FillStyle(const std::string&);
extern ptLineStyle Str2LineStyle(const std::string&);
extern std::vector<ptFillStyle> Str2FillStyleList(const std::string&);
extern std::vector<ptLineStyle> Str2LineStyleList(const std::string&);

extern std::string FillStyle2Str(ptFillStyle);
extern std::string LineStyle2Str(ptLineStyle);
extern std::string FillStyleList2Str(const std::vector<ptFillStyle>&);
extern std::string LineStyleList2Str(const std::vector<ptLineStyle>&);

} // namespace pets2

#endif // PETS2_PATTERNS_H

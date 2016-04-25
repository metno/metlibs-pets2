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


#ifndef PETS2_FONTDEFS_H
#define PETS2_FONTDEFS_H

#include <string>

namespace pets2 {

const int MAXFONTS = 4;
const int MAXFONTSIZES = 11;
const int MAXFONTFACES = 3;

enum ptFontSize {
  NO_SIZE,
  HEADER1,
  HEADER2,
  HEADER3,
  NORMAL,
  SUBSCRIPT,
  USERDEFINED1,
  USERDEFINED2,
  USERDEFINED3,
  USERDEFINED4,
  USERDEFINED5,
};

enum ptFontFace {
  F_NORMAL,
  F_BOLD,
  F_ITALIC
};

enum ptPrintMode {
  M_BITMAP,
  M_LINE
};

extern ptFontSize Str2FontSize(const std::string&);
extern std::string FontSize2Str(ptFontSize);

extern ptFontFace Str2FontFace(const std::string&);
extern std::string FontFace2Str(ptFontFace);

} // namespace pets2

#endif // PETS2_FONTDEFS_H

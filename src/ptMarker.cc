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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ptMarker.h>
#include <puTools/miStringFunctions.h>

using namespace std;

namespace pets2 {

ptMarker Str2Marker(const std::string& buffer)
{
  const std::string mar = miutil::trimmed(miutil::to_upper(buffer), true, true);

  if (mar=="NO_MARKER")        return NO_MARKER;
  else if (mar=="M_CIRCLE")    return M_CIRCLE;
  else if (mar=="M_RECTANGLE") return M_RECTANGLE;
  else if (mar=="M_TRIANGLE")  return M_TRIANGLE;
  else if (mar=="M_DIAMOND")   return M_DIAMOND;
  else if (mar=="M_STAR")      return M_STAR;
  else return NO_MARKER;
}

std::string Marker2Str(ptMarker mar)
{
  if (mar==NO_MARKER)        return "NO_MARKER";
  else if (mar==M_CIRCLE)    return "M_CIRCLE";
  else if (mar==M_RECTANGLE) return "M_RECTANGLE";
  else if (mar==M_TRIANGLE)  return "M_TRIANGLE";
  else if (mar==M_DIAMOND)   return "M_DIAMOND";
  else if (mar==M_STAR)      return "M_STAR";
  else return "NO_MARKER";
}

} // namespace pets2

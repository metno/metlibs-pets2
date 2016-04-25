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


#ifndef PETS2_INTERVALELEMENT_H
#define PETS2_INTERVALELEMENT_H

#include "ptGlobals.h"
#include "ptPlotElement.h"
#include <puTools/miTime.h>

namespace pets2 {

class IntervalElement : public PlotElement
{
public:
  struct tinterval {
    miutil::miTime start;
    miutil::miTime stop;
    std::vector<std::string> text;
  };
private:
  std::string label;
  std::string timetext;
  ptHorSides arrows;
  std::vector<std::string> vt; // timetext split by ';'
  std::vector<tinterval> intervals;
  float lineWidth;
  std::vector<miutil::miTime> timeLine;
  ptLineStyle style;
public:
  IntervalElement(const std::vector<miutil::miTime>& tline,
      const ptVertFieldf& field,
      const Layout& layout,
      XAxisInfo* xtime);

  void plot(ptPainter& painter);
  void setTimes(const std::vector<tinterval>& t);
  bool needsData() { return false; }
};

} // namespace pets2

#endif // PETS2_INTERVALELEMENT_H

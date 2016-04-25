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

#include "ptImagePlot.h"

#include "ptPainter.h"

namespace pets2 {

ptImagePlot::ptImagePlot()
{ }

ptImagePlot::ptImagePlot(const std::string& fname)
{
  setimage(fname);
}

ptImagePlot::~ptImagePlot()
{
}

void ptImagePlot::setimage(const std::string& fname)
{
  image = QImage(fname.c_str());
}

void ptImagePlot::plot(ptPainter& painter, const float& x, const float& y, const float& scale)
{
  if (!image.isNull()) {
    painter.drawImage(x, y, image, scale);
  }
}

} // namespace pets2

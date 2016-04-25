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

// ptGlobals.h : global constants and declarations for PETS

#ifndef PETS2_GLOBALS_H
#define PETS2_GLOBALS_H

#include <cfloat>

namespace pets2 {

const float F_UNDEF = -32767.0;
const int   I_UNDEF = -32767;

const int MAXPRIMF = 200; //max number of file primitives
const int MAXPRIM  = 200; //max number of addititional primitives

} // namespace pets2

struct ptPointf {
  float x,y;
};

struct ptPointi {
  int x,y;
};

struct ptRectf {
  float x1,y1,x2,y2;
};

struct ptRecti {
  int x1,y1,x2,y2;
};

const ptRectf globalWindow = {0.0,0.0,1500.0,1000.0};

struct ptVertFieldi {
  int y1, y2;
};

struct ptHorFieldi {
  int x1, x2;
};

struct ptVertFieldf {
  float y1, y2;
};

struct ptHorFieldf {
  float x1, x2;
};

#endif

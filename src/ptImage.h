/*
  libpets2 - presentation and editing of time series

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


#ifndef _ptImage_h
#define _ptImage_h

#include <string>

class ptImage {
private:
  std::string filename;  // bitmap-filename
  int width, height;     // geometry in pixels
  bool blending;         // blending on/off
  unsigned char *data;   // the actual bitmap-data
  unsigned long size;    // the size of data
  int nchannels;         // 3=RGB, 4=RGBA

  void ReadImage();
  void ReadRawImage();
  void ReadPNGImage();
  bool imageFromXpmdata(const char**);
  void ReadXPMImage();
public:
  ptImage(); // default constructor
  // constructor takes the bitmap filename and blending
  ptImage(const std::string& fname, bool blend);
  ptImage(const ptImage&); // copy constructor
  ptImage& operator=(const ptImage&); //equal operator

  ~ptImage();

  void setImage(const std::string& fname, bool blend);

  const unsigned char* Data() const {return data; }
  unsigned long Size() const {return size; }
  int Width() const {return width;  }
  int Height()const {return height; }
  const std::string& Filename() const {return filename; }
  bool Alpha() const {return blending; }
};

#endif
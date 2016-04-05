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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptImage.h"
#include <puTools/miStringFunctions.h>

#include <png.h>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <fstream>

using namespace std;


ptImage::ptImage()
    : width(0), height(0), data(0), size(0)
{
#ifdef DEBUG
  cout << "Inside ptImage constructor" << endl;
#endif
}

ptImage::ptImage(const std::string& fname, const bool blend)
    : width(0), height(0), data(0), size(0)
{
#ifdef DEBUG
  cout << "Inside ptImage constructor, kaller setImage" << endl;
#endif
  setImage(fname,blend);
}

ptImage::ptImage(const ptImage& rhs)
{
#ifdef DEBUG
  cout << "Inside ptImage copy constructor" << endl;
#endif
  size = rhs.size;
  filename = rhs.filename;
  width = rhs.width;
  height = rhs.height;
  blending = rhs.blending;
  data= 0;
  if (size){
    data = new unsigned char[size];
    for (unsigned long i=0; i<size; i++) data[i]=rhs.data[i];
  }
}

ptImage& ptImage::operator=(const ptImage& rhs)
{
#ifdef DEBUG
  cout << "Inside ptImage equal operator" << endl;
#endif
  if (this != &rhs){
    delete[] data;
    data= 0;
    size = rhs.size;
    filename = rhs.filename;
    width = rhs.width;
    height = rhs.height;
    blending = rhs.blending;
    if (size){
      data = new unsigned char[size];
      for (unsigned long i=0; i<size; i++) data[i]=rhs.data[i];
    }
  }
  return *this;
}

ptImage::~ptImage()
{
#ifdef DEBUG
  cout << "Inside ptImage destructor:" << filename << endl;
#endif
  delete[] data;
}



void ptImage::setImage(const std::string& fname, const bool blend)
{
#ifdef DEBUG
  cout << "RGBIMAGE: Initialiserer ny rgbimage med filnavn:" << fname << endl;
#endif
  if (fname.length()){
    filename = fname;
    blending = blend;
    ReadImage();
  }
}


void ptImage::ReadImage()
{
#ifdef DEBUG
  cout << "RGBIMAGE: Reading image from:" << filename << endl;
#endif
  delete[] data;
  data= 0;
  width=height=0;
  size=0;

  // Check filetype
  const vector<std::string> parts= miutil::split(filename, ".");
  if (parts.size()<2) return;

  const std::string& extension=parts.back();

  if (extension=="ppm") {
    ReadRawImage();
  } else if (miutil::contains(extension, "png")) {
    ReadPNGImage();
  } else if (miutil::contains(extension, "xpm")) {
    ReadXPMImage();
  } else {
#ifdef DEBUG
    cerr << "ptImage::ReadImage BAD EXTENSION:" << extension << endl;
#endif
  }

  if (data) {
#ifdef DEBUG
    cout << "RGBIMAGE: Success. Width:" <<  width <<
      " Height:" << height << endl;
#endif
  } else {
#ifdef DEBUG
    cout << "RGBIMAGE: Oops. imagedata = NULL" << endl;
#endif
    width=height=0;
    data=0;
    size=0;
  }
}

void ptImage::ReadRawImage()
{
  FILE *fp;
  char magic[10],dataline[105];
  int npixels, nchannels, colours, i, j, pixidx;
  char ccom[3],acom;

  if ((fp=fopen(filename.c_str(),"r"))) {

    if (fscanf(fp,"%10s \n", magic) != 1 // should be sizeof(magic)
        || strcmp(magic,"P6") != 0)
    {
      cout << "RAWIMAGE: invalid fileformat" << endl;
      return;
    }

    if (!fgets(dataline, sizeof(dataline), fp)) {
      cout << "RAWIMAGE: invalid fileformat" << endl;
      return;
    }
    while (dataline[0] == '#') {
      if (!fgets(dataline, sizeof(dataline), fp)) {
        cout << "RAWIMAGE: invalid fileformat" << endl;
        return;
      }
    }

    sscanf(dataline, "%d %d",&width,&height);
    if (!fgets(dataline, sizeof(dataline), fp)) {
      cout << "RAWIMAGE: invalid fileformat" << endl;
      return;
    }
    if (sscanf(dataline, "%d", &colours) != 1) {
      cout << "RAWIMAGE: invalid fileformat" << endl;
      return;
    }

    npixels = width * height;
    if (blending)
      nchannels = 4;
    else
      nchannels = 3;
    size = npixels*nchannels;
    data = new unsigned char[size];

    acom=0xff;
    if (data) {
      for (i=0; i<height; i++) {
        for (j=0; j<width*nchannels; j+=nchannels) {
          if (fread(ccom, sizeof(char), 3, fp) != 3) {
            ccom[0] = ccom[1] = ccom[2] = 0;
          }
          if (blending) {
            if (ccom[0]==0 && ccom[1]==0 && ccom[2]==0)
              acom=0;
            else
              acom=0xff;
          }
          pixidx = (height-(i+1))*width*nchannels + j;
          data[pixidx  ]= ccom[0];
          data[pixidx+1]= ccom[1];
          data[pixidx+2]= ccom[2];
          if (blending)
            data[pixidx+3]= acom;
        }
      }
    } else {
      cout << "RAWIMAGE: Allocation error" << endl;
    }

    fclose(fp);
  }  else {
    cout << "RAWIMAGE: Open file error" << endl;
  }
}

void ptImage::ReadPNGImage()
{
//   cerr << "--------- read_png: " << filename << endl;

#ifdef linux
  FILE *fp = fopen(filename.c_str(), "rb");
  if (!fp){
    cerr << "ReadPNGImage ERROR can't open file:" << filename << endl;
    return;
  }

  png_structp png_ptr = png_create_read_struct
    (PNG_LIBPNG_VER_STRING,
     (png_voidp)0,//user_error_ptr,
     0,//user_error_fn,
     0);//user_warning_fn);
  if (!png_ptr){
    cerr << "ReadPNGImage ERROR creating png_struct" << endl;
    fclose(fp);
    return;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr){
    png_destroy_read_struct(&png_ptr,
        (png_infopp)NULL, (png_infopp)NULL);
    cerr << "ReadPNGImage ERROR creating info_struct" << endl;
    fclose(fp);
    return;
  }

  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info){
    png_destroy_read_struct(&png_ptr, &info_ptr,
        (png_infopp)NULL);
    cerr << "ReadPNGImage ERROR creating end_info_struct" << endl;
    fclose(fp);
    return;
  }

     if (setjmp(png_jmpbuf(png_ptr))){
    png_destroy_read_struct(&png_ptr, &info_ptr,
        &end_info);
    cerr << "ReadPNGImage ERROR longjmp out of process" << endl;
    fclose(fp);
    return;
  }

  png_init_io(png_ptr, fp);


  // do the read
  const int png_transforms = 0;
  png_read_png(png_ptr, info_ptr, png_transforms, NULL);

  png_uint_32 uwidth, uheight;
  int color_type;
  int bit_depth;
  int interlace_type;//=   PNG_INTERLACE_NONE;
  int compression_type;//= PNG_COMPRESSION_TYPE_DEFAULT;
  int filter_type;//=      PNG_FILTER_TYPE_DEFAULT;

  png_get_IHDR(png_ptr, info_ptr, &uwidth, &uheight,
      &bit_depth, &color_type, &interlace_type,
      &compression_type, &filter_type);
  width= uwidth;
  height= uheight;

  //   cerr << "image width:" << img.width << endl;
  //   cerr << "image height:" << img.height << endl;
  //   cerr << "image bit_depth:" << bit_depth << endl;
  //   cerr << "image color_type:" << color_type << endl;

  nchannels=4;
  if (color_type == PNG_COLOR_TYPE_RGB_ALPHA){
    nchannels= 4;
    //cout << "PNG_COLOR_TYPE_RGB_ALPHA" << endl;
  } else if (color_type == PNG_COLOR_TYPE_RGB){
    nchannels= 3;
    //cout << "PNG_COLOR_TYPE_RGB" << endl;
  } else if (color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
    nchannels= 4;
    //cout << "PNG_COLOR_TYPE_GRAY_ALPHA" << endl;
  } else if (color_type == PNG_COLOR_TYPE_GRAY){
    nchannels= 3;
    //cout << "PNG_COLOR_TYPE_GRAY" << endl;
  } else if (color_type == PNG_COLOR_TYPE_PALETTE){
    cerr << "PNG_COLOR_TYPE_PALETTE"
         << " ..exiting" << endl;
    return;
  } else {
    cerr << "Unknown color_type:" << color_type
         << " ..exiting" << endl;
    return;
  }

  blending= (nchannels==4);

  png_bytep *row_pointers;
  row_pointers = png_get_rows(png_ptr, info_ptr);

  // unpack image from row-based structure
  size= width*height*nchannels;
  data= new unsigned char [size];
  int bp=0;
  for (int i=height-1; i>=0; i--){
    for (int j=0; j<width*nchannels; j++,bp++){
      data[bp] = row_pointers[i][j];
    }
  }

  // clean up
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  fclose(fp);

#endif
}



/*
  XPM - routines
*/


// convert a HEX-character (0-9,A-F) to int
int chartoint_(const char c)
{
  const int zv= int('0');
  const int nv= int('9');
  const int av= int('A');
  const int fv= int('F');

  int v= int(c);

  if (v >= zv && v<= nv)
    return v-zv;
  else if (v >= av && v <= fv)
    return v-av+10;

  // illegal character
  return 0;
}

int hexToInt(const std::string& p)
{
  const std::string up = miutil::to_upper(p);
  int l= up.length(), res=0, fact=1;
  for (int i=l-1; i>=0; i--,fact*=15)
    res += chartoint_(up[i])*fact;

  return res;
}


bool ptImage::imageFromXpmdata(const char** xd)
{
  int xsize=-1,ysize,ncols,nchar;
  std::string buf = xd[0];

  const vector<std::string> vs = miutil::split(buf, " ");
  if (vs.size() < 4){
    cerr << "imageFromXpmdata ERROR too few elements:" << buf << endl;
    return false;
  }
  xsize= atoi(vs[0].c_str());
  ysize= atoi(vs[1].c_str());
  ncols= atoi(vs[2].c_str());
  nchar= atoi(vs[3].c_str());

  if (xsize < 1 || ysize < 1 || ncols < 1 || nchar < 1){
    cerr << "imageFromXpmdata ERROR Illegal numbers "
         << " xsize:" << xsize << " ysize:" << ysize
         << " ncols:" << ncols << " nchar:" << nchar
         << endl;
    return false;
  }

  map<std::string,int> redmap;
  map<std::string,int> greenmap;
  map<std::string,int> bluemap;
  map<std::string,int> alphamap;

  for (int i=0; i<ncols; i++){
    buf = xd[1+i];
    std::string key=    buf.substr(0,nchar);
    std::string rest=   buf.substr(nchar,buf.length()-nchar+1);
    int j= rest.find_first_of("c");
    if (j < 0){
      cerr << "imageFromXpmdata ERROR Illegal colourdefinition (1):"
           << rest << endl;
      return false;
    }
    std::string colour= rest.substr(j+1,rest.length()-j-1);
    miutil::trim(colour);
    if (colour == "None"){
      redmap[key]  = 255;
      greenmap[key]= 255;
      bluemap[key] = 255;
      alphamap[key]= 0;
    } else if (colour == "white"){
      redmap[key]  = 255;
      greenmap[key]= 255;
      bluemap[key] = 255;
      alphamap[key]= 255;
    } else if (colour == "black" || colour == "opaque"){
      redmap[key]  = 0;
      greenmap[key]= 0;
      bluemap[key] = 0;
      alphamap[key]= 255;
    } else if (miutil::contains(colour, "gray")){
      std::string sperc= colour.substr(4,colour.length()-4);
      float perc= atof(sperc.c_str()) / 100.0;
      redmap[key]  = static_cast<int>(255*perc);
      greenmap[key]= static_cast<int>(255*perc);
      bluemap[key] = static_cast<int>(255*perc);
      alphamap[key]= 255;
    } else if (!miutil::contains(colour, "#")){
      redmap[key]  = 255;
      greenmap[key]= 255;
      bluemap[key] = 255;
      alphamap[key]= 255;
      cerr << "imageFromXpmdata File:" << filename
           << " WARNING unknown colour:" << colour << endl;
    } else {
      if (colour.size() < 2){
        cerr << "imageFromXpmdata ERROR Illegal colourdefinition (2):"
             << rest << endl;
        return false;
      }
      colour= colour.substr(1,colour.length()-1);
      int numcomp = colour.length()/3;
      redmap[key]  = hexToInt(colour.substr(0,numcomp));
      greenmap[key]= hexToInt(colour.substr(numcomp,numcomp));
      bluemap[key] = hexToInt(colour.substr(numcomp*2,numcomp));
      alphamap[key]= 255;
    }
  }

  // data
  width = xsize;
  height= ysize;
  nchannels= 4;
  blending=true;
  size= width*height*nchannels;
  data= new unsigned char [size];
  int pp= 0;
  for (int y=ysize-1; y>=0; y--){
    std::string line= xd[y+ncols+1];
    for (int x=0; x<xsize*nchar; x+=nchar){
      std::string pixel= line.substr(x,nchar);
      data[pp+0]= redmap[pixel];
      data[pp+1]= greenmap[pixel];
      data[pp+2]= bluemap[pixel];
      data[pp+3]= alphamap[pixel];
      pp+= 4;
    }
  }

  return true;
}


void ptImage::ReadXPMImage()
{
  //cerr << "--------- read_xpm: " << filename << endl;

  ifstream file(filename.c_str());

  if (!file){
    cerr << "readXpmFile ERROR: Unable to open file:"
         << filename << endl;
    return;
  }

  std::string buf;
  vector<std::string> vs, vs2;

  while(getline(file,buf)){
    miutil::trim(buf);
    if (buf.length() == 0)
      continue;
    if (buf[0]!='\"')
      continue;
    int i= buf.find_last_of("\"");
    buf= buf.substr(1,i-1);
    vs.push_back(buf);
  }
  if (vs.size() == 0)
    return;

  //   cerr << "RESULTING DATA:" << endl;
  char** tmpdata = new char*[vs.size()];
  for (size_t i=0; i<vs.size(); i++){
    tmpdata[i]= strdup(vs[i].c_str());
  }

  bool res=  imageFromXpmdata(const_cast<const char**>(tmpdata));

  // OBS: free !!!!!!!!!!!!!!!!!!!!!!!!

  delete[] tmpdata;
}

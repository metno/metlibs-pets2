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


//ptPlotElement.h class declaration for PETS PlotElement class

#ifndef PETS2_PLOTELEMENT_H
#define PETS2_PLOTELEMENT_H

#include "ptGlobals.h"
#include "ptXAxisInfo.h"
#include "ptLayout.h"
#include "ptColor.h"
#include "ptPainter.h"

#include <tsData/ptParameterDefinition.h>
#include <tsData/ptDiagramData.h>
#include <tsData/ptWeatherParameter.h>

#include <puTools/miTime.h>

#include <string>
#include <vector>

namespace pets2 {

struct DataSpec {
  ptDiagramData *dData;// diagramData
  int index;           // index to weatherparameter vector
  int comp;            // weatherparameter component
};

class PlotElement
{
protected:
  ptPrimitiveType type;       // Type of plotelement
  std::string name;              // label or name of element
  bool enabled;               // plotElement is enabled
  bool visible;               // plotElement is visible
  float startY;               // vertical startposition
  float stopY;                // vertical stopposition
  ptColor color;              // base colour
  XAxisInfo *xtime;           // x-coordinates of timepoints
  ptFontSize fontSize;        // Font size
  int fontidx;                // font index
  ptAlign align;              // Alignment
  ptCanvas* canvas; // current screen device
  int startT;                 // start of timeinterval to plot
  int stopT;                  // end of timeinterval to plot
  bool AllTimesAxisScale;     // Axis scale calculated from all time points
  bool pInColour;             // use patterns in colour
  std::vector<float> datalimits;
  std::vector<ptColor> colorlist;


  float& xval(const int& i) // return xvalue (time)
    { return xtime->xcoord[i]; }

  int dataLimitIndex(const float v);

  float dataLimit(const int i)
    { if (i < 0 || i >= (int)datalimits.size()) return 0; return datalimits[i]; }

  ptColor colorfromvalue(const float v);
  ptColor colorfromindex(const int i);
  int smoothline(int npos, float x[], float y[], int nfirst, int nlast,
      int ismooth, float xsmooth[], float ysmooth[]); // B-spline smooth

public:
  PlotElement *next; // next is made public, to let Diagram access it...
  PlotElement(const Layout& layout, const ptVertFieldf& field,
      XAxisInfo *pXtime = 0, const bool& en = true, PlotElement *pNext = 0);
  virtual ~PlotElement();

  virtual void setViewport(ptCanvas* canvas);

  const std::string& Name() const
    { return name; }

  ptPrimitiveType getType() const
    { return type; }

  virtual ParId getParId() const
    { ParId id = ID_UNDEF; return id; }

  const ptColor& Color() const
    { return color; }

  void setEnabled(const bool b)
    { enabled = b; }

  void enable()
    { enabled = true; }

  void disable()
    { enabled = false; }

  bool isEnabled() const
    { return enabled; }

  virtual void plot(ptPainter& painter)=0;

  virtual bool needsData()
    { return false; }

  virtual void switchData(void*)
    { }

  virtual void setTimeInterval(const int start, const int stop);

  void setAllTimesAxisScale(bool b)
    { AllTimesAxisScale = b; }

  void getRectangle(float& x1, float& y1, float& x2, float& y2);
  void getTimeRectangle(float& x1, float& y1, float& x2, float& y2);
  void setVisible(const bool b){visible= b; }
};


class dataPlotElement: public PlotElement
{
  ParId paramId;          // Id from Weatherparameter

protected:
  WeatherParameter *fdata; // datavalues
  DataSpec ds;             // data specification

  bool valid(const int& i) // if valid timepoint
    { return ds.dData->tobeplotted(ds.index,i);}
  int datastart();// find number of points before startT
  int dataend();// find number of points after stopT
  const float& dval(const int& i, const int& comp) // return datavalue
    { return fdata->Data(i,comp);}
  const float& dval(const int& i) // return datavalue, default component
    { return fdata->Data(i,ds.comp);}
  void setdval(const int i, const float val)
    { fdata->setData(i,ds.comp,val); fdata->setDirty(true); }
  void setdval(const int i, const int comp, const float val)
    { fdata->setData(i,comp,val); }
  int datasize() const { return fdata->Npoints(); }
  void calcDataProperties()
    { fdata->calcCompProperties(ds.comp);}
  void calcAllDataProperties()
    { fdata->calcAllProperties();}
  int datadimension() const { return fdata->Ndim(); }
  float datamin(int& comp) const {return fdata->Cmin(comp);}
  float datamin() const {return fdata->Cmin(ds.comp);}
  float datamax(int& comp) const {return fdata->Cmax(comp);}
  float datamax() const {return fdata->Cmax(ds.comp);}
  bool  datapolar() const {return fdata->Polar();}
public:
  dataPlotElement(const DataSpec cds,
      const Layout& layout,
      const ptVertFieldf& field,
      XAxisInfo *pXtime=0,
      const bool& en=true,
      PlotElement *pNext=0)
    : PlotElement(layout,field,pXtime,en,pNext),
      paramId((*cds.dData)[cds.index].Id()),
      fdata(&((*cds.dData)[cds.index])), ds(cds)
  {}
  virtual ~dataPlotElement(){}
  virtual void setTimeInterval(const int start, const int stop);
  ParId getParId() const {return paramId;}
  DataSpec getDataSpec() const {return ds;}
};

} // namespace pets2

#endif // PETS2_PLOTELEMENT_H

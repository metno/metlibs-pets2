// -*- c++ -*-
/*
 libpets2 - presentation and editing of time series

 Copyright (C) 2013-2016 met.no

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

// ptDiagram.h : Declarations for the ptDiagram class

#ifndef PETS2_DIAGRAM_H
#define PETS2_DIAGRAM_H

#include "ptGlobals.h"
#include <tsData/ptWeatherParameter.h>
#include "ptXAxisInfo.h"
#include <tsData/ptDiagramData.h>
#include "ptPlotElement.h"
#include "ptLayout.h"
#include "ptStyle.h"

#include <puTools/miTime.h>

#include <vector>
#include <map>

namespace pets2 {

class ptDiagram {
private:
  XAxisInfo xtime; // x-coordinates for the timepoints
  std::vector<miutil::miTime> timeLine; // the actual time points
  std::vector<int> timeLineIdx; // indices to complete timeline in DD
  int nPlotElements; // number of plotelements
  PlotElement *first, *last;// pointers to plotelement list
  ptDiagramData *DD; // diagramdata
  ptStyle *Style; // current style
  miutil::miTime startT, stopT; // current timeinterval
  int startidx, stopidx; // current timeinterval xaxis-indices
  bool colourFlag; // diagram in colour or black/white
  std::map<std::string, std::string> keymap; // keywords for text-strings
  bool localTime; // use localtime
  int timeZone; // which timezone
  bool showGridLines;
  void makeXtime();
  void toLocaltime(miutil::miTime& t);

public:
  ptDiagram(ptStyle*, bool showGlines=true);
  ~ptDiagram();

  // attach a diagramdata to diagram
  bool attachData(ptDiagramData*);
  // make plotelements based on style and DD
  bool makeDefaultPlotElements();
  // set current viewport both in screen pixels and in GL coordinates
  void setViewport(ptCanvas* canvas);

  // turn on scaling of Yaxis based on data from all timepoints
  void setAllTimesAxisScale(bool b);

  // set time interval for plotting
  void setTimeInterval(int, int);
  void setTimeInterval(miutil::miTime, miutil::miTime);
  void setProgInterval(int progstart, int progstop);

  // return current timeinterval, start and stop
  void getTimeInterval(int &start, int &stop);
  void getTimeInterval(miutil::miTime& start, miutil::miTime& stop);
  void getTotalTimeInterval(int &start, int &stop);
  void getTotalTimeInterval(miutil::miTime &start, miutil::miTime &stop);

  PlotElement* findElement(const std::string&, PlotElement* start = 0);// search by name
  PlotElement* findElement(ptPrimitiveType, PlotElement* start = 0);// search by type
  PlotElement* findElement(ptPrimitiveType, ParId, PlotElement* start = 0);// search by type and pid
  void addElement(PlotElement*);
  void removeElement(PlotElement*);
  void removeElement(ptPrimitiveType);
  void makeAxesLocation();
  void enableElement(PlotElement*);
  void disableElement(PlotElement*);
  void enableElement(ptPrimitiveType);
  void disableElement(ptPrimitiveType);
  void disableAll();
  void enableAll();
  void plot(ptPainter& painter);
  void tst_print();

  void toggleColour(bool use);
  bool getColour()
  {
    return colourFlag;
  }
  void setPrinting(bool print);
  void setFakeStipple(bool use);
  bool startPSoutput(const std::string& fname, const bool incolour,
      const bool inlandscape, const bool doEPS = false);
  bool startPSnewpage();
  bool endPSoutput();
  // keyword/value pair methods
  void addKeyword(const std::string&, const std::string&);
  void setKeymap(const std::map<std::string, std::string>&);
  void clearKeywords();
};

} // namespace pets2

#endif // PETS2_DIAGRAM_H

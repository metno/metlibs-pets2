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

// ptDiagram.cc: Definitions for the ptDiagram class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptDiagram.h"

#include "ptAxesElement.h"
#include "ptCloudElement.h"
#include "ptDateElement.h"
#include "ptDayElement.h"
#include "ptEditLineElement.h"
#include "ptGridlineElement.h"
#include "ptHistogramElement.h"
#include "ptLineElement.h"
#include "ptProgElement.h"
#include "ptTableElement.h"
#include "ptTextElement.h"
#include "ptUTCElement.h"
#include "ptVectorElement.h"
#include "ptWindVectorElement.h"
#include "ptYaxisElement.h"
#include "ptDoubleLineElement.h"
#include "ptSymbolElement.h"
#include "ptTimemarkerElement.h"
#include "ptMultiLineElement.h"
#include "ptAxisHistogramElement.h"
#include "ptLogoElement.h"
#include "ptBoxElement.h"
#include "ptIntervalElement.h"
#include "ptQBoxElement.h"

#include <puTools/miStringFunctions.h>

#define MILOGGER_CATEGORY "metlibs.pets2.Diagram"
#include <miLogger/miLogging.h>

using namespace miutil;
using namespace std;

// should consider additional constructor for equally spaced time points????

namespace pets2 {

ptDiagram::ptDiagram(ptStyle *style, bool showGlines)
    : nPlotElements(0)
    , first(0), last(0)
    , DD(0)
    , Style(style)
    , startidx(0), stopidx(0)
    , colourFlag(true)
    , localTime(false)
    , showGridLines(showGlines)
{
  if (Style)
    Style->getTimeSetting(localTime, timeZone);
}

// Destructor frees allocated memory
ptDiagram::~ptDiagram()
{
  PlotElement *pElement1 = first, *pElement2;

  //delete[] timeLine;              // free Nelson Mandela
  while (pElement1) { // free all the PlotElements
    pElement2 = pElement1->next;
    delete pElement1;
    pElement1 = pElement2;
  }
}

// convert miTime (UTC) to localtime
void ptDiagram::toLocaltime(miTime& t)
{
  if (!localTime)
    return;

  t.addHour(t.dst() + timeZone);
}

//---------------------------------------------------
// Name         : attachData
// Purpose      : Links a ptDiagramData (DD) to current diagram
//                Retrieves timeline from DD, and generates a XtimeLine
// Last modified:
//---------------------------------------------------
bool ptDiagram::attachData(ptDiagramData *dd)
{
  METLIBS_LOG_SCOPE();

  if (!Style)
    return false;
  if (!dd)
    return false;
  DD = dd; // attach DiagramData

  // timeLine holds the actual time points
  DD->getTimeLine(timeLine); // get timeline from DD
  int nTimePoints = timeLine.size(); // find number of timesteps in timeLine

  if (nTimePoints == 0)
    return false;

  // convert to local time
  if (localTime) {
    for (int i = 0; i < nTimePoints; i++) {
      toLocaltime(timeLine[i]);
    }
  }

  xtime.x1 = globalWindow.x1 + Style->leftMargin();
  xtime.x2 = globalWindow.x2 - Style->rightMargin();

  xtime.xcoord.clear();
  xtime.xcoord.reserve(nTimePoints);
  for (int i = 0; i < nTimePoints; i++) {
    xtime.xcoord.push_back(0.0);
  }

  startidx = 0;
  stopidx = nTimePoints - 1;
  startT = timeLine[startidx];
  stopT = timeLine[stopidx];

  makeXtime();

  if (METLIBS_LOG_DEBUG_ENABLED()) {
    METLIBS_LOG_DEBUG("timeLine:");
    for (const auto& t : timeLine)
      METLIBS_LOG_DEBUG(t);
    METLIBS_LOG_DEBUG("xtime->coord:");
    for (int i=0; i<nTimePoints;i++)
      METLIBS_LOG_DEBUG(xtime.xcoord[i]);
  }

  return 1;
}

//
void ptDiagram::makeXtime()
{
  if (!Style)
    return;

  int i;
  float startX, stopX; // left and right timeseries margins
  int nTimePoints; // number of timesteps in timeLine

  startX = xtime.x1 + Style->leftOffset();
  stopX = xtime.x2 - Style->rightOffset();

  nTimePoints = timeLine.size(); // find number of timesteps in timeLine

  // find spacing (in coordinates) between xtime points and set x-coordinate
  // values for the xtime points.
  int diff = miTime::minDiff(timeLine[stopidx], timeLine[startidx]);
  float space = (stopX - startX) / float(diff);

  for (i = 0; i < startidx; i++)
    xtime.xcoord[i] = 0.0;
  for (i = stopidx + 1; i < nTimePoints; i++)
    xtime.xcoord[i] = 0.0;
  for (i = startidx; i <= stopidx; i++) {
    xtime.xcoord[i] = startX + miTime::minDiff(timeLine[i], timeLine[startidx]) * space;
  }
}

//---------------------------------------------------
// Name         : setViewport
// Purpose      : sets the current viewport both in screen pixels and in GL coordinates
// Last modified:
//---------------------------------------------------
void ptDiagram::setViewport(ptCanvas* canvas)
{
  if (first) {
    for (PlotElement* elm = first; elm; elm = elm->next)
      elm->setViewport(canvas);
  }
}



//---------------------------------------------------
// Name         : makeDefaultPlotElements
// Purpose      : make plotelements based on current
//                weatherparameters and style
// Last modified:
//---------------------------------------------------
bool ptDiagram::makeDefaultPlotElements()
{
  METLIBS_LOG_SCOPE();
  if (!Style)
    return false;

  std::string stationName;
  std::string mainmodelName;
  StyleOrder primOrderList[MAXPRIMF + MAXPRIM], primList[MAXPRIMF + MAXPRIM];
  int nPrimOut;
  int nPlotPrim;

  int i, j = 0;
  for (i = 0; i < DD->size(); i++, j++) { // data primitives from file
    if ((*DD)[i].Type() == DONT_PLOT) {
      j--;
      continue;
    }
    primList[j].id = (*DD)[i].Id();
    primList[j].dataIndex = i;
    primList[j].dataComp = 0;
    primList[j].type = (*DD)[i].Type();
    METLIBS_LOG_DEBUG("Primitive:" << i << " type:" << primList[j].type << " Ndim:" << (*DD)[i].Ndim() << " ParId:" << primList[j].id);
  }
  // Dropper textlinjer foreloebig...
  //   for (i=0;i<count.nTextLines;j++,i++) {   // text lines from file
  //     aliasList[j] = DUM_ALIAS;
  //     primList[j] = TEXT;
  //     nPlotPrim++;
  //   }
  j++;
  primList[j].id = ID_UNDEF;
  primList[j].dataIndex = -1;
  primList[j].dataComp = -1;
  primList[j].type = STAT;
  //  nPlotPrim++;
  nPlotPrim = j;

  METLIBS_LOG_DEBUG("Organizing the plot from the style");
  if (!(Style->organize(primOrderList, &nPrimOut, primList, nPlotPrim))) {
    METLIBS_LOG_ERROR("Something catastrophic happened during style->organize\n"
                      << "Exiting");
    exit(0);
  }
  METLIBS_LOG_DEBUG("About to create plotElements");

  // loop through the primOrderList and create PlotElements for each element
  ErrorFlag ef; // OBS OBS
  ProgLine progLine; // OBS OBS
  std::string stmp;
  PlotElement *elm;
  DataSpec ds;
  ptVertFieldf field;
  pets2::Layout *layout;
  const int maxYaxis = 40;
  vector<yAxisElement*> yAxisElm(maxYaxis, (yAxisElement*) (0)); // OBS OBS

  stationName = DD->getStation().Name();
  keymap["#$"] = stationName;
  keymap["${POSITION}"] = stationName;

  std::string modeltext;
  set<Model> models = DD->allModels();
  if (models.size() > 0) {
    mainmodelName = *(models.begin());
    const vector<std::string> vs = DD->getTextLines(mainmodelName);
    for (unsigned int k = 0; k < vs.size(); k++)
      modeltext += (std::string(k > 0 ? "\n" : "") + vs[k]);
  }
  keymap["${MODEL}"] = mainmodelName;

  yAxisElement *yElm;
  ds.dData = DD;
  for (i = 0; i < nPrimOut; i++) {
    elm = 0;
    ds.index = primOrderList[i].dataIndex;
    ds.comp = primOrderList[i].dataComp;
    field = primOrderList[i].field;
    layout = &(primOrderList[i].layout);
    switch (primOrderList[i].type) {
    case YAXIS:
      elm = new yAxisElement(field, *layout, &xtime);
      yAxisElm[layout->yaid] = (yAxisElement*) elm;
      elm = 0;
      break;
    case YAXIS_STATIC:
      elm = new staticYaxisElement(field, *layout, &xtime);
      yAxisElm[layout->yaid] = (yAxisElement*) elm;
      elm = 0;
      break;
    case LINE:
      yElm = yAxisElm[layout->yaid];
      if (yElm)
        elm = new LineElement(yElm, ds, field, *layout, &xtime);
      break;
    case DOUBLE_LINE:
      yElm = yAxisElm[layout->yaid];
      if (yElm)
        elm = new DoubleLineElement(yElm, ds, field, *layout, &xtime);
      break;
    case MULTI_LINE:
      yElm = yAxisElm[layout->yaid];
      if (yElm)
        elm = new LineElement(yElm, ds, field, *layout, &xtime,true);
      break;
    case QBOX:
      yElm = yAxisElm[layout->yaid];
      if (yElm) {
        elm = new QBoxElement(yElm, ds, field, *layout, &xtime);
      }
      break;
    case EDITLINE:
      yElm = yAxisElm[layout->yaid];
      if (yElm)
        elm = new EditLineElement(yElm, ds, field, *layout, &xtime);
      break;
    case GRIDLINE:
      if(showGridLines)
        elm = new GridlineElement(timeLine, field, *layout, &xtime);
      break;
    case TIMEMARKER:
      elm = new TimemarkerElement(timeLine, field, *layout, &xtime);
      break;
    case VECTOR:
      elm = new VectorElement(ds, field, *layout, &xtime);
      break;
    case WIND_VECTOR:
      elm = new WindVectorElement(ds, field, *layout, &xtime);
      break;
    case HIST:
      elm = new HistogramElement(ds, field, *layout, &xtime, timeLine);
      break;
    case AXISHIST:
      yElm = yAxisElm[layout->yaid];
      if (yElm)
        elm = new AxisHistogramElement(yElm, ds, field, *layout, &xtime);
      break;
    case TABLE:
      elm = new TableElement(ds, field, *layout, &xtime);
      break;
    case TIMEBOX:
      elm = new BoxElement(ds, timeLine, field, *layout, &xtime);
      break;
    case CLOUDBOX:
      elm = new CloudElement(ds, field, *layout, &xtime);
      break;
    case SYMBOL:
      elm = new SymbolElement(ds, field, *layout, &xtime);
      break;
    case DATE:
      elm = new DateElement(timeLine, field, *layout, &xtime);
      break;
    case UTC:
      elm = new UTCElement(timeLine, field, *layout, &xtime);
      break;
    case DAY:
      elm = new DayElement(timeLine, field, *layout, &xtime);
      break;
    case XAXIS:
      elm = new AxesElement(field, *layout, true, &xtime);
      break;
    case PROG:
      DD->getProgLine(0, progLine, &ef); // OBS OBS
      elm = new ProgElement(progLine, field, *layout, &xtime);
      break;
    case TEXT:
      if (layout->fromFile && layout->text.length() == 0)// print text from data file
        elm = new TextElement(modeltext, keymap, field, *layout, &xtime);
      else {
        // print text from layout.text (specified in style)
        elm = new TextElement(layout->text, keymap, field, *layout, &xtime);
      }
      break;
    case STAT:
      elm = new TextElement(stationName, keymap, field, *layout, &xtime);
      break;
    case LOGO:
      elm = new LogoElement(field, *layout, &xtime);
      break;
    case INTERVAL:
      elm = new IntervalElement(timeLine, field, *layout, &xtime);
      break;
    }
    if (elm)
      addElement(elm);
  }
  // put yaxis at the end of the list
  for (i = 0; i < maxYaxis; i++)
    if (yAxisElm[i])
      addElement((PlotElement*) yAxisElm[i]);

  return true;
}

// keyword/value pair methods
void ptDiagram::addKeyword(const std::string& key, const std::string& value)
{
  keymap[key] = value;
}

void ptDiagram::setKeymap(const map<std::string, std::string>& keym)
{
  keymap = keym;
}

void ptDiagram::clearKeywords()
{
  keymap.clear();
}

void ptDiagram::setAllTimesAxisScale(bool b)
{
  for (PlotElement *elm = first; elm; elm = elm->next)
    elm->setAllTimesAxisScale(b);
}

// set timeinterval to plot by timeline-indices
void ptDiagram::setTimeInterval(int start, int stop)
{
  if (timeLine.size()) {
    if (start >= 0 && start < timeLine.size()) {
      startidx = start;
      startT = timeLine[start];
    }
    if (stop >= startidx && stop < timeLine.size()) {
      stopidx = stop;
      stopT = timeLine[stop];
    }

    makeXtime(); // calculate new x-values
    for (PlotElement *elm = first; elm; elm = elm->next) {
      elm->setTimeInterval(startidx, stopidx);
    }
  }
}

// set timeinterval to plot by specific date/clock
void ptDiagram::setTimeInterval(miTime Start, miTime Stop)
{
  if (localTime) {
    toLocaltime(Start);
    toLocaltime(Stop);
  }
  int start, stop;
  int tsize = timeLine.size();
  if (tsize) {
    for (start = 0; start < tsize && Start > timeLine[start]; start++)
      ;
    for (stop = 0; stop < tsize && Stop > timeLine[stop]; stop++)
      ;
    setTimeInterval(start, stop);
  }
}

// set the intervall by progtimes (hours)

void ptDiagram::setProgInterval(int progstart, int progstop)
{
  int tsize = timeLine.size();
  int start = -1;
  int stop = -1;
  int diff;

  if (progstart > progstop)
    return;

  if (!tsize)
    return;

  miTime Start = timeLine[0];

  for (int i = 0; i < tsize; i++) {

    diff = miTime::hourDiff(timeLine[i], Start);

    if (start == -1 && diff >= progstart)
      start = i;

    if (stop == -1 && diff >= progstop) {
      stop = i;
      break;
    }
  }

  if (start < 0)
    start = 0;
  if (stop < 0)
    stop = tsize - 1;
  setTimeInterval(start, stop);

}

void ptDiagram::getTimeInterval(int &start, int &stop)
{
  start = startidx;
  stop = stopidx;
}

void ptDiagram::getTimeInterval(miTime& start, miTime & stop)
{
  if (startidx >= 0 && startidx<timeLine.size())
    start = timeLine[startidx];
  if (stopidx >= 0 && stopidx<timeLine.size())
    stop = timeLine[stopidx];
}

void ptDiagram::getTotalTimeInterval(int &start, int &stop)
{
  start = 0;
  stop = timeLine.size() - 1;
}

void ptDiagram::getTotalTimeInterval(miTime &start, miTime &stop)
{
  if (timeLine.size() > 0) {
    start = timeLine[0];
    stop = timeLine[timeLine.size() - 1];
  }
}

// find first plotelement with specific name
// .. starting with plotelement start
PlotElement* ptDiagram::findElement(const std::string& name, PlotElement* start)
{
  if (!start)
    start = first;
  else {
    start = start->next;
    if (!start)
      return 0;
  }
  for (PlotElement* cur = start; cur; cur = cur->next)
    if (cur->Name() == name)
      return cur;

  return 0; // element not found
}

// find first plotelement with specific type
// .. starting with plotelement start
PlotElement* ptDiagram::findElement(ptPrimitiveType type, PlotElement* start)
{
  if (!start)
    start = first;
  else {
    start = start->next;
    if (!start)
      return 0;
  }
  for (PlotElement* cur = start; cur; cur = cur->next)
    if (cur->getType() == type)
      return cur;

  return 0; // element not found
}

// find first plotelement with specific type and parameter id
// .. starting with plotelement start
PlotElement* ptDiagram::findElement(ptPrimitiveType type, ParId id,
    PlotElement* start)
{
  if (!start)
    start = first;
  else {
    start = start->next;
    if (!start)
      return 0;
  }

  for (PlotElement* cur = start; cur; cur = cur->next)
    if ((id == cur->getParId()) && (cur->getType() == type || type
        == DUM_PRIMITIVE))
      return cur;

  return 0; // element not found
}

// remove one element
void ptDiagram::removeElement(PlotElement *elm)
{
  METLIBS_LOG_SCOPE();
  PlotElement *cur = first;

  //special care if first element is to be removed
  if (cur == elm) {
    first = cur->next;
    delete elm;
    nPlotElements--;
    return;
  }

  while (cur) {
    if (cur->next == elm) {
      cur->next = elm->next;
      delete elm;
      nPlotElements--;
      return;
    }
    cur = cur->next;
  }
  METLIBS_LOG_ERROR("Element not found");
}

// remove all elements of specified type
void ptDiagram::removeElement(ptPrimitiveType type)
{
  PlotElement *elm;
  while ((elm = findElement(type)))
    removeElement(elm);
}

// enable all elements of specified type
void ptDiagram::enableElement(ptPrimitiveType type)
{
  PlotElement *elm = 0;
  while ((elm = findElement(type, elm)))
    elm->enable();
}

// disable all elements of specified type
void ptDiagram::disableElement(ptPrimitiveType type)
{
  PlotElement *elm = 0;
  while ((elm = findElement(type, elm)))
    elm->disable();
}

// enable all plotelements
void ptDiagram::enableAll()
{
  for (PlotElement *elm = first; elm; elm = elm->next)
    elm->enable();
}

// disable all plotelements
void ptDiagram::disableAll()
{
  for (PlotElement *elm = first; elm; elm = elm->next)
    elm->disable();
}

// enable plotelement
void ptDiagram::enableElement(PlotElement* elm)
{
  elm->enable();
}

// disable plotelement
void ptDiagram::disableElement(PlotElement* elm)
{
  elm->disable();
}

// add a new plotelement to the list
void ptDiagram::addElement(PlotElement* elm)
{
  METLIBS_LOG_SCOPE();
  if (first == NULL)
    first = last = elm;
  else {
    last->next = elm;
    last = elm;
  }
  elm->next = NULL;
  nPlotElements++;
}

// plot diagram
void ptDiagram::plot(ptPainter& painter)
{
  METLIBS_LOG_SCOPE();
  if (!first)
    return;

  if (Style)
    painter.clear(Style->backgroundColor());
  for (PlotElement* elm = first; elm; elm = elm->next)
    elm->plot(painter);
}

void ptDiagram::tst_print()
{
  //   cout << "Printing of ptDiagram structure\nNumber of Elements: " <<
  //     nPlotElements << endl;
  //   for (PlotElement* elm=first; elm; elm=elm->next)
  //     elm->tst_print();
}

} // namespace pets2

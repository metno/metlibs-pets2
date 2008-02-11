/*
  libpets2 - presentation and editing of time series
  
  $Id$

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


// ptLayout.h: definition of Layout struct

#ifndef _layout_h
#define _layout_h

#include "ptGlobals.h"
#include <tsData/ptParameterDefinition.h>
#include "ptFontColl.h"
#include "ptColor.h"
#include "ptMarker.h"
#include "ptPatterns.h"

#include <puTools/miString.h>

#include <iostream>
#include <vector>

using namespace std;

enum ptAxis {
  LEFTLEFT,
  RIGHTRIGHT,
  LEFTRIGHT,
  RIGHTLEFT,
  NO_AXIS
};

enum ptAlign {
  CENTER,
  LEFT,
  RIGHT,
  OVER,
  BELOW
};

enum ptSize {
  ZERO    = 0,
  X_SMALL = 5,
  SMALL   = 10,
  MEDIUM  = 25,
  LARGE   = 50,
  X_LARGE = 75
};

enum ptHorSides {
  SH_NONE,
  SH_LEFT,
  SH_RIGHT,
  SH_BOTH
};

enum ptVerSides {
  SV_NONE,
  SV_TOP,
  SV_BOTTOM,
  SV_BOTH
};

enum ptEditStyle {
  ES_LINE,
  ES_HISTOGRAM,
  ES_SWITCH
};

enum ptDateStyle {
  DS_DATE,
  DS_DAY,
  DS_WEEK,
  DS_MONTH,
  DS_YEAR
};

struct Layout {
  miString name;     // name of primitive
  bool visible;      // is visible in diagram
  float height;      // height of element
  float trueheight; 
  float position;
  float spacing;     // vertical spacing between unequal elements
  float intSpacing;  // internal vert. spacing between equal elements
  ptColor color;         // colour to use
  ptColor color2;        // additional colour
  ptColor color3;        // additional colour
  vector<ptColor> colorlist; // list of colours
  ptFontSize font;       // size of font
  float lineWidth;       // width of lines
  bool label;            // plot label
  bool unit;             // plot unit on axis
  bool patternInColour;  // use line- and fill-pattern in colourmode
  ptLineStyle linePattern;          // line pattern
  vector<ptLineStyle> linepatternlist; // list of line patterns
  ptFillStyle fillstyle;            // single polygon pattern
  vector<ptFillStyle> patternlist;  // list of polygon patterns
  ptAlign align;         // alignment flag
  miString text;         // text to output
  miString text2;        // additional text
  ptAxis axis;           // which axis
  ptHorSides horsides;   // for frames etc
  ptVerSides versides;   // for frames etc

  int yaid;              // id used for yaxis and lines
  bool  axisgrid;        // plot gridlines in axis
  float gridwidth;       // linewidth for grid
  ptColor gridcolor;     // colour of grid
  ptLineStyle gridstyle; // linestyle for grid

  bool axisRectangle;    // xaxis draws a full rectangle
  bool gridxonly;        // axisRectangle: draw vertical gridlines only
  bool plotDayPattern;   // gridlines draws day patterns
  float axisWidth;       // lineWidth of axis-line
  int minSkipX;          // for printing of timelines
  int useTimes;          // use timepoints modulo useTimes
  vector<int> modhours;  // use timepoints with hours modulo xx
  float tickWidth;       // width of tickmarks
  float tickLen;         // length of tickmarks
  float minRange;        // minimum datavalue range for normal yaxis
  float interval;        // yaxis interval between small tickmarks
  float delta;           // yaxis interval between large tickmarks
  int numTickMajor;      // number of major tickmarks
  int numTickMinor;      // number of minor tickmarks
  float labelSpace;      // vertical space for axislabels
  bool horLabels;        // print axis-labels horizontally
  float horLabelOffset;  // vertical horLabel offset (in current textHeight's)
  float minMargin;       // yaxis margin between axischildren. and axis-start/stop
  float minValue;        // y-axis minimum datavalue
  float maxValue;        // y-axis maximum datavalue
  bool maxIsSet;         // maxValue is absolute
  bool minIsSet;         // minValue is absolute
  bool useMinMax;        // keep axis inside minValue and maxValue
  bool quantized;        // quantized values
  float quantum;         // quantum for the above
  bool reverse;          // reverse vector-direction etc.
  bool centerVector;     // for windarrow and vectors
  bool vectorArrow;      // for vectors (arrowhead)
  bool lineArrows;       // for line elements (arrows on line)
  float arrowLength;     // for line elements with arrows (length of arrows)
  float arrowSize;       // for line elements with arrows (size of arrowhead)
  bool fromFile;         // for text lines only
  miString pformat;      // print format Eks: %2.1f
  bool vectorF;          // Vector Table element: write angle AND size
  bool keepinaxis;       // keep axischildelements within axis
  bool labelOnLine;      // label on line-elements
  float lineLabelPos;    // label position (in percent of numTimePoints)
  bool doubleLine;       // Two connected lines, with shading between
  ptMarker marker;       // markertype
  ptFillStyle markerFill;// marker fillstyle
  float size;            // size of various entities in pixels
  bool shadow;           // draw a shadow if possible
  int histStart;         // startpoint in x for histogram (0..9)
  int histStop;          // endpoint in x for histogram (0..9)
  bool fittopage;        // try to fit object within diagramborders
  float cutoff;          // cutoff-value used by some elements
  int priority;          // priority for editing etc
  miString language;     // language ("NO" or "EN") 
  bool drawbackground;   // draw background
  ptEditStyle editstyle; // type of graphical editing
  vector<miString> textlabels; // fixed textlabels for yaxis
  bool printValueLock;   // edit: lock current printValue switch
  bool lineBar;          // line drawn as horisontal bars
  bool lineStep;         // line drawn stepwise
  bool asNumber;         // draw as number
  ptDateStyle datestyle; // DATE subtype 
  bool legendlineinside; // plot axis-legend-lines inside diagram
  vector<float> datalimits; // list of data values for switching colour, pattern etc
  bool colorbyvalue;     // use colorlist and datalimits to change colours
  bool linestylebyvalue; // use linepatternlist and datalimits to change line patterns
  bool fillbyvalue;      // use patternlist and datalimits to change fill patterns
  bool smoothing;        // use splines on lineElements
  int smoothdiv;         // number of smoothing points between points
  bool datainknots;      // data is in knots
  bool scalewidth;       // scale symbol-widths

  Layout() : 
    visible(true), height(-1), spacing(0), intSpacing(0), font(NORMAL),
    lineWidth(1.0), label(true), unit(false), linePattern(FULL),
    fillstyle(SOLID), align(CENTER), axis(LEFTLEFT), axisWidth(2.0),
    minSkipX(0), useTimes(1), tickWidth(1.0), tickLen(10),
    minRange(0), interval(1), patternInColour(false), trueheight(0),
    delta(1), minMargin(0), lineBar(false), lineStep(false),
    minValue(-1000000), maxValue(1000000), maxIsSet(false), 
    minIsSet(false), useMinMax(false), fromFile(true), position(0),
    vectorF(false), numTickMajor(5), numTickMinor(2), labelSpace(100),
    quantized(false), quantum(1.0), labelOnLine(false), lineLabelPos(50),
    doubleLine(false), axisgrid(false), gridxonly(true), gridwidth(1),
    gridstyle(DOTTED),gridcolor("GREY95"),axisRectangle(false),
    marker(NO_MARKER),size(1),markerFill(SOLID),shadow(false),
    histStart(0), histStop(10), fittopage(false), horLabels(false),
    horLabelOffset(0),cutoff(-FLT_MAX), horsides(SH_NONE),versides(SV_NONE),
    plotDayPattern(false), centerVector(true), vectorArrow(false),
    lineArrows(false), arrowLength(40), arrowSize(0.25),
    keepinaxis(false), reverse(false), priority(0), language("EN"),
    drawbackground(false), editstyle(ES_LINE), printValueLock(false),
    asNumber(false), datestyle(DS_DATE), legendlineinside(false),
    colorbyvalue(false), linestylebyvalue(false), fillbyvalue(false),
    smoothing(false), smoothdiv(10), datainknots(true), scalewidth(true)
    {}
  void print() const
    {
      cout << "Printing of layout:\n" << "\theight: " << height
	   << "\tspacing: " << spacing << "\tintSpacing: " << intSpacing
	   << "\tcolor: " << color << "\tfont: "
	   << font << "\tlineWidth: " << lineWidth << "\tlabel: " << label
	   << "\tunit: " << unit
	   << "\tlinePattern: " << linePattern << "\tfillstyle: "
	   << fillstyle << "\talign: " << align << "\ttext: " 
	   << text << /*"\taxis: " << axis <<*/ "\taxisWidth: " << axisWidth
	   << "\tcolor2: " << color2 << "\ttickWidth: " << tickWidth 
	   << "\tminRange: " << minRange      
	   << "\tinterval: " << interval << "\tdelta: " << delta
	   << "\tminMargin: " << minMargin << "\tfromFile: " << fromFile
	   << endl;
    }
};

extern ptAxis Str2Axis(miString);
extern ptAlign Str2Align(miString);
extern ptSize Str2Size(miString);
extern ptHorSides Str2HorSides(miString);
extern ptVerSides Str2VerSides(miString);
extern ptEditStyle Str2EditStyle(miString);
extern vector<miString> Str2TextLabels(miString);
extern vector<float> Str2FloatList(miString);
extern vector<int> Str2IntList(miString);
extern ptDateStyle Str2DateStyle(miString);

extern miString Axis2Str(ptAxis);
extern miString Align2Str(ptAlign);
extern miString Size2Str(ptSize);
extern miString HorSides2Str(ptHorSides);
extern miString VerSides2Str(ptVerSides);
extern miString EditStyle2Str(ptEditStyle);
extern miString TextLabels2Str(const vector<miString>&);
extern miString FloatList2Str(const vector<float>&);
extern miString IntList2Str(const vector<int>&);
extern miString DateStyle2Str(ptDateStyle);

#endif

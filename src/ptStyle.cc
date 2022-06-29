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

// ptStyle.cc : Definitions for ptStyle class

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ptStyle.h"

#include <puTools/miStringFunctions.h>

#include <iostream>
#include <fstream>
#include <map>

#define MILOGGER_CATEGORY "metlibs.pets2.Style"
#include <miLogger/miLogging.h>

using namespace miutil;
using namespace std;

namespace pets2 {

stMother Str2Mother(const std::string& buf)
{
  const std::string mother = miutil::trimmed(miutil::to_upper(buf), true, true);
  if (mother == "AXES1")
    return AXES1;
  else if (mother == "AXES2")
    return AXES2;
  else if (mother == "AXES3")
    return AXES3;
  else if (mother == "AXES4")
    return AXES4;
  else if (mother == "AXES5")
    return AXES5;
  else if (mother == "DIAGRAM")
    return DIAGRAM;
  else if (mother == "NO_MOTHER")
    return NO_MOTHER;
  else
    return NO_MOTHER;
}

std::string Mother2Str(stMother mother)
{
  if (mother == AXES1)
    return "AXES1";
  else if (mother == AXES2)
    return "AXES2";
  else if (mother == AXES3)
    return "AXES3";
  else if (mother == AXES4)
    return "AXES4";
  else if (mother == AXES5)
    return "AXES5";
  else if (mother == DIAGRAM)
    return "DIAGRAM";
  else if (mother == NO_MOTHER)
    return "NO_MOTHER";
  else
    return "NO_MOTHER";
}

Primitive::Primitive(ptPrimitiveType t)
    : type(t), order(-1), enabled(true), id(ID_UNDEF), nr(0), conv(NO_CONVERSION)
    , mother(NO_MOTHER), plotAll(false), component(0)
{
}

void Primitive::print() const
{
  cout << "\nPrinting of Primitive:\n\ttype: " << type << "\torder: " << order
      << "\talias: " << id.alias << "\tnr: " << nr << "\tconv: " << conv
      << "\tmother: " << mother << "\tplotAll: " << plotAll << "\tenabled: "
      << enabled << endl << "\t";
  layout.print();
}

ptStyle::ptStyle()
    : topMargin_(50), bottomMargin_(50), leftMargin_(100), rightMargin_(100)
    , leftOffset_(0), rightOffset_(0)
    , bgColor("BLACK")
    , nprimF(0), nprim(0)
    , nAxes(0)
    , localTime(false), timeZone(0)
{
}

ptStyle::ptStyle(const ptStyle& rhs)
{
  topMargin_ = rhs.topMargin_;
  bottomMargin_ = rhs.bottomMargin_;
  leftMargin_ = rhs.leftMargin_;
  rightMargin_ = rhs.rightMargin_;
  localTime = rhs.localTime;
  timeZone = rhs.timeZone;
  leftOffset_ = rhs.leftOffset_;
  rightOffset_ = rhs.rightOffset_;

  nprimF = rhs.nprimF;
  nprim = rhs.nprim;
  bgColor = rhs.bgColor;
  nAxes = rhs.nAxes;
  pprimF = rhs.pprimF;
  pprim = rhs.pprim;
}

ptStyle::~ptStyle()
{
}

ptStyle& ptStyle::operator=(const ptStyle& rhs)
{
  if (this == &rhs)
    return *this;

  // memberwise copy
  topMargin_ = rhs.topMargin_;
  bottomMargin_ = rhs.bottomMargin_;
  leftMargin_ = rhs.leftMargin_;
  rightMargin_ = rhs.rightMargin_;
  localTime = rhs.localTime;
  timeZone = rhs.timeZone;
  leftOffset_ = rhs.leftOffset_;
  rightOffset_ = rhs.rightOffset_;
  nprimF = rhs.nprimF;
  nprim = rhs.nprim;
  bgColor = rhs.bgColor;
  nAxes = rhs.nAxes;
  pprimF = rhs.pprimF;
  pprim = rhs.pprim;

  return *this;
}

// search for and return file primitive of corresponding type
int ptStyle::_findPrimF(const ptPrimitiveType type) const
{
  for (int i = 0; i < nprimF; i++)
    if (pprimF[i].type == type && pprimF[i].enabled)
      return i;
  return -1; // primitive not found
}

// search for and return file primitive of corresponding alias
//int ptStyle::_findPrimF(const Alias alias, const int from) const
int ptStyle::_findPrimF(const ParId id, const int from) const
{
  int i;

  if (from != -1) {// search for alias among primitives of from's type
    for (i = from; i < nprimF && pprimF[i].type == pprimF[from].type; i++)
      if (pprimF[i].id == id && pprimF[i].enabled)
        return i;

    return -1; // alias not found
  } else {// search from start of pprim array
    for (i = 0; i < nprimF; i++)
      if (pprimF[i].id == id && pprimF[i].enabled)
        return i;
    return -1; // alias not found
  }
}

void ptStyle::_enablePrimitive(const bool dataP, const int index)
{
  if (dataP) {
    pprimF[index].enabled = true;
  } else {
    pprim[index].enabled = true;
  }
}

void ptStyle::_disablePrimitive(const bool dataP, const int index)
{
  if (dataP) {
    pprimF[index].enabled = false;
  } else {
    pprim[index].enabled = false;
  }
}

void ptStyle::enablePrimitive(const bool dataP, const ptPrimitiveType type)
{
  int i;
  if (dataP) {
    for (i = 0; i < nprimF; i++)
      if (pprimF[i].type == type)
        _enablePrimitive(true, i);
  } else {
    for (i = 0; i < nprim; i++)
      if (pprim[i].type == type)
        _enablePrimitive(false, i);
  }
}

void ptStyle::disablePrimitive(const bool dataP, const ptPrimitiveType type)
{
  int i;
  if (dataP) {
    for (i = 0; i < nprimF; i++)
      if (pprimF[i].type == type)
        _disablePrimitive(true, i);
  } else {
    for (i = 0; i < nprim; i++)
      if (pprim[i].type == type)
        _disablePrimitive(false, i);
  }
}

void ptStyle::enablePrimitive(const ParId pid, const ptPrimitiveType type)
{
  for (int i = 0; i < nprimF; i++)
    if (pid == pprimF[i].id
        && (type == pprimF[i].type || type == DUM_PRIMITIVE))
      _enablePrimitive(true, i);
}

void ptStyle::disablePrimitive(const ParId pid, const ptPrimitiveType type)
{
  for (int i = 0; i < nprimF; i++)
    if (pid == pprimF[i].id
        && (type == pprimF[i].type || type == DUM_PRIMITIVE))
      _disablePrimitive(true, i);
}

void ptStyle::enablePrimitive(const bool dataP, const int index)
{
  if (dataP) {
    if (index >= 0 && index < nprimF)
      _enablePrimitive(true, index);
  } else {
    if (index >= 0 && index < nprim)
      _enablePrimitive(false, index);
  }
}

void ptStyle::disablePrimitive(const bool dataP, const int index)
{
  if (dataP) {
    if (index >= 0 && index < nprimF)
      _disablePrimitive(true, index);
  } else {
    if (index >= 0 && index < nprim)
      _disablePrimitive(false, index);
  }
}

void ptStyle::enableAllPrimitives(const bool dataP)
{
  int i;
  if (dataP) {
    for (i = 0; i < nprimF; i++)
      _enablePrimitive(true, i);
  } else {
    for (i = 0; i < nprim; i++)
      _enablePrimitive(false, i);
  }
}

void ptStyle::disableAllPrimitives(const bool dataP)
{
  int i;
  if (dataP) {
    for (i = 0; i < nprimF; i++)
      _disablePrimitive(true, i);
  } else {
    for (i = 0; i < nprim; i++)
      _disablePrimitive(false, i);
  }
}

ptAxis ptStyle::_findFirstFreeAxis(bool usedAxes[][MAXYAXIS],
    const stMother mother)
{
  int i, j;
  // first try this mother
  for (j = 0; j < MAXYAXIS; j++)
    if (!usedAxes[mother][j]) {
      usedAxes[mother][j] = true;
      METLIBS_LOG_DEBUG("aFound free axis - Mother: " << mother << " axis: " << j);
      return (ptAxis) j;
    }
  // try the previous mothers
  for (i = 0; i < mother; i++)
    for (j = 0; j < MAXYAXIS; j++)
      if (!usedAxes[i][j]) {
        usedAxes[i][j] = true;
        METLIBS_LOG_DEBUG("bFound free axis - Mother: " << i << " axis: " << j);
        return (ptAxis) j;
      }
  // try the next mothers
  for (i = mother; i < nAxes; i++)
    for (j = 0; j < MAXYAXIS; j++)
      if (!usedAxes[i][j]) {
        usedAxes[i][j] = true;
        METLIBS_LOG_DEBUG("cFound free axis - Mother: " << i << " axis: " << j);
        return (ptAxis) j;
      }
  METLIBS_LOG_DEBUG("No free axis");
  return NO_AXIS; // no free axis exists
}

void ptStyle::_sortArray(Out*** array, const int& nelem,
    const stMother& maxMother, const int* motherCount)
{
  //   stMother curMother;
  int curMother;
  int i, j;
  Out *tmp;

  // first, make sure that all DIAGRAM-mothers appears first in array,
  for (i = 0; i < motherCount[DIAGRAM]; i++)
    if ((*array[i])->mother != DIAGRAM) {
      for (j = i + 1; j < nelem; j++)
        // find first DIAGRAM-mother in rest of list
        if ((*array[j])->mother == DIAGRAM)
          break;
      // switch the elements
      tmp = *array[i];
      *array[i] = *array[j];
      *array[j] = tmp;
    }
  // force the AXES1-elements after the DIAGRAM-elements, then AXES2, AXES3...
  int index = motherCount[DIAGRAM];
  for (curMother = AXES1; curMother <= maxMother; index
      += motherCount[curMother++])
    for (i = 0; i < motherCount[curMother]; i++)
      if ((*array[index + i])->mother != curMother) {
        for (j = index + i + 1; j < nelem; j++)
          if ((*array[j])->mother == curMother)
            break;
        tmp = *array[index + i];
        *array[index + i] = *array[j];
        *array[j] = tmp;
      }

  // sort elements within each AXES by z-order and order
  index = motherCount[DIAGRAM];
  for (curMother = AXES1; curMother <= maxMother; index
      += motherCount[curMother++]) {
    // sort by zorder
    for (i = motherCount[curMother] - 1 + index; i >= index; i--)
      for (j = index; j < i; j++)
        if ((*array[j])->zorder > (*array[j + 1])->zorder) {
          tmp = *array[j];
          *array[j] = *array[j + 1];
          *array[j + 1] = tmp;
        }
    // sort by order
    for (i = motherCount[curMother] - 1 + index; i >= index; i--)
      for (j = index; j < i; j++)
        if ((*array[j])->order > (*array[j + 1])->order) {
          tmp = *array[j];
          *array[j] = *array[j + 1];
          *array[j + 1] = tmp;
        }

  }

  // sort the DIAGRAM-mother-elements of the array by y-order (bubble sort)
  for (i = motherCount[DIAGRAM] - 1; i >= 0; i--) {
    for (j = 0; j < i; j++) {
      if ((*array[j])->order > (*array[j + 1])->order) {
        tmp = *array[j];
        *array[j] = *array[j + 1];
        *array[j + 1] = tmp;
      }
    }
  }
}

void ptStyle::print() const
{
  int i;
  cout << "****************************************************************\n"
      << "* PRINTING OF STYLE \n"
      << "****************************************************************\n"
      << "\ttopMargin: " << topMargin_ << "\tbottomMargin: " << bottomMargin_
      << "\tleftMargin: " << leftMargin_ << "\trightMargin: " << rightMargin_
      << "\tbgColor: " << bgColor << "\tnAxes: " << nAxes << "\n" << endl;
  //        << "\tnfont: " << nfont << endl;
  //   for (int i=0;i<nfont;i++)
  //     cout << fontList[i] << "\n";
  cout << "******************************************\n"
      << "* PRINTING OF FILE PRIMITIVES\n"
      << "******************************************\n"
      << "Number of primitives: " << nprimF << endl;
  for (i = 0; i < nprimF; pprimF[i++].print())
    ;
  cout << "******************************************\n"
      << "* PRINTING OF OTHER PRIMITIVES\n"
      << "******************************************\n"
      << "Number of primitives: " << nprim << endl;
  for (i = 0; i < nprim; pprim[i++].print())
    ;
  cout << endl;
}

// ptStyle::organize sets layout, calculates location etc. for the primitives
// specified by primListIn and aliasListIn
// In addition, location and layout are set for the primitives not found on
// file.  All primitives and aliases are stored in aliasListOut/primListOut
// Return value is list of vertical location for the primitives.
// NULL is returned on error
// assumes that sufficient space is allocated for the out-arrays
bool ptStyle::organize(StyleOrder *orderList, int *pnout, StyleOrder *inList,
    int nin)
{
  METLIBS_LOG_SCOPE();
  int iprim, iprimtype, iline, nFound;
  int nout = 0;
  bool usedAxes[MAXAXES][MAXYAXIS];
  stMother maxMother = AXES1;
  Primitive onePrim;
  int axesIndex = 0;
  pets2::Layout oneLayout;
  int unspecLines[MAXUNSPECIFIED_LINES]; // index to inList for unspecified lines
  int nunspecLines = 0;
  int motherCount[6] =
    { 0, 0, 0, 0, 0, 0 };
  int i, j;
  Out **outList[MAXPRIMF + MAXPRIM];
  Out *pAxes[MAXAXES];

  METLIBS_LOG_DEBUG("Number of primitives in:" << nin);
  //initialize usedAxes
  for (i = 0; i < MAXAXES; i++)
    for (j = 0; j < MAXYAXIS; j++)
      usedAxes[i][j] = false;

  //let iline index first line primitive (from style)
  iline = _findPrimF(LINE);

  // loop through all elements of inList and set layout
  nin--; // OBS OBS OBS
  for (i = 0; i < nin; i++) {
    METLIBS_LOG_DEBUG("Inside for all input elements loop no " << i);
    METLIBS_LOG_DEBUG("ParId: " << inList[i].id << " Type: " << inList[i].type);
    // find all primitives with this ParId
    nFound = 0;
    for (iprim = 0; iprim < nprimF; iprim++) {
      if (pprimF[iprim].id == inList[i].id && pprimF[iprim].enabled) {
        // found primitive
        METLIBS_LOG_DEBUG("Found primitive. ParId:" << pprimF[iprim].id << " Type:" << pprimF[iprim].type);
        nFound++;

        METLIBS_LOG_DEBUG("Adding to outList (SPECIFIC)");
        //  	if (pprimF[iprim].type == LINE ||
        // 	    pprimF[iprim].type == EDITLINE) {
        // 	  usedAxes[pprimF[iprim].mother][pprimF[iprim].layout.axis] = true;
        // 	}
        if (pprimF[iprim].type == YAXIS || pprimF[iprim].type == YAXIS_STATIC) {
          usedAxes[pprimF[iprim].mother][pprimF[iprim].layout.axis] = true;
        }

        outList[nout] = new Out*;
        *outList[nout] = new Out;
        (*outList[nout])->prim.layout = pprimF[iprim].layout;
        (*outList[nout])->mother = pprimF[iprim].mother;
        motherCount[(*outList[nout])->mother]++;

        if ((*outList[nout])->mother != DIAGRAM && maxMother
            < (*outList[nout])->mother)
          maxMother = (*outList[nout])->mother;

        (*outList[nout])->order = pprimF[iprim].order;
        (*outList[nout])->zorder = zOrder(pprimF[iprim].type);
        (*outList[nout])->prim.type = pprimF[iprim].type;
        (*outList[nout])->prim.dataIndex = inList[i].dataIndex;
        (*outList[nout])->prim.dataComp = pprimF[iprim].component;
        (*outList[nout++])->prim.id = inList[i].id;
      } // primitive with correct parid found
    } // for all file primitives
    if (!nFound) { // no primitives for this ParId
      METLIBS_LOG_DEBUG("No primitives found for current ParId");
      // is this type mentioned in style?
      if ((iprimtype = _findPrimF(inList[i].type)) != -1) {
        // yes, it is
        METLIBS_LOG_DEBUG("..but we found another primitive with same type");
        if (pprimF[iprimtype].plotAll) {
          // style says: plot all data with this type
          METLIBS_LOG_DEBUG("..and it says PlotAll");
          if (inList[i].type == LINE) {
            METLIBS_LOG_DEBUG("It is a line..do nothing yet");
            unspecLines[nunspecLines++] = i;
          } else {
            METLIBS_LOG_DEBUG("Adding to outList (GENERIC)");
            outList[nout] = new Out*;
            (*outList[nout]) = new Out;
            //use default lout:
            (*outList[nout])->prim.layout = pprimF[iprimtype].layout;
            (*outList[nout])->mother = pprimF[iprimtype].mother;
            motherCount[(*outList[nout])->mother]++;
            (*outList[nout])->order = pprimF[iprimtype].order;
            (*outList[nout])->zorder = zOrder(inList[i].type);
            (*outList[nout])->prim.type = inList[i].type;
            (*outList[nout])->prim.dataIndex = inList[i].dataIndex;
            (*outList[nout])->prim.dataComp = inList[i].dataComp;
            (*outList[nout++])->prim.id = inList[i].id;
          }
        }
      } // if type found

    } // if nFound>0

  } // for all input elements


  // decide axes for the unspecified lineprimitives
  if (iline != -1) {
    oneLayout = pprimF[iline].layout;
    for (i = 0; i < nunspecLines; i++) {
      METLIBS_LOG_DEBUG("Inside loop for unspecified line primitives");
      if ((oneLayout.axis = _findFirstFreeAxis(usedAxes, maxMother)) != NO_AXIS) {
        outList[nout] = new Out*;
        (*outList[nout]) = new Out;
        (*outList[nout])->prim.layout = oneLayout;
        (*outList[nout])->mother = maxMother;
        motherCount[(*outList[nout])->mother]++;
        (*outList[nout])->prim.type = LINE;
        (*outList[nout])->zorder = zOrder(LINE);
        (*outList[nout])->prim.dataIndex = inList[unspecLines[i]].dataIndex;
        (*outList[nout])->prim.dataComp = inList[unspecLines[i]].dataComp;
        (*outList[nout++])->prim.id = inList[unspecLines[i]].id;
      } else
        break; // no free axis, break out of loop
      if (METLIBS_LOG_DEBUG_ENABLED()) {
        for (int k=0;k<MAXAXES;k++) {
          for (int l=0;l<MAXYAXIS;l++)
            METLIBS_LOG_DEBUG("usedAxes[" << k << "][" << l << "] = " << usedAxes[k][l]);
        }
      }
    }

    METLIBS_LOG_DEBUG("Finished for unspecified line primitives. nout is " << nout);
  }

  // OBS dette skal flyttes inn i uspesifiserte linjeprimitiver


  // check if we lack any Y-axes in non-fileprimitives
  //   pets2::Layout curl;
  //   Primitive curp;
  //   bool found;
  //   curp.type = YAXIS;
  //   curp.enabled=true;
  //   for (i=0;i<MAXAXES;i++)
  //     for (j=0;j<MAXYAXIS;j++)
  //       if (usedAxes[i][j]) { // check if this y-axis exists
  // 	found = false;
  // 	for (k=0; k<nprim; k++) {
  // 	  if (pprim[k]->type == YAXIS)
  // 	    found = (pprim[k]->enabled &&
  // 		     pprim[k]->mother == stMother(i) &&
  // 		     pprim[k]->layout.axis == ptAxis(j));
  // 	  if (found) break;
  // 	}
  // 	if (!found) {
  // 	  curl.axis = ptAxis(j);
  // 	  //curl.yaid = ;
  // 	  curp.layout = curl;
  // 	  curp.mother = stMother(i);
  // 	  pprim[nprim++] = new Primitive(curp);
  // 	}
  //       }


  // go through the list of non-file-primitives and put primitives on list
  for (i = 0; i < nprim; i++) {
    if (pprim[i].enabled) {
      METLIBS_LOG_DEBUG("Inside for all non-file-primitives loop no " << i);
      outList[nout] = new Out*;
      (*outList[nout]) = new Out;
      (*outList[nout])->prim.layout = pprim[i].layout;
      (*outList[nout])->mother = pprim[i].mother;
      motherCount[(*outList[nout])->mother]++;
      (*outList[nout])->order = pprim[i].order;
      (*outList[nout])->zorder = zOrder(pprim[i].type);
      (*outList[nout])->prim.type = pprim[i].type;
      (*outList[nout])->prim.dataIndex = 0;
      (*outList[nout])->prim.dataComp = 0;
      (*outList[nout++])->prim.id = ID_UNDEF;
      if (pprim[i].type == XAXIS) { // get reference to axis-primitive
        pAxes[axesIndex++] = *outList[nout - 1];
      }
    }
  }

  // sort the list on output primitives with respect to mother and order
  // first comes the DIAGRAM-mother-elements, then AXES1, AXES2 etc..
  if (METLIBS_LOG_DEBUG_ENABLED()) {
    METLIBS_LOG_DEBUG("outList before sorting:");
    for (i = 0; i < nout; i++)
      METLIBS_LOG_DEBUG("type: " << (*outList[i])->prim.type << " mother: " << (*outList[i])->mother << " order: " << (*outList[i])->order
                                 << " zorder: " << (*outList[i])->zorder << " height: " << (*outList[i])->prim.layout.height);
    METLIBS_LOG_DEBUG("MotherCount: DIAGRAM: " << motherCount[DIAGRAM] << " AXES1: " << motherCount[AXES1] << " AXES2: " << motherCount[AXES2] << " AXES3: "
                                               << motherCount[AXES3] << " AXES4: " << motherCount[AXES4] << " NO_MOTHER: " << motherCount[NO_MOTHER]);
    METLIBS_LOG_DEBUG("maxMother : " << maxMother);
  }
  _sortArray(outList, nout, maxMother, motherCount);
  if (METLIBS_LOG_DEBUG_ENABLED()) {
    METLIBS_LOG_DEBUG("outList after sorting:");
    for (i = 0; i < nout; i++)
      METLIBS_LOG_DEBUG("type: " << (*outList[i])->prim.type << " mother: " << (*outList[i])->mother << " order: " << (*outList[i])->order
                                 << " zorder: " << (*outList[i])->zorder << " height: " << (*outList[i])->prim.layout.height);
  }

  if (METLIBS_LOG_DEBUG_ENABLED()) {
    METLIBS_LOG_DEBUG("pAxes:");
    for (i=0;i<axesIndex;i++)
      METLIBS_LOG_DEBUG("type: " << pAxes[i]->prim.type << " height: " << pAxes[i]->prim.layout.height);
  }
  if (axesIndex < maxMother + 1) {
    METLIBS_LOG_ERROR("too few XAXIS defined:" << axesIndex << " asked for at least:" << maxMother + 1);
    return false;
  }

  // now, step through the sorted list of output primitives and set vertical
  // position on screen
  // We assume that maximum one height specification is missing
  float cury = globalWindow.y1 + bottomMargin_;
  float cury2 = globalWindow.y2 - topMargin_;
  int index = 0, index2 = motherCount[DIAGRAM] - 1;
  while (index < nout && (*outList[index])->mother == DIAGRAM) {
    METLIBS_LOG_DEBUG("Inside loop for vertical positions " << index);
    if (index > 0) {
      cury
          += ((*outList[index])->prim.type == (*outList[index - 1])->prim.type) ? (*outList[index
              - 1])->prim.layout.intSpacing
              : (*outList[index - 1])->prim.layout.spacing;
    }
    if ((*outList[index])->prim.layout.height >= 0) {
      (*outList[index])->prim.field.y1 = cury;
      cury += (*outList[index])->prim.layout.height;
      (*outList[index++])->prim.field.y2 = cury;
      if (cury > cury2) // coordinate sum exceeds global range, issue warning
        METLIBS_LOG_WARN("sum of specified heights on style file exceeds "
                         << "global range. \nPlot may look funny");
    } else { // else block will be executed maximum one time
      // primitive[index] misses height specification.
      // Set all other heights first
      while (index2 > index) {
        if (index2 < motherCount[DIAGRAM] - 1) {
          cury2
              -= ((*outList[index2])->prim.type
                  == (*outList[index2 + 1])->prim.type) ? (*outList[index2])->prim.layout.intSpacing
                  : (*outList[index2])->prim.layout.spacing;
        }
        (*outList[index2])->prim.field.y2 = cury2;
        cury2 -= (*outList[index2])->prim.layout.height;
        (*outList[index2--])->prim.field.y1 = cury2;
      }
      if (index2 < motherCount[DIAGRAM] - 1) {
        cury2
            -= ((*outList[index2])->prim.type
                == (*outList[index2 + 1])->prim.type) ? (*outList[index2])->prim.layout.intSpacing
                : (*outList[index2])->prim.layout.spacing;
      }
      // now, set primitive[index] coordinates
      (*outList[index])->prim.field.y1 = cury;
      (*outList[index])->prim.field.y2 = cury2;
      if (cury2 < cury)
        // specified coordinate-sum exceeds global range, issue warning
        METLIBS_LOG_WARN("sum of specified heights on style file exceeds "
                         << "global range. \nPlot may look funny");
      break; // break out of while loop
    }
  } // while
  METLIBS_LOG_DEBUG("Finished loop for vertical positions");

  // set coordinates for the inside-axes-primitives
  index = motherCount[DIAGRAM];
  METLIBS_LOG_DEBUG("MaxMother before inside-axes-primitives:" << maxMother);
  for (i = 0; i <= maxMother; index += motherCount[i++])
    for (j = 0; j < motherCount[i]; j++) {
      METLIBS_LOG_DEBUG("Inside loop, set coordinates for axeprims " << i << " " << j);
      (*outList[index + j])->prim.field.y1 = pAxes[i]->prim.field.y1
          + pAxes[i]->prim.layout.tickLen;
      (*outList[index + j])->prim.field.y2 = pAxes[i]->prim.field.y2;
    }

  if (METLIBS_LOG_DEBUG_ENABLED()) {
    METLIBS_LOG_DEBUG("outList with specified vertical positions:");
    for (i = 0; i < nout; i++)
      METLIBS_LOG_DEBUG("type: " << (*outList[i])->prim.type << " mother: " << (*outList[i])->mother << " order: " << (*outList[i])->order << " zorder: "
                                 << (*outList[i])->zorder << " height: " << (*outList[i])->prim.layout.height << " y1: " << (*outList[i])->prim.field.y1
                                 << " y2: " << (*outList[i])->prim.field.y2 << " text: " << (*outList[i])->prim.layout.text);
  }

  // set the elements of the return array
  for (i = 0; i < nout; i++) {
    METLIBS_LOG_DEBUG("Inside loop, set output list " << i);
    orderList[i].layout = (*outList[i])->prim.layout;
    orderList[i].id = (*outList[i])->prim.id;
    orderList[i].type = (*outList[i])->prim.type;
    orderList[i].field = (*outList[i])->prim.field;
    orderList[i].dataIndex = (*outList[i])->prim.dataIndex;
    orderList[i].dataComp = (*outList[i])->prim.dataComp;
  }

  *pnout = nout; // set number of output primitives:

  // delete allocated space
  for (i = 0; i < nout; delete (*outList[i]), delete outList[i++])
    ;

  return true;
}

bool ptStyle::readStyle(const std::string& filename, bool verbose)
{
  METLIBS_LOG_SCOPE();
  ParameterDefinition parDef;
  Primitive onep, curp;
  pets2::Layout onel, curl;
  map<std::string, std::string> userkeys;

  if (verbose)
    METLIBS_LOG_INFO("Reading stylefile '" << filename << "'");
  else
    METLIBS_LOG_DEBUG("Reading stylefile '" << filename << "'");
  ifstream sfile(filename);
  if (!sfile) {
    METLIBS_LOG_ERROR("Cannot open file '" << filename << "'");
    return false;
  }

  localTime = false;
  timeZone = 0;

  onep.type = DUM_PRIMITIVE;
  onep.id = ID_UNDEF;
  onep.mother = DIAGRAM;
  onep.plotAll = false;
  onep.order = 0;

  //default layout
  onel.name = "";
  onel.height = 0;
  onel.trueheight = 0;
  onel.position = 0;
  onel.color = ptColor("RED");
  onel.font = NORMAL;
  onel.lineWidth = 1.0;
  onel.label = true;
  onel.patternInColour = false;
  onel.linePattern = FULL;
  onel.fillstyle = SOLID;
  onel.size = 1;
  onel.marker = NO_MARKER;
  onel.align = LEFT;
  onel.spacing = MEDIUM;
  onel.intSpacing = SMALL;
  onel.axisRectangle = false;
  onel.axisWidth = 2.0;
  onel.tickWidth = 1.0;
  onel.tickLen = 1.0;
  onel.minRange = 0;
  onel.interval = 0.1;
  onel.delta = 2;
  onel.minMargin = 0;
  onel.maxIsSet = false;
  onel.minIsSet = false;
  onel.minValue = FLT_MAX;
  onel.maxValue = -FLT_MAX;
  onel.useMinMax = false;
  onel.yaid = 0;
  onel.reverse = false;
  onel.centerVector = false;
  onel.vectorArrow = false;
  onel.lineArrows = false;
  onel.numTickMajor = 15;
  onel.numTickMinor = 2;
  onel.labelSpace = 150;
  onel.quantized = false;
  onel.quantum = 1.0;
  onel.keepinaxis = false;
  onel.priority = 0;

  onel.gridxonly = true;
  onel.plotDayPattern = false;
  onel.axisgrid = false;
  onel.gridwidth = 1;
  onel.gridcolor = ptColor("GREY25");
  onel.gridstyle = DOTTED;

  onel.shadow = false;
  onel.histStart = 0;
  onel.histStop = 10;
  onel.fittopage = false;
  onel.horLabels = false;
  onel.horLabelOffset = 0;
  onel.horsides = SH_NONE;
  onel.versides = SV_NONE;
  onel.editstyle = ES_LINE;

  // background color
  bgColor = ptColor("GREY85");

  // margins
  topMargin_ = bottomMargin_ = 50;
  leftMargin_ = rightMargin_ = 170;

  while (sfile.good()) {
    std::string buf;
    getline(sfile, buf);
    miutil::trim(buf);
    // discard empty lines and comments
    if (!buf.length() || buf[0] == '#')
      continue;
    // check for commands
    if (buf[0] == '[') {
      if (miutil::contains(buf, "ADD")) {
        // add a new primitive from curl and curp
        curp.layout = curl;
        if (filePrimitive(curp.type)) {
          pprimF.push_back(curp);
          nprimF++;
        } else {
          pprim.push_back(curp);
          nprim++;
        }
        METLIBS_LOG_DEBUG("adding primitive "
                          << "Type:" << curp.type << "order:" << curp.order << "enabled:" << curp.enabled << "id:" << curp.id << "Mother:" << curp.mother
                          << "Component:" << curp.component
                          //<< "Layout:\n" << curp.layout
        );
      } else if (miutil::contains(buf, "DEFAULT")) {
        // set new default
        onel = curl;
        onep = curp;
      } else if (miutil::contains(buf, "NEW")) {
        // copy default to current
        curl = onel;
        curp = onep;
      }
      continue;
    }
    // split into keyword and argument
    const vector<std::string> parts = miutil::split(buf, "=", true);
    if (parts.size() < 2)
      continue;
    const std::string keyw = miutil::to_lower(parts[0]);

    const int ieq = buf.find_first_of("=");
    std::string argu = miutil::trimmed(buf.substr(ieq + 1, buf.length() - ieq - 1));
    // substitute user-defined variables
    map<std::string, std::string>::iterator itr = userkeys.begin();
    for (; itr != userkeys.end(); itr++)
      if (miutil::contains(argu, itr->first))
        miutil::replace(argu, itr->first, itr->second);
    const bool argu_true = (miutil::to_upper(argu) == "TRUE");

    // global values
    if (keyw == "bgcolor")
      bgColor.fromStr(argu);
    else if (keyw == "topmargin")
      topMargin_ = atof(argu.c_str());
    else if (keyw == "bottommargin")
      bottomMargin_ = atof(argu.c_str());
    else if (keyw == "leftmargin")
      leftMargin_ = atof(argu.c_str());
    else if (keyw == "rightmargin")
      rightMargin_ = atof(argu.c_str());
    else if (keyw == "localtime")
      localTime = argu_true;
    else if (keyw == "timezone")
      timeZone = atoi(argu.c_str());
    else if (keyw == "leftoffset")
      leftOffset_ = atof(argu.c_str());
    else if (keyw == "rightoffset")
      rightOffset_ = atof(argu.c_str());
    // primitive-attributes
    else if (keyw == "type")
      curp.type = Str2Primitive(argu);
    else if (keyw == "order")
      curp.order = atoi(argu.c_str());
    else if (keyw == "enabled")
      curp.enabled = argu_true;
    else if (keyw == "mother")
      curp.mother = pets2::Str2Mother(argu);
    else if (keyw == "component")
      curp.component = atoi(argu.c_str());
    else if (keyw == "parameter")
      curp.id = parDef.Str2ParId(argu);
    else if (keyw == "alias")
      curp.id.alias = argu;
    else if (keyw == "level")
      curp.id.level = atoi(argu.c_str());
    else if (keyw == "model")
      curp.id.model = argu;
    else if (keyw == "run")
      curp.id.run = atoi(argu.c_str());
    else if (keyw == "submodel")
      curp.id.submodel = argu;
    else if (keyw == "plotall")
      curp.plotAll = argu_true;
    // layout-attributes
    else if (keyw == "name")
      curl.name = argu;
    else if (keyw == "visible")
      curl.visible = argu_true;
    else if (keyw == "color")
      curl.color.fromStr(argu);
    else if (keyw == "color2")
      curl.color2.fromStr(argu);
    else if (keyw == "color3")
      curl.color3.fromStr(argu);
    else if (keyw == "colorlist")
      curl.colorlist = ptColor::Str2Colorlist(argu);
    else if (keyw == "font")
      curl.font = Str2FontSize(argu);
    else if (keyw == "height")
      curl.height = atoi(argu.c_str());
    else if (keyw == "trueheight")
      curl.trueheight = atoi(argu.c_str());
    else if (keyw == "position")
      curl.position = atoi(argu.c_str());
    else if (keyw == "size")
      curl.size = atoi(argu.c_str());
    else if (keyw == "marker")
      curl.marker = Str2Marker(argu);
    else if (keyw == "markerfill")
      curl.markerFill = Str2FillStyle(argu);
    else if (keyw == "rectangle")
      curl.axisRectangle = argu_true;
    else if (keyw == "linepattern")
      curl.linePattern = Str2LineStyle(argu);
    else if (keyw == "fillpattern")
      curl.fillstyle = Str2FillStyle(argu);
    else if (keyw == "patternlist")
      curl.patternlist = Str2FillStyleList(argu);
    else if (keyw == "linepatternlist")
      curl.linepatternlist = Str2LineStyleList(argu);
    else if (keyw == "align")
      curl.align = Str2Align(argu);
    else if (keyw == "spacing")
      curl.spacing = Str2Size(argu);
    else if (keyw == "intspacing")
      curl.intSpacing = Str2Size(argu);
    else if (keyw == "label")
      curl.label = argu_true;
    else if (keyw == "patternincolour")
      curl.patternInColour = argu_true;
    else if (keyw == "linewidth")
      curl.lineWidth = atof(argu.c_str());
    else if (keyw == "axiswidth")
      curl.axisWidth = atof(argu.c_str());
    else if (keyw == "tickwidth")
      curl.tickWidth = atof(argu.c_str());
    else if (keyw == "ticklen")
      curl.tickLen = atof(argu.c_str());
    else if (keyw == "minrange")
      curl.minRange = atof(argu.c_str());
    else if (keyw == "interval")
      curl.interval = atof(argu.c_str());
    else if (keyw == "delta")
      curl.delta = atof(argu.c_str());
    else if (keyw == "minmargin")
      curl.minMargin = atof(argu.c_str());
    else if (keyw == "maxisset")
      curl.maxIsSet = argu_true;
    else if (keyw == "minisset")
      curl.minIsSet = argu_true;
    else if (keyw == "minvalue")
      curl.minValue = atof(argu.c_str());
    else if (keyw == "maxvalue")
      curl.maxValue = atof(argu.c_str());
    else if (keyw == "useminmax")
      curl.useMinMax = argu_true;
    else if (keyw == "yaid")
      curl.yaid = atoi(argu.c_str());
    else if (keyw == "reverse")
      curl.reverse = argu_true;
    else if (keyw == "centervector")
      curl.centerVector = argu_true;
    else if (keyw == "vectorarrow")
      curl.vectorArrow = argu_true;
    else if (keyw == "linearrows")
      curl.lineArrows = argu_true;
    else if (keyw == "arrowlength")
      curl.arrowLength = atof(argu.c_str());
    else if (keyw == "arrowsize")
      curl.arrowSize = atof(argu.c_str());
    else if (keyw == "numtickmajor")
      curl.numTickMajor = atoi(argu.c_str());
    else if (keyw == "numtickminor")
      curl.numTickMinor = atoi(argu.c_str());
    else if (keyw == "labelspace")
      curl.labelSpace = atof(argu.c_str());
    else if (keyw == "quantized")
      curl.quantized = argu_true;
    else if (keyw == "quantum")
      curl.quantum = atof(argu.c_str());
    else if (keyw == "gridxonly")
      curl.gridxonly = argu_true;
    else if (keyw == "plotdaypattern")
      curl.plotDayPattern = argu_true;
    else if (keyw == "axisgrid")
      curl.axisgrid = argu_true;
    else if (keyw == "gridwidth")
      curl.gridwidth = atof(argu.c_str());
    else if (keyw == "gridcolor")
      curl.gridcolor.fromStr(argu);
    else if (keyw == "gridstyle")
      curl.gridstyle = Str2LineStyle(argu);
    else if (keyw == "minskipx")
      curl.minSkipX = atoi(argu.c_str());
    else if (keyw == "usetimes")
      curl.useTimes = atoi(argu.c_str());
    else if (keyw == "modhours")
      curl.modhours = Str2IntList(argu.c_str());
    else if (keyw == "axis")
      curl.axis = Str2Axis(argu);
    else if (keyw == "text")
      curl.text = argu;
    else if (keyw == "text2")
      curl.text2 = argu;
    else if (keyw == "unit")
      curl.unit = argu_true;
    else if (keyw == "pformat")
      curl.pformat = argu;
    else if (keyw == "vectorf")
      curl.vectorF = argu_true;
    else if (keyw == "linelabelpos")
      curl.lineLabelPos = atof(argu.c_str());
    else if (keyw == "labelonline")
      curl.labelOnLine = argu_true;
    else if (keyw == "keepinaxis")
      curl.keepinaxis = argu_true;
    else if (keyw == "shadow")
      curl.shadow = argu_true;
    else if (keyw == "histogramstart")
      curl.histStart = atoi(argu.c_str());
    else if (keyw == "histogramstop")
      curl.histStop = atoi(argu.c_str());
    else if (keyw == "fittopage")
      curl.fittopage = argu_true;
    else if (keyw == "horlabels")
      curl.horLabels = argu_true;
    else if (keyw == "horlabeloffset")
      curl.horLabelOffset = atof(argu.c_str());
    else if (keyw == "cutoff")
      curl.cutoff = atof(argu.c_str());
    else if (keyw == "horsides")
      curl.horsides = Str2HorSides(argu);
    else if (keyw == "versides")
      curl.versides = Str2VerSides(argu);
    else if (keyw == "priority")
      curl.priority = atoi(argu.c_str());
    else if (keyw == "language")
      curl.language = miutil::to_upper(argu);
    else if (keyw == "drawbackground")
      curl.drawbackground = argu_true;
    else if (keyw == "editstyle")
      curl.editstyle = Str2EditStyle(argu);
    else if (keyw == "textlabels")
      curl.textlabels = Str2TextLabels(argu);
    else if (keyw == "valuetextlabels")
      curl.valuetextlabels = Str2TextLabels(argu);
    else if (keyw == "printvaluelock")
      curl.printValueLock = argu_true;
    else if (keyw == "linebars")
      curl.lineBar = argu_true;
    else if (keyw == "linestep")
      curl.lineStep = argu_true;
    else if (keyw == "asnumber")
      curl.asNumber = argu_true;
    else if (keyw == "datestyle")
      curl.datestyle = Str2DateStyle(argu);
    else if (keyw == "legendlinesinside")
      curl.legendlineinside = argu_true;
    else if (keyw == "datalimits")
      curl.datalimits = Str2FloatList(argu);
    else if (keyw == "colorbyvalue")
      curl.colorbyvalue = argu_true;
    else if (keyw == "linestylebyvalue")
      curl.linestylebyvalue = argu_true;
    else if (keyw == "fillbyvalue")
      curl.fillbyvalue = argu_true;
    else if (keyw == "smoothing")
      curl.smoothing = argu_true;
    else if (keyw == "smoothdiv")
      curl.smoothdiv = atoi(argu.c_str());
    else if (keyw == "datainknots")
      curl.datainknots = argu_true;
    else if (keyw == "scalewidth")
      curl.scalewidth = argu_true;
    else if (keyw == "wrapdegrees")
      curl.wrapdegrees = argu_true;
    else if (keyw == "wraplimit")
      curl.wraplimit = atof(argu.c_str());

    else if (miutil::contains(keyw, "$"))
      userkeys[keyw] = argu;
    else
      METLIBS_LOG_ERROR("Unknown keyword '" << keyw << "'");
  }

  sfile.close();
  return true;
}

bool ptStyle::addPrimitive(const Primitive& p)
{
  if (filePrimitive(p.type)) {
    pprimF.push_back(p);
    nprimF++;
  } else {
    pprim.push_back(p);
    nprim++;
  }

  return true;
}

void ptStyle::clear()
{
  nprimF = 0;
  nprim = 0;
  pprimF.clear();
  pprim.clear();
  nAxes = 0;
}

} // namespace pets2

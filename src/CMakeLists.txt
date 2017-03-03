
SET(CMAKE_AUTOMOC TRUE)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${PC_METLIBS_INCLUDE_DIRS})
ADD_DEFINITIONS(-DQT_NO_KEYWORDS -W -Wall ${PC_METLIBS_CFLAGS_OTHER})
LINK_DIRECTORIES(${PC_METLIBS_LIBRARY_DIRS})

########################################################################

SET(pets2_SOURCES
  ptPainter.cc
  ptQPainter.cc
  ptAxesElement.cc
  ptAxisHistogramElement.cc
  ptBoxElement.cc
  ptCloudElement.cc
  ptColor.cc
  ptDateElement.cc
  ptDayElement.cc
  ptDiagram.cc
  ptDoubleLineElement.cc
  ptEditLineElement.cc
  ptFontDefs.cc
  ptGridlineElement.cc
  ptHistogramElement.cc
  ptImagePlot.cc
  ptIntervalElement.cc
  ptLayout.cc
  ptLineElement.cc
  ptLogoElement.cc
  ptMarker.cc
  ptMultiLineElement.cc
  ptPatterns.cc
  ptPlotElement.cc
  ptProgElement.cc
  ptQBoxElement.cc
  ptStyle.cc
  ptSymbolElement.cc
  ptTableElement.cc
  ptTextElement.cc
  ptTimemarkerElement.cc
  ptUTCElement.cc
  ptVectorElement.cc
  ptWindVectorElement.cc
  ptYaxisElement.cc
)
METNO_HEADERS (pets2_HEADERS pets2_SOURCES ".cc" ".h")

LIST(APPEND pets2_SOURCES
  ptAxisChildElement.cc
  ptStaticYaxisElement.cc
)

LIST(APPEND pets2_HEADERS
  ptEditModes.h
  ptGlobals.h
  ptStyleOrder.h
  ptXAxisInfo.h
  pets2Version.h
)

########################################################################

ADD_LIBRARY(pets2 SHARED
  ${pets2_SOURCES}
)

SET_TARGET_PROPERTIES(pets2 PROPERTIES
  VERSION ${PETS2_PVERSION_FULL}
  SOVERSION "${PETS2_VERSION_MAJOR}"
  OUTPUT_NAME "${lib_name}"
)

TARGET_LINK_LIBRARIES(pets2
  ${QT_LIBRARIES}
  ${PC_METLIBS_LIBRARIES}
)

INSTALL(TARGETS pets2
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

INSTALL(FILES ${pets2_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/metlibs/pets2
)

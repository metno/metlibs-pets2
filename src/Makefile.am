# $Id$

pkgname = pets2

lib_LTLIBRARIES = libpets2.la

cc_sources= \
        ptPainter.cc ptQPainter.cc \
	ptAxesElement.cc \
	ptAxisHistogramElement.cc \
	ptBoxElement.cc \
	ptCloudElement.cc \
	ptColor.cc \
	ptDateElement.cc \
	ptDayElement.cc \
	ptDiagram.cc \
	ptDoubleLineElement.cc \
	ptEditLineElement.cc \
	ptFontDefs.cc \
	ptGridlineElement.cc \
	ptHistogramElement.cc \
	ptImagePlot.cc \
	ptIntervalElement.cc \
	ptLayout.cc \
	ptLineElement.cc \
	ptLogoElement.cc \
	ptMarker.cc \
	ptMultiLineElement.cc \
	ptPatterns.cc \
	ptPlotElement.cc \
	ptProgElement.cc \
	ptQBoxElement.cc \
	ptStyle.cc \
	ptSymbolElement.cc \
	ptTableElement.cc \
	ptTextElement.cc \
	ptTimemarkerElement.cc \
	ptUTCElement.cc \
	ptVectorElement.cc \
	ptWindVectorElement.cc \
	ptYaxisElement.cc


libpets2_la_SOURCES = \
	$(cc_sources) \
	ptAxisChildElement.cc \
	ptStaticYaxisElement.cc


pkg_includedir = ${includedir}/pets2

pkg_include_HEADERS =        \
	$(cc_sources:.cc=.h) \
	ptEditModes.h        \
	ptGlobals.h          \
	ptStyleOrder.h       \
	ptXAxisInfo.h        \
	pets2Version.h

noinst_HEADERS = polyStipMasks.h


libpets2_la_CPPFLAGS = \
	$(QT4_CPPFLAGS) \
	${PUTOOLS_CPPFLAGS} \
	${TSDATA_CPPFLAGS}

libpets2_la_LDFLAGS = ${VERSION_INFO}
libpets2_la_LIBADD =

if ENABLE_SHARED_LIBRARY
libpets2_la_LDFLAGS += \
	$(QT4_LDFLAGS) \
	${PUTOOLS_LDFLAGS} \
	${TSDATA_LDFLAGS}

libpets2_la_LIBADD += \
	$(QT4_LIBS) \
	${PUTOOLS_LIBS} \
	${TSDATA_LIBS}
endif


INSTALL=install -p 

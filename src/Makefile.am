pkglib_LTLIBRARIES = lib@PACKAGE_NAME@.la

lib@PACKAGE_NAME@_la_SOURCES = \
ptAxesElement.cc \
ptAxisChildElement.cc \
ptAxisHistogramElement.cc \
ptBoxElement.cc \
ptCloudElement.cc \
ptColor.cc \
ptDateElement.cc \
ptDayElement.cc \
ptDiagram.cc \
ptDoubleLineElement.cc \
ptEditLineElement.cc \
ptFontCollGlc.cc \
ptFontCollGLPR.cc \
ptFontCollX.cc \
ptFontDefs.cc \
ptFontManager.cc \
ptGridlineElement.cc \
ptHistogramElement.cc \
ptImage.cc \
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
ptStaticYaxisElement.cc \
ptStyle.cc \
ptSymbolElement.cc \
ptTableElement.cc \
ptTextElement.cc \
ptTimemarkerElement.cc \
ptUTCElement.cc \
ptVectorElement.cc \
ptWindVectorElement.cc \
ptYaxisElement.cc

lib@PACKAGE_NAME@_la_CPPFLAGS = \
	${GL_CPPFLAGS} \
	${tsData_CFLAGS} ${glText_CFLAGS} ${glp_CFLAGS} ${puMet_CFLAGS} \
	${puDatatypes_CFLAGS} ${parameter_CFLAGS} ${puTools_CFLAGS} \
	${miLogger_CFLAGS} \
	${LOG4CXX_CPPFLAGS} ${GL_CFLAGS}

lib@PACKAGE_NAME@_la_LDFLAGS = \
	${tsData_LIBS} ${glText_LIBS} ${glp_LIBS} ${puMet_LIBS} \
	${puDatatypes_LIBS} ${parameter_LIBS} ${puTools_LIBS} \
	${miLogger_LIBS} \
	${LOG4CXX_LDFLAGS} ${LOG4CXX_LIBS}  \
	${GL_LDFLAGS} ${GL_LIBS} ${VERSION_INFO}

INSTALL=install -p # tells install to preserve timestamp
DEFAULT_INCLUDES=-I${srcdir}/../include

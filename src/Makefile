#----------------------------------------
# ABI-independent makefile for petslib
# called from ../Makefile
#----------------------------------------

COBJ=

CCOBJ=ptFontManager.o ptColor.o ptFontDefs.o ptPatterns.o ptLayout.o ptMarker.o ptPlotElement.o ptLineElement.o ptEditLineElement.o ptVectorElement.o ptWindVectorElement.o ptGridlineElement.o ptHistogramElement.o ptCloudElement.o ptTableElement.o ptTextElement.o ptDiagram.o ptAxesElement.o ptProgElement.o ptDateElement.o ptUTCElement.o ptDayElement.o ptYaxisElement.o ptStaticYaxisElement.o ptAxisChildElement.o ptStyle.o ptDoubleLineElement.o ptImage.o ptSymbolElement.o ptTimemarkerElement.o ptMultiLineElement.o ptAxisHistogramElement.o ptLogoElement.o ptBoxElement.o ptIntervalElement.o ptQBoxElement.o

CSRC=$(COBJ:.o=.c)

CCSRC=$(CCOBJ:.o=.cc)

LIBOBJ=$(COBJ) $(CCOBJ)

LIBSRC=$(CSRC) $(CCSRC)

VPATH=$(LIBDIR)

.SUFFIXES: .cc .o

.cc.o:
	$(CXX) $(CCFLAGS) -c $< $(INCLUDE) 

.c.o:
	$(CC) $(CFLAGS) -c $< $(INCLUDE) 

all: $(LIBDIR)/libpets2.a

depends:
	rm -f $(DEPENDSFILE)
	echo '# Automatically-generated dependencies list:' > $(DEPENDSFILE)
	$(CXX) $(CCFLAGS) $(INCLUDE) -M $(LIBSRC) >> $(DEPENDSFILE)

$(LIBDIR)/libpets2.a: $(LIBOBJ)
	$(AR) $(ARFLAGS) $@ $(LIBOBJ)


include $(DEPENDSFILE)

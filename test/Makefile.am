
check_PROGRAMS = TestQPainter


CPPFLAGS = -DTEST_SRCDIR=\"$(abs_srcdir)/\" \
	-I$(top_srcdir)/src \
	-I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui \
	$(QT4_CPPFLAGS) \
	${PUTOOLS_CPPFLAGS} \
	${TSDATA_CPPFLAGS}

LDFLAGS = \
	$(QT4_LDFLAGS) \
	${PUTOOLS_LDFLAGS} \
	${TSDATA_LDFLAGS}

LDADD = $(top_builddir)/src/libpets2.la \
	$(QT4_LIBS) \
	${PUTOOLS_LIBS} \
	${TSDATA_LIBS}

TestQPainter_SOURCES = TestQPainter.cc

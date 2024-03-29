PROJECT(metlibs-pets2)
CMAKE_MINIMUM_REQUIRED(VERSION 3.10)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE(MetnoUtils)
INCLUDE(MetnoQtUtils)
ENABLE_TESTING()

METNO_PVERSION_DEFINES(PETS2 "src/pets2Version.h")

SET(PKGS
  metlibs-putools>=8.0.0
  metlibs-tsdata-core>=8.0.0
  metlibs-milogger>=6.0.0
)

PKG_CHECK_MODULES(PC_METLIBS REQUIRED
  ${PKGS}
)

FIND_PACKAGE(Boost REQUIRED)

METNO_FIND_QT(Widgets)

SET(lib_name "metlibs-pets2${METNO_QT_SUFFIX}")

METNO_GEN_PKGCONFIG(metlibs-pets2.pc.in "${lib_name}.pc"
  "${PKGS};${METNO_PC_DEPS_QT}" "" "" "")

CONFIGURE_FILE(pets2.pc.in pets2.pc @ONLY)
INSTALL(FILES ${CMAKE_BINARY_DIR}/pets2.pc DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig")


ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

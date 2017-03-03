
INCLUDE_DIRECTORIES(
  "${CMAKE_SOURCE_DIR}/src"
)
LINK_DIRECTORIES(${PC_METLIBS_LIBRARY_DIRS} ${BOOST_LIBRARY_DIRS})

ADD_DEFINITIONS(-DTEST_SRCDIR="${CMAKE_CURRENT_SOURCE_DIR}")

ADD_EXECUTABLE(pets2_test
  TestQPainter.cc
)

TARGET_LINK_LIBRARIES(pets2_test
  pets2
)

ADD_TEST(NAME pets2_test
  COMMAND pets2_test -platform offscreen
)
SET_TESTS_PROPERTIES(pets2_test
  # the offscreen platform seems to need some help with finding fonts
  PROPERTIES ENVIRONMENT "QT_QPA_FONTDIR=/usr/share/fonts/truetype"
)

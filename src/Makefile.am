ACLOCAL_AMFLAGS = -I m4

AM_LDFLAGS = -lgdal

lib_LTLIBRARIES = libgdal_correlator.la

libgdal_correlator_la_SOURCES = sources/GDALFeaturePoint.cpp \
  sources/GDALFeaturePointsCollection.cpp \
  sources/GDALIntegralImage.cpp \
  sources/GDALMatchedPointsCollection.cpp \
  sources/GDALOctaveLayer.cpp \
  sources/GDALOctaveMap.cpp \
  sources/GDALSimpleSURF.cpp

libgdal_correlator_la_CPPFLAGS = -Iheaders -I/usr/include/gdal --pedantic -Wall -ansi

include_HEADERS = headers/GDALCorrelator.h \
  headers/GDALFeaturePoint.h \
  headers/GDALFeaturePointsCollection.h \
  headers/GDALIntegralImage.h \
  headers/GDALMatchedPointsCollection.h \
  headers/GDALOctaveLayer.h \
  headers/GDALOctaveMap.h \
  headers/GDALSimpleSURF.h

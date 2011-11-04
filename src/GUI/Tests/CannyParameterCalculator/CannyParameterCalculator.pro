TEMPLATE = app
TARGET = CannyParameterCalculator 

unix {
CONFIG += link_pkgconfig
PKGCONFIG += opencv
}
Win32 {
INCLUDEPATH += C:\OpenCV2.2\include\
LIBS += -LC:\OpenCV2.2\lib \
-lopencv_core220 \
-lopencv_highgui220 \
-lopencv_imgproc220 \
-lopencv_features2d220 \
-lopencv_calib3d220
}



QT        += core gui 

HEADERS   += cannyparametercalculator.h
SOURCES   += main.cpp \
    cannyparametercalculator.cpp
FORMS     += cannyparametercalculator.ui    
RESOURCES +=

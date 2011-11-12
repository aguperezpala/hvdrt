TEMPLATE = app
TARGET = ZoneSelector
QT += core \
    gui
unix { 
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
}
Win32:INCLUDEPATH += C:\OpenCV2.2\include \
    LIBS \
    += \
    -LC:\OpenCV2.2\lib \
    -lopencv_core220 \
    -lopencv_highgui220 \
    -lopencv_imgproc220 \
    -lopencv_features2d220 \
    -lopencv_calib3d220
    
INCLUDEPATH += ../../../Common/Debug 
HEADERS += MouseQLabel.h \
	../../../Common/Debug/DebugUtil.h \
    zoneselector.h
SOURCES += MouseQLabel.cpp \
    main.cpp \
    zoneselector.cpp
FORMS += zoneselector.ui
RESOURCES += 

TEMPLATE = app
TARGET = ImageGeneratorConfigurator
DEFINES += DEBUG
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

LIBS	  += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_calib3d \
	-lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_ml \
 	-lopencv_objdetect -lopencv_video -lopencv_ts -lopencv_legacy



INCLUDEPATH += ../../Common/Debug \
	../../Common/Timestamp \
	../../Common \
    ../../DeviceCalibrator \
    ../../GUI/CannyParameterCalculator \
    ../../GUI/RealTimeDataDisplayer \
    ../../GUI/VideoCaptureCanny \
    ../../GUI/ZoneSelector \
    ../../GUI/Utils \
    ../../IPSModule/ImageProcessingSystem \
    ../../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter \
    ../../IPSModule/ImageProcessingSystem/WaveHeightIPS \
    ../../ImageAnalyzer \
    ../../ImageAnalyzer/ImageInterpreter \
    ../../ImageGenerator 
HEADERS += ../../Common/Debug/DebugUtil.h \
    ../../Common/GlobalDefines.h \
    ../../Common/Timestamp/Timestamp.h \
    ../../DeviceCalibrator/DeviceCalibrator.h \
    ../../GUI/Utils/GUIUtils.h \
    ../../IPSModule/ImageProcessingSystem/ImageProcessingSystem.h \
    ../../ImageAnalyzer/ImageAnalyzer.h \
    ../../ImageAnalyzer/ImageInterpreter/ImageInterpreter.h \
    ../../ImageAnalyzer/ImageProcessor.h \
    ../../ImageGenerator/Frame.h \
    ../../ImageGenerator/FrameEmitter.h \
    ../../ImageGenerator/FrameListener.h \
    ../../ImageGenerator/ImageGenerator.h 
SOURCES += ../../DeviceCalibrator/DeviceCalibrator.cpp \
    ../../GUI/Utils/GUIUtils.cpp \
  ../../ImageAnalyzer/ImageAnalyzer.cpp \
    ../../ImageGenerator/FrameEmitter.cpp \
    ../../ImageGenerator/ImageGenerator.cpp 



INCLUDEPATH += ../../Common/Debug 
HEADERS += ../../Common/Debug/DebugUtil.h \
    imagegeneratorconfigurator.h
SOURCES += main.cpp \
    imagegeneratorconfigurator.cpp
FORMS += imagegeneratorconfigurator.ui
RESOURCES += 

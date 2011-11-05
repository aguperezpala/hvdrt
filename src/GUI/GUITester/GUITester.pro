TEMPLATE = app
TARGET = GUITester 
DEFINES += DEBUG

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
    ../../ImageGenerator \
    ../../Tests/SystemCLITest 


# ImageGeneratorConfigurator
INCLUDEPATH += ../ImageGeneratorConfigurator \
	../../ImageGenerator
HEADERS   += ../../Common/GlobalDefines.h \
	../ImageGeneratorConfigurator/imagegeneratorconfigurator.h \
	../../ImageGenerator/ImageGenerator.h \
	../../ImageGenerator/Frame.h \
	../../ImageGenerator/FrameEmitter.h \
	../../ImageGenerator/FrameListener.h 
SOURCES   += ../ImageGeneratorConfigurator/imagegeneratorconfigurator.cpp \
	../../ImageGenerator/ImageGenerator.cpp \
	../../ImageGenerator/FrameEmitter.cpp 
FORMS     += ../ImageGeneratorConfigurator/imagegeneratorconfigurator.ui


HEADERS	  += ../../Common/GlobalDefines.h \
		../../GUI/Utils/GUIUtils.h
SOURCES   += main.cpp \
	../../GUI/Utils/GUIUtils.cpp 


RESOURCES +=

TEMPLATE = app
TARGET = HydrodynamicVD
QT += core \
    gui

LIBS	  += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_calib3d \
	-lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_ml \
 	-lopencv_objdetect -lopencv_video -lopencv_ts -lopencv_legacy

INCLUDEPATH += Common/Debug \
	Common/Timestamp \
	Common \
    DeviceCalibrator \
    GUI/CannyParameterCalculator \
    GUI/RealTimeDataDisplayer \
    GUI/VideoCaptureCanny \
    GUI/ZoneSelector \
    IPSModule/ImageProcessingSystem \
    IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter \
    IPSModule/ImageProcessingSystem/WaveHeightIPS \
    ImageAnalyzer \
    ImageAnalyzer/ImageInterpreter \
    ImageGenerator \
    Tests/SystemCLITest 
HEADERS += Common/Debug/DebugUtil.h \
    Common/GlobalDefines.h \
    Common/Timestamp/Timestamp.h \
    DeviceCalibrator/DeviceCalibrator.h \
    GUI/CannyParameterCalculator/cannyparametercalculator.h \
    GUI/RealTimeDataDisplayer/realtimedatadisplayer.h \
    GUI/VideoCaptureCanny/videocapturecanny.h \
    GUI/ZoneSelector/MouseQLabel.h \
    GUI/ZoneSelector/zoneselector.h \
    IPSModule/ImageProcessingSystem/ImageProcessingSystem.h \
    IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/CoordsInterpreter.h \
    IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightIPS.h \
    ImageAnalyzer/ImageAnalyzer.h \
    ImageAnalyzer/ImageInterpreter/ImageInterpreter.h \
    ImageAnalyzer/ImageProcessor.h \
    ImageGenerator/Frame.h \
    ImageGenerator/FrameEmitter.h \
    ImageGenerator/FrameListener.h \
    ImageGenerator/ImageGenerator.h \
    Tests/SystemCLITest/AuxClasses.h 
SOURCES += DeviceCalibrator/DeviceCalibrator.cpp \
    GUI/CannyParameterCalculator/cannyparametercalculator.cpp \
    GUI/RealTimeDataDisplayer/realtimedatadisplayer.cpp \
    GUI/VideoCaptureCanny/videocapturecanny.cpp \
    GUI/ZoneSelector/MouseQLabel.cpp \
    GUI/ZoneSelector/zoneselector.cpp \
    IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/CoordsInterpreter.cpp \
    IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightIPS.cpp \
    ImageAnalyzer/ImageAnalyzer.cpp \
    ImageGenerator/FrameEmitter.cpp \
    ImageGenerator/ImageGenerator.cpp 
FORMS += GUI/CannyParameterCalculator/cannyparametercalculator.ui \
    GUI/RealTimeDataDisplayer/realtimedatadisplayer.ui \
    GUI/VideoCaptureCanny/videocapturecanny.ui \
    GUI/ZoneSelector/zoneselector.ui 
RESOURCES += 

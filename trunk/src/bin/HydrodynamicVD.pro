TEMPLATE = app
DEFINES += DEBUG
TARGET = HydrodynamicVD
QT += core \
    gui

LIBS	  += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_calib3d \
	-lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_ml \
 	-lopencv_objdetect -lopencv_video -lopencv_ts -lopencv_legacy


LIBS += -L../Common/tinyxml -ltinyxmlpath
INCLUDEPATH += ../Common/tinyxml
DEPENDPATH += ../Common/tinyxml
PRE_TARGETDEPS += ../Common/tinyxml/libtinyxmlpath.a

INCLUDEPATH += ../Common/Debug \
	../Common/Timestamp \
	../Common \
	../Common/tinyxml \
    ../DeviceCalibrator \
    ../GUI/ImageGeneratorConfigurator \
    ../GUI/IPSModule/ImageProcessingSystem \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalizer/CoordsInterpreter \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalizer/BorderDetector \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/RealTimeDataDisplayer \
    ../IPSModule/ImageProcessingSystem \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/BorderDetector \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS \
    ../ImageAnalyzer \
    ../ImageAnalyzer/ImageInterpreter \
    ../ImageGenerator \
    ../GUI/Utils \
    ../GUI/MainProgram \
    ../GUI/GUIConfiguratorDialog \
    ../Tests/SystemCLITest 
HEADERS += ../Common/Debug/DebugUtil.h \
    ../Common/GlobalDefines.h \
	../Common/tinyxml/tinyxml.h \    
    ../Common/Timestamp/Timestamp.h \
    ../DeviceCalibrator/DeviceCalibrator.h \
    ../GUI/Utils/GUIUtils.h \
    #../GUI/MainProgram/MainProgram.h \
    ../GUI/ImageGeneratorConfigurator/imagegeneratorconfigurator.h \
    ../GUI/IPSModule/ImageProcessingSystem/guiimageprocessingsystem.h \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/guiwaveheightips.h \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/guiperspectivetransformator.h \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/MouseQLabel.h \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalizer/CoordsInterpreter/coordsinterpreterconfigurator.h \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalizer/BorderDetector/cannyparametercalculator.h \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/RealTimeDataDisplayer/realtimedatadisplayer.h \    
    ../IPSModule/ImageProcessingSystem/ImageProcessingSystem.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/CoordsInterpreter.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/DataProcessor.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/BorderDetector/BorderDetector.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/PerspectiveTransformator.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightIPS.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImgPointPainter.h \
    ../ImageAnalyzer/ImageAnalyzer.h \
    ../ImageAnalyzer/CallBFunctor.h \
    ../ImageAnalyzer/ImageInterpreter/ImageInterpreter.h \
    ../ImageAnalyzer/ImageProcessor.h \
    ../ImageGenerator/Frame.h \
    ../ImageGenerator/FrameEmitter.h \
    ../ImageGenerator/FrameListener.h \
    ../ImageGenerator/ImageGenerator.h \
    ../GUI/GUIConfiguratorDialog/GUIConfiguratorDialog.h \    
    ../Tests/SystemCLITest/AuxClasses.h 
SOURCES += ../DeviceCalibrator/DeviceCalibrator.cpp \
    ../GUI/Utils/GUIUtils.cpp \
    #../GUI/MainProgram/MainProgram.cpp \
    ../GUI/MainProgram/main.cpp \
    ../GUI/ImageGeneratorConfigurator/imagegeneratorconfigurator.cpp \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/guiwaveheightips.cpp \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/guiperspectivetransformator.cpp \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/MouseQLabel.cpp \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalizer/CoordsInterpreter/coordsinterpreterconfigurator.cpp \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalizer/BorderDetector/cannyparametercalculator.cpp \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/RealTimeDataDisplayer/realtimedatadisplayer.cpp \    
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/CoordsInterpreter.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/DataProcessor.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/BorderDetector/BorderDetector.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/PerspectiveTransformator.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightIPS.cpp \
    ../ImageAnalyzer/ImageAnalyzer.cpp \
    ../ImageGenerator/FrameEmitter.cpp \
    ../ImageGenerator/ImageGenerator.cpp 
FORMS += ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/guiwaveheightips.ui \
	../GUI/ImageGeneratorConfigurator/imagegeneratorconfigurator.ui \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/guiperspectivetransformator.ui \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalizer/CoordsInterpreter/coordsinterpreterconfigurator.ui \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalizer/BorderDetector/cannyparametercalculator.ui \
    ../GUI/IPSModule/ImageProcessingSystem/WaveHeightIPS/RealTimeDataDisplayer/realtimedatadisplayer.ui     
    
RESOURCES += 

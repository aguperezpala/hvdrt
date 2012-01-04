TEMPLATE = app
TARGET = GUIFramework
DEFINES += DEBUG
QT += core \
    gui
LIBS += -L/usr/local/lib \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_highgui \
    -lopencv_calib3d \
    -lopencv_contrib \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_gpu \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_video \
    -lopencv_ts \
    -lopencv_legacy
LIBS += -L../Common/tinyxml \
    -ltinyxmlpath
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
    ../GUI/MainProgram \
    ../GUI/GUIConfiguratorDialog \
    ../Tests/SystemCLITest
INCLUDEPATH += Utils \
	ConfigWindowManager/UnitTest \
	ConfigWindowManager 
	
HEADERS += ConfigWindowManager/UnitTest/configwindowtest3.h \
    ConfigWindowManager/UnitTest/configwindowtest2.h \
    ConfigWindowManager/UnitTest/configwindowtest1.h \
    ConfigWindowManager/configwindowmanager.h \
    ConfigWindowManager/configwindow.h \
    Utils/GUIUtils.h \
    ../Common/Debug/DebugUtil.h \
    ../Common/GlobalDefines.h \
    ../Common/tinyxml/tinyxml.h
HEADERS += guiframework.h
SOURCES += ConfigWindowManager/UnitTest/configwindowtest3.cpp \
    ConfigWindowManager/UnitTest/configwindowtest2.cpp \
    ConfigWindowManager/UnitTest/configwindowtest1.cpp \
    Utils/GUIUtils.cpp \
    ConfigWindowManager/configwindowmanager.cpp \
    main.cpp \
    guiframework.cpp
FORMS += ConfigWindowManager/UnitTest/configwindowtest3.ui \
    ConfigWindowManager/UnitTest/configwindowtest2.ui \
    ConfigWindowManager/UnitTest/configwindowtest1.ui \
    ConfigWindowManager/configwindowmanager.ui \
    guiframework.ui
RESOURCES += 

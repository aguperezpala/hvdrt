TEMPLATE = app
CONFIG += qwt
TARGET = GUIFramework
DEFINES += DEBUG
QMAKE_CXX += -g
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
    -ltinyxmlpath \
    -L/usr/local/qwt-6.0.1/lib \
    -lqwt \
    -lqwtmathml
INCLUDEPATH += ../Common/tinyxml \
    /usr/local/qwt-6.0.1/include
DEPENDPATH += ../Common/tinyxml
DEPENDPATH += /usr/local/qwt-6.0.1/lib
PRE_TARGETDEPS += ../Common/tinyxml/libtinyxmlpath.a

# #############################################################################
# The Hydrodynamic Variable Detection core "source code"
# #############################################################################
INCLUDEPATH += ../Common/Debug \
    ../Common/Timestamp \
    ../Common \
    ../Common/tinyxml \
    ../Common/XmlHelper \
    ../DeviceCalibrator \
    ../IPSModule/ImageProcessingSystem \ # ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter \
# ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/BorderDetector \
# ../IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Analyzer \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Analyzer/WaveHeightAnalyzer \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/BorderDetector \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/BorderDetector/CannyBorderDetector \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Clipping \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Clipping/MiddlePointClipping \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Rectifier \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Rectifier/PerspectiveRectifier \
    ../ImageAnalyzer \
    ../ImageAnalyzer/ImageInterpreter \
    ../ImageGenerator
HEADERS += ../Common/Debug/DebugUtil.h \
    ../Common/GlobalDefines.h \
    ../Common/tinyxml/tinyxml.h \
    ../Common/XmlHelper/XmlHelper.h \
    ../Common/Timestamp/Timestamp.h \
    ../DeviceCalibrator/DeviceCalibrator.h \
    ../IPSModule/ImageProcessingSystem/ImageProcessingSystem.h \ # ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/CoordsInterpreter.h \
# ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/DataProcessor.h \
# ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/BorderDetector/BorderDetector.h \
# ../IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/PerspectiveTransformator.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightException.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightIPFactory.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Analyzer/IAnalyzer.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Analyzer/WaveHeightAnalyzer/WaveHeightAnalyzer.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Analyzer/WaveHeightAnalyzer/ColumnAnalyzer.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/BorderDetector/IBorderDetector.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/BorderDetector/CannyBorderDetector/CannyBorderDetector.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Clipping/IClipping.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Clipping/MiddlePointClipping/MiddlePointClipping.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Rectifier/IRectifier.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Rectifier/PerspectiveRectifier/PerspectiveRectifier.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightIPS.h \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImgPointPainter.h \
    ../ImageAnalyzer/ImageAnalyzer.h \
    ../ImageAnalyzer/CallBFunctor.h \
    ../ImageAnalyzer/ImageInterpreter/ImageInterpreter.h \
    ../ImageAnalyzer/ImageProcessor.h \
    ../ImageGenerator/Frame.h \
    ../ImageGenerator/FrameEmitter.h \
    ../ImageGenerator/FrameListener.h \
    ../ImageGenerator/ImageGenerator.h
SOURCES += ../DeviceCalibrator/DeviceCalibrator.cpp \
    ../Common/XmlHelper/XmlHelper.cpp \ # ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/CoordsInterpreter.cpp \
# ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/CoordsInterpreter/DataProcessor.cpp \
# ../IPSModule/ImageProcessingSystem/WaveHeightIPS/ImageAnalyzer/BorderDetector/BorderDetector.cpp \
# ../IPSModule/ImageProcessingSystem/WaveHeightIPS/DeviceCalibrator/PerspectiveTransformator.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightIPFactory.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Analyzer/WaveHeightAnalyzer/WaveHeightAnalyzer.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Analyzer/WaveHeightAnalyzer/ColumnAnalyzer.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/BorderDetector/CannyBorderDetector/CannyBorderDetector.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Clipping/MiddlePointClipping/MiddlePointClipping.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveImageProcessors/Rectifier/PerspectiveRectifier/PerspectiveRectifier.cpp \
    ../IPSModule/ImageProcessingSystem/WaveHeightIPS/WaveHeightIPS.cpp \
    ../ImageAnalyzer/ImageAnalyzer.cpp \
    ../ImageGenerator/FrameEmitter.cpp \
    ../ImageGenerator/ImageGenerator.cpp

# #############################################################################
INCLUDEPATH += Utils \
    ConfigWindowManager/UnitTest \
    CommonConfigWindows/CameraConfigWindow \
    CommonConfigWindows/VideoFileConfigWindow \
    GuiHelpers/FrameLabelDisplayer \
    IPSModule \
    IPSModule/IPSs/GUIWaveHeightIPS \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows \
    GuiHelpers/ZoomedLabel \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIPerspectiveRectifier \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUICannyBorderDetector \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIMiddlePointClipping \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIRealTimeDataDisplayer \
    IPSModule/IPSs/GUIWaveHeightIPS \
    CommonConfigWindows/DataAnalyzeWindow \
    ConfigWindowManager
HEADERS += GuiHelpers/FrameLabelDisplayer/framelabeldisplayer.h \
    CommonConfigWindows/VideoFileConfigWindow/videofileconfigwindow.h \
    ConfigWindowManager/UnitTest/configwindowtest3.h \
    ConfigWindowManager/UnitTest/configwindowtest2.h \
    ConfigWindowManager/UnitTest/configwindowtest1.h \
    ConfigWindowManager/configwindowmanager.h \
    ConfigWindowManager/configwindow.h \
    Utils/GUIUtils.h \
    ../Common/Debug/DebugUtil.h \
    ../Common/GlobalDefines.h \
    ../Common/tinyxml/tinyxml.h \
    IPSModule/guiimageprocessingsystem.h \
    IPSModule/IPSs/GUIWaveHeightIPS/guiwaveheightips.h \
    CommonConfigWindows/CameraConfigWindow/cameraconfigwindow.h
HEADERS += CommonConfigWindows/DataAnalyzeWindow/CurveData.h \
    CommonConfigWindows/DataAnalyzeWindow/dataanalyzewindow.h \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIRealTimeDataDisplayer/RealTimeProcessorBridge.h \
    IPSModule/IPSs/GUIWaveHeightIPS/DataDisplayerBridge.h \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIRealTimeDataDisplayer/guirealtimedatadisplayer.h \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIMiddlePointClipping/guimiddlepointclipping.h \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUICannyBorderDetector/guicannyborderdetector.h \
    GuiHelpers/ZoomedLabel/zoomedlabel.h \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/imgprocconfigwindows.h \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIPerspectiveRectifier/guiperspectiverectifier.h \
    guiframework.h
SOURCES += CommonConfigWindows/DataAnalyzeWindow/CurveData.cpp \
    CommonConfigWindows/DataAnalyzeWindow/dataanalyzewindow.cpp \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIRealTimeDataDisplayer/RealTimeProcessorBridge.cpp \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIRealTimeDataDisplayer/guirealtimedatadisplayer.cpp \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIMiddlePointClipping/guimiddlepointclipping.cpp \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUICannyBorderDetector/guicannyborderdetector.cpp \
    GuiHelpers/ZoomedLabel/zoomedlabel.cpp \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIPerspectiveRectifier/guiperspectiverectifier.cpp \
    GuiHelpers/FrameLabelDisplayer/framelabeldisplayer.cpp \
    CommonConfigWindows/VideoFileConfigWindow/videofileconfigwindow.cpp \
    ConfigWindowManager/UnitTest/configwindowtest3.cpp \
    ConfigWindowManager/UnitTest/configwindowtest2.cpp \
    ConfigWindowManager/UnitTest/configwindowtest1.cpp \
    Utils/GUIUtils.cpp \
    ConfigWindowManager/configwindowmanager.cpp \
    IPSModule/IPSs/GUIWaveHeightIPS/guiwaveheightips.cpp \
    CommonConfigWindows/CameraConfigWindow/cameraconfigwindow.cpp \
    main.cpp \
    guiframework.cpp
FORMS += CommonConfigWindows/DataAnalyzeWindow/dataanalyzewindow.ui \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIRealTimeDataDisplayer/guirealtimedatadisplayer.ui \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIMiddlePointClipping/guimiddlepointclipping.ui \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUICannyBorderDetector/guicannyborderdetector.ui \
    IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIPerspectiveRectifier/guiperspectiverectifier.ui \
    GuiHelpers/FrameLabelDisplayer/framelabeldisplayer.ui \
    CommonConfigWindows/VideoFileConfigWindow/videofileconfigwindow.ui \
    ConfigWindowManager/UnitTest/configwindowtest3.ui \
    ConfigWindowManager/UnitTest/configwindowtest2.ui \
    ConfigWindowManager/UnitTest/configwindowtest1.ui \
    IPSModule/IPSs/GUIWaveHeightIPS/guiwaveheightips.ui \
    ConfigWindowManager/configwindowmanager.ui \
    CommonConfigWindows/CameraConfigWindow/cameraconfigwindow.ui \
    guiframework.ui
RESOURCES += 

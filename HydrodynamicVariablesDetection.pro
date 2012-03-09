TEMPLATE = app
TARGET = HydrodynamicVariablesDetection
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
    opencv_ml \
    -lopencv_objdetect \
    -lopencv_video \
    -lopencv_ts \
    -lopencv_legacy
HEADERS += src/GUIFramework/CommonConfigWindows/DataAnalyzeWindow/IntegralCalculator.h \
    src/GUIFramework/MainProgram/MainProgram.h \
    src/GUIFramework/IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIRealTimeDataDisplayer/RealTimeProcessorBridge.h \
    src/GUIFramework/Utils/GUIUtils.h \
    src/GUIFramework/ConfigWindowManager/configwindow.h \
    realtimedatadisplayer.h
SOURCES += src/GUIFramework/CommonConfigWindows/DataAnalyzeWindow/IntegralCalculator.cpp \
    src/GUIFramework/MainProgram/MainProgram.cpp \
    src/GUIFramework/IPSModule/IPSs/GUIWaveHeightIPS/ImgProcConfigWindows/GUIRealTimeDataDisplayer/RealTimeProcessorBridge.cpp \
    src/GUIFramework/Utils/GUIUtils.cpp \
    src/GUIFramework/ConfigWindowManager/configwindow.cpp \
    main.cpp \
    realtimedatadisplayer.cpp
FORMS += src/GUIFramework/ConfigWindowManager/configwindow.ui \
    realtimedatadisplayer.ui
RESOURCES += 

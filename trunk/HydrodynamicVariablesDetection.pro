TEMPLATE = app
TARGET = HydrodynamicVariablesDetection 

QT        += core gui 
LIBS	  += -L/usr/local/lib -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_calib3d \
	-lopencv_contrib -lopencv_features2d -lopencv_flann -lopencv_gpu opencv_ml \
 	-lopencv_objdetect -lopencv_video -lopencv_ts -lopencv_legacy

HEADERS   += realtimedatadisplayer.h
SOURCES   += main.cpp \
    realtimedatadisplayer.cpp
FORMS     += realtimedatadisplayer.ui    
RESOURCES +=

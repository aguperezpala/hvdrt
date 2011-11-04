#include "realtimedatadisplayer.h"

#include <QtGui>
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RealTimeDataDisplayer w;

    w.setMaxXValue(100.0);
    w.setMaxYValue(10.0);
    w.setMeasureScale(100.0);
    w.setTimeScale(500.0);

    float dTime = 100.0/2000.0;
    for(int i = 0; i < 2000; i ++){
    	float p = qrand() %  100;
    	if(p < 0){
    		p *= -1;
    	}
    	p /= 20.0;

    	w.addNewPoint(dTime+i*dTime,p);
    }

    if(!w.init()){
    	std::cerr << "Error initializing the RealTimeDataDisplayer\n";
    	return -1;
    }
    w.show();
    return a.exec();
}

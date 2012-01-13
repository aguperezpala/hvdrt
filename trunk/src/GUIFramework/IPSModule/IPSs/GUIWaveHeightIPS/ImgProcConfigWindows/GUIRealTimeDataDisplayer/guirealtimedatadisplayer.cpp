

#include "DebugUtil.h"
#include "GUIUtils.h"
#include "WaveHeightAnalyzer.h"

#include "guirealtimedatadisplayer.h"



////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::createAxis(void)
{
	float xLen = 500;
	float yLen = 500;

	// create the X axis
	QGraphicsLineItem *xAxis = new QGraphicsLineItem(0,0.0,xLen,0.0);
	mScene.addItem(xAxis);


	// creates the Y axis
	QGraphicsLineItem *yAxis = new QGraphicsLineItem(0.0,0,0.0,-yLen);
	mScene.addItem(yAxis);


	QGraphicsTextItem *t = new QGraphicsTextItem("(0,0)");
	t->setPos(-50,50);
	t->setDefaultTextColor(QColor(100,100,100));
	mScene.addItem(t);


}


////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::createNewPoint(float x, float y)
{
	// invert the y axis
	y *= -1;

	// add the line
	const QGraphicsEllipseItem *lastPoint = mPoints.back();
	createNewLine(lastPoint->x(), lastPoint->y(),
			x, y);

	// add to the list of points

	// create the ellipse
	QGraphicsEllipseItem *point = mScene.addEllipse(-1,-1,2,2);
	point->setPos(x,y);
	mPoints.push_back(point);
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::redrawAll(float oldXScale, float oldYScale)
{
	ASSERT(mLines.size() == mPoints.size()-1);

	// first of all we will iterate over all the points and we will change
	// their position using the oldScale
	float realX, realY;
	oldXScale = 1.0f / oldXScale;
	oldYScale = 1.0f / oldYScale;
	QGraphicsLineItem *li;
	QGraphicsEllipseItem *pi, *pi2;

	for(int i = mPoints.size()-1; i >= 0; --i){
		pi = mPoints[i];
		realX = pi->x() * oldXScale;
		realY = pi->y() * oldYScale;

		pi->setPos(realX * mXAxisScale, realY * mYAxisScale);
	}

	// change the lines
	for(int i = mLines.size()-1; i >= 0 ; --i){
		pi = mPoints[i];
		pi2 = mPoints[i+1];
		li = mLines[i];
		li->setLine(pi->x(), pi->y(), pi2->x(),
				pi2->y());

	}

	mScene.update();

}

////////////////////////////////////////////////////////////////////////////////
bool GUIRealTimeDataDisplayer::readAxisScales(float &xScale, float &yScale)
{
	bool ok;
	xScale = ui.xScaleLineEdit->text().toFloat(&ok);
	if(!ok){
		return false;
	}
	yScale = ui.yScaleLineEdit->text().toFloat(&ok);

	return ok;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUIRealTimeDataDisplayer::GUIRealTimeDataDisplayer(ImageGenerator *ig,QWidget *parent)
    : ImgProcConfigWindows(ig, parent, "GUIRealTimeDataDisplayer"),
      mXAxisScale(-1),
      mYAxisScale(-1),
      mOldXAxisScale(-1),
      mOldYAxisScale(-1)
{
	ui.setupUi(this);

	mLines.reserve(RTDD_RESERVE_NUM_ITEMS);
	mPoints.reserve(RTDD_RESERVE_NUM_ITEMS);

	ui.graphicsView->setScene(&mScene);

	QObject::connect(ui.changeScaleButton,SIGNAL(clicked(bool)), this,
							SLOT(onChangeScaleClicked(void)));
	QObject::connect(ui.startCapturingButton,SIGNAL(clicked(bool)), this,
							SLOT(onStartCapturingClicked(void)));

	// create the (0,0) point
	QGraphicsEllipseItem *point = new QGraphicsEllipseItem(0,0,2,2);
	mPoints.push_back(point);

	readAxisScales(mXAxisScale, mYAxisScale);
	readAxisScales(mOldXAxisScale, mOldYAxisScale);

	createAxis();
}

////////////////////////////////////////////////////////////////////////////////
GUIRealTimeDataDisplayer::~GUIRealTimeDataDisplayer()
{

}


////////////////////////////////////////////////////////////////////////////////
errCode GUIRealTimeDataDisplayer::loadConfig(const TiXmlElement *elem)
{
	// TODO:

	return INCOMPLETE_CONFIGURATION;


	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
std::auto_ptr<TiXmlElement> GUIRealTimeDataDisplayer::getConfig(void) const
{
	// TODO:
	return std::auto_ptr<TiXmlElement> (0);
}

////////////////////////////////////////////////////////////////////////////////
QString GUIRealTimeDataDisplayer::getInfo(void) const
{
	// TODO:

	return "No tenemos info sobre esta ventana todavia.";
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::windowVisible(void)
{
	// TODO:

}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::windowInvisible(void)
{
	// TODO:

}

////////////////////////////////////////////////////////////////////////////////
errCode GUIRealTimeDataDisplayer::finish(QString &error)
{
	// TODO:

}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::addPoint(double timeSecs, double mmHeight)
{
	if(mXAxisScale < 0.0f){
		// we have to read the values
		readAxisScales(mXAxisScale, mYAxisScale);
		if(mOldXAxisScale < 0.0f){
			readAxisScales(mOldXAxisScale, mOldYAxisScale);
		}
	}

	createNewPoint(timeSecs * mXAxisScale, mmHeight * mYAxisScale);
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::clearPoints(void)
{
	mScene.clear();
	mPoints.clear();
	mLines.clear();
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::onChangeScaleClicked(void)
{
	readAxisScales(mXAxisScale, mYAxisScale);
	debug("Changing scale: OLD: %f\t%f\t NEW: %f\t%f\n",
			mOldXAxisScale, mOldYAxisScale, mXAxisScale, mYAxisScale);
	redrawAll(mOldXAxisScale, mOldYAxisScale);

	// replace the new values
	mOldXAxisScale = mXAxisScale;
	mOldYAxisScale = mYAxisScale;
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::onStartCapturingClicked(void)
{
	// read the axis scales
	readAxisScales(mXAxisScale, mYAxisScale);

	// advise the Bridge to start capturing
	// TODO:
}

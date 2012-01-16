

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
	mXAxisLine = new QGraphicsLineItem(0,0.0,xLen,0.0);
	mScene.addItem(mXAxisLine);


	// creates the Y axis
	mYAxisLine = new QGraphicsLineItem(0.0,0,0.0,-yLen);
	mScene.addItem(mYAxisLine);


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
		realX = pi->x() * oldXScale * mXAxisScale;
		realY = pi->y() * oldYScale * mYAxisScale;

		pi->setPos(realX, realY);
		updateAxis(realX, realY);
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
void GUIRealTimeDataDisplayer::updateAxis(float x, float y)
{
	ASSERT(mXAxisLine);
	ASSERT(mYAxisLine);

	QLineF l1 = mXAxisLine->line();
	if(x > l1.x2()){
		l1.setP2(QPoint(x, l1.y2()));
		mXAxisLine->setLine(l1);
	}
	QLineF l2 = mYAxisLine->line();
	if(y < l2.y2()){
		l2.setP2(QPoint(l2.x2(), y));
		mYAxisLine->setLine(l2);
	}

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUIRealTimeDataDisplayer::GUIRealTimeDataDisplayer(ImageGenerator *ig,QWidget *parent)
    : ImgProcConfigWindows(ig, parent, "GUIRealTimeDataDisplayer"),
      mXAxisScale(-1),
      mYAxisScale(-1),
      mOldXAxisScale(-1),
      mOldYAxisScale(-1),
      mShowLines(true),
      mCallbackFunct(0),
      mXAxisLine(0),
      mYAxisLine(0)
{
	ui.setupUi(this);

	mLines.reserve(RTDD_RESERVE_NUM_ITEMS);
	mPoints.reserve(RTDD_RESERVE_NUM_ITEMS);

	ui.graphicsView->setScene(&mScene);

	QObject::connect(ui.changeScaleButton,SIGNAL(clicked(bool)), this,
							SLOT(onChangeScaleClicked(void)));
	QObject::connect(ui.startCapturingButton,SIGNAL(clicked(bool)), this,
							SLOT(onStartCapturingClicked(void)));
	QObject::connect(ui.showLinesCheckBox,SIGNAL(toggled(bool)), this,
							SLOT(onShowLinesToggled(bool)));


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

	float x = timeSecs * mXAxisScale;
	float y = mmHeight * mYAxisScale;
	createNewPoint(x, y);

	// check if we have to update the axis given a point
	updateAxis(x,y);

	mScene.update();
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::clearPoints(void)
{
	mScene.clear();
	mPoints.clear();
	mLines.clear();
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::setEventCallbackFunctor(CallbackFunctor *cf)
{
	mCallbackFunct = cf;
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::onChangeScaleClicked(void)
{
	if(!readAxisScales(mXAxisScale, mYAxisScale)){
		return;
	}
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


	// check which event have to emmit
	if(!ui.changeScaleButton->isEnabled()){
		// then we are capturing so we have to emmit the "stop event"
		if(mCallbackFunct){
			(*mCallbackFunct)(EVENT_STOP_CAPTURING);
		}
		ui.changeScaleButton->setEnabled(true);
		ui.showLinesCheckBox->setEnabled(true);

		ui.startCapturingButton->setText("Empezar capturar");
	} else {
		ui.changeScaleButton->setEnabled(false);
		ui.showLinesCheckBox->setEnabled(false);

		ui.startCapturingButton->setText("Parar capturar");
		if(mCallbackFunct){
			(*mCallbackFunct)(EVENT_START_CAPTURING);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::onShowLinesToggled(bool t)
{
	if(t){
		// we have to hide all the lines
		for(int i = mLines.size()-1; i >= 0; --i){
			mLines[i]->hide();
		}
		mShowLines = false;
	} else {
		for(int i = mLines.size()-1; i >= 0; --i){
			mLines[i]->show();
		}
		mShowLines = true;
	}
}









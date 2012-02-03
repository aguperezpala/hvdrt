

#include "DebugUtil.h"
#include "GUIUtils.h"
#include "WaveHeightAnalyzer.h"

#include "guirealtimedatadisplayer.h"


////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::showFrame(void)
{
	mFrameDisplayer.setImage(mImgGenerator->getLastFrameCaptured());
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::enableTimers(void)
{
	if(ui.ploterCheckbox->isChecked()){
		mPlotter.stopRefresh();
		mPlotter.startRefresh();
	}
	if(ui.frameCheckbox->isChecked()){
		if(mTimerId < 0){
			mTimerId = startTimer(FRAME_UPDATE_TIME);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::disableTimers(void)
{
	mPlotter.stopRefresh();
	if(mTimerId >= 0){
		killTimer(mTimerId);
		mTimerId = -1;
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUIRealTimeDataDisplayer::GUIRealTimeDataDisplayer(ImageGenerator *ig,QWidget *parent)
    : ImgProcConfigWindows(ig, parent, "GUIRealTimeDataDisplayer"),
      mCallbackFunct(0),
      mPlotter(0),
      mFrameDisplayer(this),
      mTimerId(-1)
{
	ui.setupUi(this);

	ui.scrollArea->setWidget(&mFrameDisplayer);
	// add the plotter
	ui.verticalLayout->insertWidget(0,&mPlotter);

	QObject::connect(ui.startCapturingButton,SIGNAL(clicked(bool)), this,
							SLOT(onStartCapturingClicked(void)));
	QObject::connect(ui.frameCheckbox,SIGNAL(toggled(bool)), this,
							SLOT(onFrameChecboxToggled(bool)));
	QObject::connect(ui.ploterCheckbox,SIGNAL(toggled(bool)), this,
							SLOT(onPloterChecboxToggled(bool)));

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
	// clear the data
//	mPlotter.clearData();
	enableTimers();
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::windowInvisible(void)
{
	disableTimers();
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIRealTimeDataDisplayer::finish(QString &error)
{
	// TODO:
	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::addPoint(double timeSecs, double mmHeight)
{
	mPlotter.addNewPoint(timeSecs, mmHeight);
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::clearPoints(void)
{
	mPlotter.clearData();
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::setEventCallbackFunctor(CallbackFunctor *cf)
{
	mCallbackFunct = cf;
}


////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::onStartCapturingClicked(void)
{
	// check which event have to emmit
	if(ui.startCapturingButton->text() == "Parar capturar"){
		// then we are capturing so we have to emmit the "stop event"
		if(mCallbackFunct){
			(*mCallbackFunct)(EVENT_STOP_CAPTURING);
		}
		ui.startCapturingButton->setText("Empezar capturar");
	} else {
		ui.startCapturingButton->setText("Parar capturar");
		if(mCallbackFunct){
			(*mCallbackFunct)(EVENT_START_CAPTURING);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::onFrameChecboxToggled(bool t)
{
	if(t){
		enableTimers();
	} else {
		killTimer(mTimerId); mTimerId = -1;
	}
}


////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::onPloterChecboxToggled(bool t)
{
	if(t){
		enableTimers();
	} else {
		mPlotter.stopRefresh();
	}
}


////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::timerEvent(QTimerEvent *e)
{
	if(e->timerId() == mTimerId){
		showFrame();
	}
}









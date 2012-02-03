

#include "DebugUtil.h"
#include "GUIUtils.h"
#include "WaveHeightAnalyzer.h"

#include "guirealtimedatadisplayer.h"



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUIRealTimeDataDisplayer::GUIRealTimeDataDisplayer(ImageGenerator *ig,QWidget *parent)
    : ImgProcConfigWindows(ig, parent, "GUIRealTimeDataDisplayer"),
      mCallbackFunct(0),
      mPlotter(0)
{
	ui.setupUi(this);

	// add the plotter
	ui.verticalLayout->insertWidget(0,&mPlotter);

	QObject::connect(ui.startCapturingButton,SIGNAL(clicked(bool)), this,
							SLOT(onStartCapturingClicked(void)));

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
	mPlotter.stopRefresh();
	mPlotter.startRefresh();
}

////////////////////////////////////////////////////////////////////////////////
void GUIRealTimeDataDisplayer::windowInvisible(void)
{
	mPlotter.stopRefresh();
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









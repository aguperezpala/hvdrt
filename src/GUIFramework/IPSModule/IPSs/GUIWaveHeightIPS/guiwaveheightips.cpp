#include <strstream>
#include "guiwaveheightips.h"




////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::RealTimeDDEventReceiver::RealTimeDDEventReceiver(WaveHeightIPS *ips) :
mIPS(ips)
{
	ASSERT(ips);
}

////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::RealTimeDDEventReceiver::~RealTimeDDEventReceiver(void)
{

}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::RealTimeDDEventReceiver::operator()(int event)
{
	// check which is the event
	switch(event){
	case GUIRealTimeDataDisplayer::EVENT_START_CAPTURING:
	{
		ASSERT(mIPS);
		// execute the ips, in a new thread? TODO: ver esto de los threads
		errCode err = mIPS->execute();
		if(err != NO_ERROR){
			debug("Some error occurr during the executiong of the IPS: %d\n", err);
			GUIUtils::showMessageBox("Ocurrio un error al ejecutar el IPS: " +
					QString::number(err));
		}
	}
		break;
	case GUIRealTimeDataDisplayer::EVENT_STOP_CAPTURING:
	{
		ASSERT(mIPS);
		// stops the execution
		mIPS->stop();
	}
		break;

	default:
		debug("Error trying to process an unknown event: %d\n", event);
		ASSERT(false);
	}
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureIPS(void) throw (WaveHeightException)
{
	errCode err = mWaveHeightIPS.initialize();
	if(err != NO_ERROR){
		throw WaveHeightException(err, "Error inicializando el core del IPS");
	}

	// set the callback
	mWaveHeightIPS.setCallBackFunctor(&mDataDisplayerBridge);

	// TODO: setear las demas cosas:
//	mWaveHeightIPS.setOutFilename("");
//	mWaveHeightIPS.setTrackingMode();
//	mWaveHeightIPS.setProcessMode();
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureCannyWin(void) throw (WaveHeightException)
{
	// we get the 2 processors that we will use
	ImageProcessor *ip = mWaveHeightIPS.getImageProcessor("CannyBorderDetector");
	if(!ip){
		throw WaveHeightException(INTERNAL_ERROR, "No se encontro el CannyBorderDetector"
				" ImageProcessor");
	}
	mCannyWin.setCannyImgProcessor(ip);

	ip = mWaveHeightIPS.getImageProcessor("PerspectiveRectifier");
	if(!ip){
		throw WaveHeightException(INTERNAL_ERROR, "No se encontro el PerspectiveRectifier"
				" ImageProcessor");
	}
	mCannyWin.setPerspectiveRectifierIP(ip);
}


////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureMiddlePointWin(void) throw (WaveHeightException)
{
	ImageProcessor *ip = mWaveHeightIPS.getImageProcessor("MiddlePointClipping");
	if(!ip){
		throw WaveHeightException(INTERNAL_ERROR, "No se encontro el MiddlePointClipping"
				" ImageProcessor");
	}
	mMiddlePointWin.setMiddlePointClippingIP(ip);

	ip = mWaveHeightIPS.getImageProcessor("PerspectiveRectifier");
	if(!ip){
		throw WaveHeightException(INTERNAL_ERROR, "No se encontro el PerspectiveRectifier"
				" ImageProcessor");
	}
	mMiddlePointWin.setPerspectiveRectifierIP(ip);

	ip = mWaveHeightIPS.getImageProcessor("WaveHeightAnalyzer");
	if(!ip){
		throw WaveHeightException(INTERNAL_ERROR, "No se encontro el WaveHeightAnalyzer"
				" ImageProcessor");
	}
	mMiddlePointWin.setWaveHeightAnalyzerIP(ip);

}



////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configurePerspectiveWin(void) throw (WaveHeightException)
{
	ImageProcessor *ip = mWaveHeightIPS.getImageProcessor("PerspectiveRectifier");
	if(!ip){
		throw WaveHeightException(INTERNAL_ERROR, "No se encontro el PerspectiveRectifier"
				" ImageProcessor");
	}
	mPerspectiveWin.setPerspectiveRectifierIP(ip);

	ip = mWaveHeightIPS.getImageProcessor("WaveHeightAnalyzer");
	if(!ip){
		throw WaveHeightException(INTERNAL_ERROR, "No se encontro el WaveHeightAnalyzer"
				" ImageProcessor");
	}
	mPerspectiveWin.setWaveHeightAnalyzerIP(ip);
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureRealTimeDDWin(void) throw (WaveHeightException)
{
	mRealTimeDDWin.setEventCallbackFunctor(&mEventReceiver);
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureVideoFileConfWin(void) throw (WaveHeightException)
{
	mVideoFileWin.setImageGenerator(mWaveHeightIPS.getImageGenerator());
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureCameraConfigWin(void) throw (WaveHeightException)
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::addXmlSessionInfo(TiXmlElement *root)
{
	ASSERT(root);
	TiXmlElement *si = new TiXmlElement("SessionInfo");

	// Session name
	TiXmlElement *sessionName = new TiXmlElement("Name");
	sessionName->SetAttribute("value", ui.sesionLineEdit->text().toAscii().data());
	si->LinkEndChild(sessionName);

	// Description
	TiXmlElement *desc = new TiXmlElement("Description");
	desc->SetAttribute("value", ui.descriptionTextEdit->toPlainText().toAscii().data());
	si->LinkEndChild(desc);

	// Date
	TiXmlElement *date = new TiXmlElement("Date");
	QString strDate = ui.dateTimeEdit->dateTime().toString();
	date->SetAttribute("value", strDate.toAscii().data());
	si->LinkEndChild(date);

	// Input Type
	TiXmlElement *input = new TiXmlElement("InputType");
	QString strInput = QString::number(ui.comboBox->currentIndex());
	input->SetAttribute("value", strInput.toAscii().data());
	si->LinkEndChild(input);


	// check if already exists
	TiXmlElement *toReplace = root->FirstChildElement("SessionInfo");
	if(toReplace){
		root->ReplaceChild(toReplace, *si);
		delete si;
	} else {
		root->LinkEndChild(si);
	}
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIWaveHeightIPS::fillGuiFromXml(const TiXmlElement *elem)
{
	ASSERT(elem);
	const TiXmlElement *root = elem->FirstChildElement("SessionInfo");
	if(!root){
		if((QString(elem->Value()) != "SessionInfo")){
			GUIUtils::showMessageBox("XML Invalido, no se encontro SessionInfo");
			return INVALID_PARAM;
		}
		// else elem == root
		root = elem;
	}

	// extract the fields
	const TiXmlElement *name = elem->FirstChildElement("Name");
	const TiXmlElement *desc = elem->FirstChildElement("Description");
	const TiXmlElement *date = elem->FirstChildElement("Date");
	const TiXmlElement *input = elem->FirstChildElement("InputType");

	if(!name || !desc || !date || !input){
		GUIUtils::showMessageBox("XML Invalido");
		return INVALID_PARAM;
	}

	// extract the values an set it to the gui
	ui.sesionLineEdit->setText(name->Attribute("value"));
	ui.descriptionTextEdit->setPlainText(desc->Attribute("value"));
	ui.dateTimeEdit->setDateTime(QDateTime::fromString(date->Attribute("value")));
	bool ok;
	ui.comboBox->setCurrentIndex(QString(date->Attribute("value")).toInt(&ok));


	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureWindowManager(void) throw (WaveHeightException)
{
	// add all the windows that we will use
	mConfigWinMngr.removeAllWindows();
	mConfigWinMngr.restart();

	// we will add the windows in the order we want to show
	if(/*TODO: check here if we are using video or other*/ true){
		// we are using video file so set the next order
		mConfigWinMngr.addNewWindow(&mVideoFileWin);
		mConfigWinMngr.addNewWindow(&mPerspectiveWin);
		mConfigWinMngr.addNewWindow(&mVideoFileWin);
		mConfigWinMngr.addNewWindow(&mMiddlePointWin);
		mConfigWinMngr.addNewWindow(&mVideoFileWin);
		mConfigWinMngr.addNewWindow(&mCannyWin);
		mConfigWinMngr.addNewWindow(&mVideoFileWin);
		mConfigWinMngr.addNewWindow(&mRealTimeDDWin);

	} else {
		// we are using video camera:
		mConfigWinMngr.addNewWindow(&mCameraConfWin);
		mConfigWinMngr.addNewWindow(&mPerspectiveWin);
		mConfigWinMngr.addNewWindow(&mMiddlePointWin);
		mConfigWinMngr.addNewWindow(&mCannyWin);
		mConfigWinMngr.addNewWindow(&mRealTimeDDWin);
	}
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::enableOptions(bool enable)
{
	ui.sesionLineEdit->setEnabled(enable);
	ui.descriptionTextEdit->setEnabled(enable);
	ui.dateTimeEdit->setEnabled(enable);
	ui.comboBox->setEnabled(enable);
}

////////////////////////////////////////////////////////////////////////////////
bool GUIWaveHeightIPS::checkFields(void)
{
	if(ui.sesionLineEdit->text().isEmpty()){
		GUIUtils::showMessageBox("El nombre de sesion no puede ser vacio");
		return false;
	}
	// nothing else to check
	return true;
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onWinMngrClose(void)
{
	// TODO:
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onLoadSessionClicked(void)
{
	// open the
}


////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onNewSessionClicked(void)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onStartClicked(void)
{
	// TODO
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::GUIWaveHeightIPS(QWidget *parent, int windowW, int windowH)
    : GUIImageProcessingSystem(parent, windowW, windowH),
      mConfigWinMngr(0, windowW, windowH),
      mCannyWin(mWaveHeightIPS.getImageGenerator(), &mConfigWinMngr),
      mMiddlePointWin(mWaveHeightIPS.getImageGenerator(), &mConfigWinMngr),
      mPerspectiveWin(mWaveHeightIPS.getImageGenerator(), &mConfigWinMngr),
      mRealTimeDDWin(mWaveHeightIPS.getImageGenerator(), &mConfigWinMngr),
      mVideoFileWin(&mConfigWinMngr),
      mCameraConfWin(&mConfigWinMngr),
      mEventReceiver(&mWaveHeightIPS),
      mDataDisplayerBridge(&mRealTimeDDWin)
{

	ui.setupUi(this);

	QObject::connect(&mConfigWinMngr, SIGNAL(closeWindowSignal(void)), this,
			SLOT(onWinMngrClose(void)));

	showMaximized();
	activateWindow();
	raise();


}

////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::~GUIWaveHeightIPS()
{

}

////////////////////////////////////////////////////////////////////////////////
const std::string &GUIWaveHeightIPS::getName(void) const
{
	return mWaveHeightIPS.getName();
}

////////////////////////////////////////////////////////////////////////////////
const std::string &GUIWaveHeightIPS::getInfo(void) const
{
	return mWaveHeightIPS.getInfo();
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIWaveHeightIPS::initialize(void)
{
	// Initialize everything
	try {
		configureIPS();

		// Configure the windows
		configureCannyWin();
		configureMiddlePointWin();
		configurePerspectiveWin();
		configureRealTimeDDWin();
		configureVideoFileConfWin();
		configureCameraConfigWin();

	} catch (WaveHeightException &e){
		// some error ocurr :(
		GUIUtils::showMessageBox(e.info.c_str());
		return e.code;
	}

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIWaveHeightIPS::execute(void)
{
	// here we have to show the main window where we have to create the
	// session (info, etc) and where we will choose the input source (camera
	// or video file).
	show();
	exec();

	// Configure the window manager depending which
	configureWindowManager();


	// show the window config manager.
	mConfigWinMngr.startShow();
	mConfigWinMngr.show();
	mConfigWinMngr.exec();

	return NO_ERROR;
}










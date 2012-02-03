#include <qfiledialog.h>

#include <strstream>

#include "guiwaveheightips.h"
#include "XmlHelper.h"




////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::RealTimeDDEventReceiver::RealTimeDDEventReceiver(WaveHeightIPS *ips) :
mIPS(ips),
mError(NO_ERROR)
{
	ASSERT(ips);
	setPriority(QThread::TimeCriticalPriority);
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
		debug("Start capturing\n");
		ASSERT(mIPS);
		// execute the ips, in a new thread? TODO: ver esto de los threads
		mError = NO_ERROR;

//#ifdef DEBUG
//		mError = mIPS->execute();
//		if(mError != NO_ERROR){
//			debug("Some error occurr during the executiong of the IPS: %d\n", mError);
//			GUIUtils::showMessageBox("Ocurrio un error al ejecutar el IPS: " +
//					QString::number(mError));
//		}
//		break;
//#endif

		if(isRunning()){
			mIPS->stop();
			terminate();
		}
		start();
	}
		break;
	case GUIRealTimeDataDisplayer::EVENT_STOP_CAPTURING:
	{
		debug("Stop capturing\n");
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
void GUIWaveHeightIPS::RealTimeDDEventReceiver::run()
{
	debug("Capturing thread started\n");
	mError = mIPS->execute();
	if(mError != NO_ERROR){
		debug("Some error occurr during the executiong of the IPS: %d\n", mError);
		GUIUtils::showMessageBox("Ocurrio un error al ejecutar el IPS: " +
				QString::number(mError));
	}

	debug("Capturing thread finish\n");
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
	errCode err = mVideoFileWin.setImageGenerator(mWaveHeightIPS.getImageGenerator());
	if(err != NO_ERROR){
		throw WaveHeightException(err, "Error al configurar el VideoFileConfigWin");
	}
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureCameraConfigWin(void) throw (WaveHeightException)
{
	errCode err = mCameraConfWin.setImageGenerator(mWaveHeightIPS.getImageGenerator());
	if(err != NO_ERROR){
		throw WaveHeightException(err, "Error al configurar la CameraConfigWindow");
	}
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

	// Input Path
	TiXmlElement *inputPath = new TiXmlElement("InputPath");
	inputPath->SetAttribute("value", mInputPath.toAscii().data());
	si->LinkEndChild(inputPath);

	// Autor
	TiXmlElement *autor = new TiXmlElement("Autor");
	autor->SetAttribute("value", ui.autorText->text().toAscii().data());
	si->LinkEndChild(autor);

	// OutFile
	TiXmlElement *OutFile = new TiXmlElement("OutFile");
	OutFile->SetAttribute("value", ui.outText->text().toAscii().data());
	si->LinkEndChild(OutFile);


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
	const TiXmlElement *name = root->FirstChildElement("Name");
	const TiXmlElement *desc = root->FirstChildElement("Description");
	const TiXmlElement *date = root->FirstChildElement("Date");
	const TiXmlElement *input = root->FirstChildElement("InputType");
	const TiXmlElement *inputPath = root->FirstChildElement("InputPath");
	const TiXmlElement *autor = root->FirstChildElement("Autor");
	const TiXmlElement *dataOut = root->FirstChildElement("OutFile");

	if(!name || !desc || !date || !input || !inputPath || !autor || !dataOut){
		GUIUtils::showMessageBox("XML Invalido");
		return INVALID_PARAM;
	}

	// extract the values an set it to the gui
	ui.sesionLineEdit->setText(name->Attribute("value"));
	ui.descriptionTextEdit->setPlainText(desc->Attribute("value"));
	ui.dateTimeEdit->setDateTime(QDateTime::fromString(date->Attribute("value")));
	bool ok;
	ui.comboBox->setCurrentIndex(QString(input->Attribute("value")).toInt(&ok));
	ui.inputLabel->setText(inputPath->Attribute("value"));
	mInputPath = inputPath->Attribute("value");

	ui.autorText->setText(autor->Attribute("value"));
	ui.outText->setText(dataOut->Attribute("value"));


	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::configureWindowManager(bool fromFile) throw (WaveHeightException)
{
	// add all the windows that we will use
	mConfigWinMngr.removeAllWindows();

	// we will add the windows in the order we want to show
	if(fromFile){
		// we are using video file so set the next order
		mConfigWinMngr.addNewWindow(&mVideoFileWin);
		mConfigWinMngr.addNewWindow(&mPerspectiveWin);
		mConfigWinMngr.addNewWindow(&mVideoFileWin);
		mConfigWinMngr.addNewWindow(&mMiddlePointWin);
		//mConfigWinMngr.addNewWindow(&mVideoFileWin);
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
	if(ui.autorText->text().isEmpty()){
		GUIUtils::showMessageBox("El nombre del autor no puede ser vacio");
		return false;
	}
	if(ui.outText->text().isEmpty()){
		GUIUtils::showMessageBox("El nombre del archivo de salida no puede ser"
				" vacio");
		return false;
	}

	// nothing else to check
	return true;
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::clearFields(void)
{
	ui.fileLabel->setText("--");
	ui.sesionLineEdit->setText("");
	ui.descriptionTextEdit->setPlainText("");
	ui.inputLabel->setText("none");
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onWinMngrClose(void)
{
	debug("ConfigWinMngr closed event detected, showing GUIWaveHeightIPS again\n");

	mConfigWinMngr.hide();

	// here we have to save all the info in the session file
	TiXmlElement *root = new TiXmlElement("ConfigInfo");
	std::auto_ptr<TiXmlElement> xml(0);
	if(mConfigWinMngr.getConfig(xml) != NO_ERROR){
		GUIUtils::showMessageBox("Error al intentar guardar las configuraciones");
		return;
	}

	// check if we have to replace the configurations
	ASSERT(mDocument);
	TiXmlElement *session = mDocument->RootElement();
	ASSERT(session);

	TiXmlElement *toReplace = session->FirstChildElement("ConfigInfo");
	ASSERT(xml.get());
	root->LinkEndChild(xml.release());
	if(toReplace){
		// we have to replace
		session->ReplaceChild(toReplace, *root);
		delete root;
	} else {
		session->LinkEndChild(root);
	}


	// save the file
	if(!mDocument->SaveFile(ui.fileLabel->text().toAscii().data())){
		GUIUtils::showMessageBox("Error al intentar guardar la session en " +
				ui.fileLabel->text());
	}

	show();

}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onLoadSessionClicked(void)
{
	QString filename = QFileDialog::getOpenFileName(0, "Cargar session", ".", "*.xml");
	if(filename.isEmpty()){
		return;
	}
	clearFields();

	if(mDocument){
		delete mDocument; mDocument = 0;
	}
	// Try to parse it
	mDocument = XmlHelper::loadFromFile(filename.toAscii().data());
	if(!mDocument){
		GUIUtils::showMessageBox("Error cargando el archivo " + filename);
		return;
	}

	errCode err = fillGuiFromXml(mDocument->RootElement());
	if(err != NO_ERROR){
		GUIUtils::showMessageBox("Error parseando el archivo xml " + filename);
		return;
	}

	// if we are here, everything is ok
	ui.fileLabel->setText(filename);

	// disable all the options
	enableOptions(false);

	ui.startButton->setEnabled(true);
}


////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onNewSessionClicked(void)
{
	clearFields();

	QString filename = QFileDialog::getSaveFileName(0, "Nueva session", ".", "*.xml");
	if(filename.isEmpty()){
		return;
	}

	ui.fileLabel->setText(filename);

	// enable options
	enableOptions(true);
	mInputPath = "none";
	ui.inputLabel->setText(mInputPath);
	ui.startButton->setEnabled(true);
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onStartClicked(void)
{
	// Here we have to:
	// 1) Check the input path, if we are selecting from file then we open the
	// qfiledialog.
	// 2) Get the xml associated an save it to the file.
	// 3) Show and start the mConfigWinManager

	if(mInputPath == "none"){
		if(ui.comboBox->currentIndex() == 0){
			// video file
			QString filename = QFileDialog::getOpenFileName(0,
					"Cargar archivo de video", ".", "*");
			if(filename.isEmpty()){
				return;
			}
			mInputPath = filename;
		} else {
			// from webcam
			mInputPath = "Camera0";
		}
	}

	ui.inputLabel->setText(mInputPath);

	// create the device by the mInputPath
	ImageGenerator *ig = mWaveHeightIPS.getImageGenerator();
	ig->destroyDevice();
	bool isVideo = false;
	if(mInputPath == "Camera0"){
		if(!ig->createDevice(0)){
			GUIUtils::showMessageBox("Error al crear el ImageGenerator en"
					" el dispositivo 0");
			mInputPath = "none";
			return;
		}
		isVideo = false;
		// configure the camera
		try{
			configureCameraConfigWin();
		}catch(WaveHeightException &e){
			QString info = e.info.c_str();
			GUIUtils::showMessageBox(info + " | error code:" + QString::number(e.code));
			return;
		}
	} else {
		// load the new ImageGenerator
		if(!ig->createDevice(mInputPath.toAscii().data())){
			GUIUtils::showMessageBox("Error al crear el ImageGenerator desde"
					" archivo " + mInputPath );
			mInputPath = "none";
			return;
		}
		isVideo = true;
		try{
			configureVideoFileConfWin();
		}catch(WaveHeightException &e){
			QString info = e.info.c_str();
			GUIUtils::showMessageBox(info + " | error code:" + QString::number(e.code));
			return;
		}
	}

	// Save the xml
	TiXmlElement *root = 0;
	if(!mDocument){
		mDocument = new TiXmlDocument();
		root = new TiXmlElement("Session");
		mDocument->LinkEndChild(root);
	} else {
		root = mDocument->RootElement();
	}

	addXmlSessionInfo(root);
	if(!mDocument->SaveFile(ui.fileLabel->text().toAscii().data())){
		GUIUtils::showMessageBox("Error al guardar la session en el archivo " +
				ui.fileLabel->text());
	}

	// hide this window
	hide();

	// Configure the window manager depending which (if currentIndex == 0 -> From
	// file, otherwise is from camera
	configureWindowManager(isVideo);

	// check if we have info to load
	TiXmlElement *session = mDocument->RootElement();
	ASSERT(session);
	TiXmlElement *configInfo = session->FirstChildElement("ConfigInfo");
	if(configInfo){
		errCode err = mConfigWinMngr.loadConfig(configInfo);
		if(err != NO_ERROR){
			GUIUtils::showMessageBox("Error al cargar las configuraciones"
					" de las ventanas \"ConfigWindows\": " + QString::number(err));
		}
	}

	// set the outfile name
	mWaveHeightIPS.setOutFilename(ui.outText->text().toAscii().data());


	// show the window config manager.
	mConfigWinMngr.restart();
	mConfigWinMngr.show();
	mConfigWinMngr.showMaximized();
	mConfigWinMngr.activateWindow();
	mConfigWinMngr.raise();
	mConfigWinMngr.exec();

}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onOutClicked(void)
{
	QString filename = QFileDialog::getSaveFileName(0, "Archivo salida", ".", "*.agu");
	if(filename.isEmpty()){
		return;
	}

	ui.outText->setText(filename);
}

////////////////////////////////////////////////////////////////////////////////
void GUIWaveHeightIPS::onAnalyzeClicked(void)
{
	// first check if the outText file exists, if not, then we can open a new
	// window (qfiledialgobox) and let the user to choose it.
	// after that we just open a new window (separated window) and show it
	//
	mDataAnalyzeWin.clear();
	mDataAnalyzeWin.loadFile(ui.outText->text());
	mDataAnalyzeWin.show();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUIWaveHeightIPS::GUIWaveHeightIPS(QWidget *parent, int windowW, int windowH)
    : GUIImageProcessingSystem(parent, windowW, windowH),
      mConfigWinMngr(0, windowW, windowH),
      mCannyWin(mWaveHeightIPS.getImageGenerator(), 0),
      mMiddlePointWin(mWaveHeightIPS.getImageGenerator(), 0),
      mPerspectiveWin(mWaveHeightIPS.getImageGenerator(), 0),
      mRealTimeDDWin(mWaveHeightIPS.getImageGenerator(), 0),
      mVideoFileWin(0),
      mCameraConfWin(0),
      mEventReceiver(&mWaveHeightIPS),
      mDataDisplayerBridge(&mRealTimeDDWin),
      mDocument(0)
{

	ui.setupUi(this);

	QObject::connect(&mConfigWinMngr, SIGNAL(closeWindowSignal(void)), this,
			SLOT(onWinMngrClose(void)));
	QObject::connect(ui.loadSessionButton,SIGNAL(clicked(bool)), this,
						SLOT(onLoadSessionClicked(void)));
	QObject::connect(ui.newSessionButton,SIGNAL(clicked(bool)), this,
						SLOT(onNewSessionClicked(void)));
	QObject::connect(ui.startButton,SIGNAL(clicked(bool)), this,
						SLOT(onStartClicked(void)));
	QObject::connect(ui.outButton,SIGNAL(clicked(bool)), this,
						SLOT(onOutClicked(void)));
	QObject::connect(ui.analyzeButton,SIGNAL(clicked(bool)), this,
						SLOT(onAnalyzeClicked(void)));
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


	debug("Going back!\n");
	return NO_ERROR;
}










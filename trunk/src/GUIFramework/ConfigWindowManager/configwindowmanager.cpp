
#include <algorithm>

#include "DebugUtil.h"
#include "GUIUtils.h"
#include "XmlHelper.h"
#include "configwindowmanager.h"


////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::disconnectSignals(ConfigWindow *w)
{
	ASSERT(w);

	// disconnect the signals and connect it to this one
	QObject::disconnect(w, SIGNAL(doneSignal(int, QString)),
			this, SLOT(configWindowFinished(int, QString)));

	// hide the widget
	w->hide();
	w->windowInvisible();
	ui.configWindowLayout->removeWidget(w);

	// autosave the config
	std::auto_ptr<TiXmlElement> config = w->getConfig();
	if(config.get()){
		addTempXml(config.release(), w->getName());
		updateAutoSaveFile();
	} else {
		debug("Error getting the config of %s\n", w->getName().c_str());
	}

}

////////////////////////////////////////////////////////////////////////////////
bool ConfigWindowManager::showNewWindow(ConfigWindow *w)
{
	ASSERT(w);

	// connect the new ones
	QObject::connect(w, SIGNAL(doneSignal(int, QString)),
			this, SLOT(configWindowFinished(int, QString)));

	// get the info of the window
	if(!w->getInfo().isEmpty()){
		// show the info
		ui.infoLabel->setText(w->getInfo());
		ui.infoLabel->adjustSize();
	}

	ui.configWindowLayout->addWidget(w);
	w->show();

	// get the size of the text
	int th = ui.infoLabel->size().height();

	// the new height of the widget
	int nh = mWindowHeight - th - 200; // the size of the top menu

	w->setMaximumSize(QSize(mWindowWidth - 30,nh));

	// advise the window that we are showing it
	w->windowVisible();

	return true;
}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::showButtons(void)
{
	if(mConfigWindows.empty()){
		ui.nextButton->setEnabled(false);
		ui.backButton->setEnabled(false);
		return;
	}

	if(mConfigWinIt != mConfigWindows.begin()){
		ui.backButton->setEnabled(true);
	} else {
		ui.backButton->setEnabled(false);
	}

	CFListIterator aux = mConfigWinIt;
	++aux;
	if(aux != mConfigWindows.end()){
		ui.nextButton->setEnabled(true);
	} else {
		ui.nextButton->setEnabled(false);
	}

}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::forwardWindow(void)
{
	if(mConfigWinIt == mConfigWindows.end() || mConfigWindows.empty()){
		showButtons();
		return;
	}

	// hide the actual
	disconnectSignals(*mConfigWinIt);

	++mConfigWinIt;
	showButtons();
	// show the new one
	showNewWindow(*mConfigWinIt);

}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::backwardWindow(void)
{
	if(mConfigWinIt == mConfigWindows.begin() || mConfigWindows.empty()){
		// imposible to move back
		showButtons();
		return;
	}

	// hide the actual
	disconnectSignals(*mConfigWinIt);

	--mConfigWinIt;

	showButtons();

	// show the new one
	showNewWindow(*mConfigWinIt);

}

////////////////////////////////////////////////////////////////////////////////
bool ConfigWindowManager::canCloseConfigWindow(ConfigWindow *w)
{
	ASSERT(w);
	QString errInfo;
	errCode err = w->finish(errInfo);

	if(err != NO_ERROR){
		GUIUtils::showMessageBox("No podemos continuar debido a que: " + errInfo);
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
ConfigWindow *ConfigWindowManager::getConfigWindowByName(const std::string &name)
{
	CFListIterator it = mConfigWindows.begin(), endIt = mConfigWindows.end();
	for(; it != endIt; ++it){
		if((*it)->getName() == name){
			return *it;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::addTempXml(TiXmlElement *elem, const std::string &cwn)
{
	ASSERT(elem);

	// check if already exists the actual element
	TiXmlElement *root = mAutosaveDoc.RootElement();
	if(!root || (root = root->FirstChildElement("ConfigWindowSections")) == 0){
		return;
	}

	TiXmlElement *toReplace = XmlHelper::findByAttr(root, "name", cwn.c_str());

	TiXmlElement *newElem = new TiXmlElement("ConfigWindow");
	newElem->SetAttribute("name", cwn.c_str());
	newElem->LinkEndChild(elem);

	if(toReplace){
		root->ReplaceChild(toReplace, *newElem);
		delete newElem;
	} else {
		// we add the element to the xml
		root->LinkEndChild(newElem);
	}

}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::updateAutoSaveFile(void)
{
	if(!mAutosaveDoc.SaveFile(CWM_AUTOSAVE_FILENAME)){
		debug("Error trying to save the CWM_AUTOSAVE_FILENAME\n");
	}
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ConfigWindowManager::ConfigWindowManager(QWidget *parent, int windowW,int windowH)
    : QDialog(parent),
      mConfigWinIt(mConfigWindows.end()),
      mWindowHeight(windowH),
      mWindowWidth(windowW)
{
	ui.setupUi(this);
	QObject::connect(ui.nextButton,SIGNAL(clicked(bool)), this,
					SLOT(nextButtonClicked(void)));
	QObject::connect(ui.backButton,SIGNAL(clicked(bool)), this,
					SLOT(backButtonClicked(void)));
	showButtons();
//	setMinimumHeight(windowH);
//	setMinimumWidth(windowW);

	// create the root xml tag
	TiXmlElement *root = new TiXmlElement("Autosave");
	TiXmlElement *sections = new TiXmlElement("ConfigWindowSections");
	root->LinkEndChild(sections);
	mAutosaveDoc.LinkEndChild(root);
}

////////////////////////////////////////////////////////////////////////////////
ConfigWindowManager::~ConfigWindowManager()
{

}



////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::addNewWindow(ConfigWindow *cw)
{
	ASSERT(cw);
	mConfigWindows.push_back(cw);

	// TODO: verificar si tenemos que mostrar o no los botones
}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::removeWindow(ConfigWindow *cw)
{
	ASSERT(cw);
	CFList::iterator it = std::find(mConfigWindows.begin(), mConfigWindows.end(), cw);

	if(it == mConfigWindows.end()){
		debug("Trying to remove an inexistent ConfigWindow: %s\n", cw->getName().c_str());
		return;
	}
}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::removeAllWindows(void)
{
	mConfigWindows.clear();
}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::startShow(void)
{
	if(mConfigWindows.empty()){
		GUIUtils::showMessageBox("No ConfigWindows set\n");
		return;
	}

	// else we have to start to show the windows (the actual one)
	if(mConfigWinIt == mConfigWindows.end()){
		restart();
	} else {
		// disconnect the signals
		disconnectSignals(*mConfigWinIt);

		// show buttons
		showButtons();

		// show the actual
		showNewWindow(*mConfigWinIt);
	}

	return;
}

/////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::restart(void)
{
	if(mConfigWindows.empty()){
		GUIUtils::showMessageBox("No ConfigWindows set\n");
		return;
	}


	// disconnect the actual signals
	if(mConfigWinIt != mConfigWindows.end()){
		disconnectSignals(*mConfigWinIt);
	}

	mConfigWinIt = mConfigWindows.begin();
	showButtons();
	showNewWindow(*mConfigWinIt);
}

////////////////////////////////////////////////////////////////////////////////
errCode ConfigWindowManager::loadConfig(const TiXmlElement *elem)
{
	ASSERT(elem);
	const TiXmlElement *pElem = elem->FirstChildElement("ConfigWindowSections");
	if(!pElem){
		debug("Invalid xml: no ConfigWindowSections found\n");
		return INVALID_PARAM;
	}

	// now iterate over all the sections
	pElem = pElem->FirstChildElement("ConfigWindow");
	errCode err = NO_ERROR;
	while(pElem){
		ConfigWindow *cw = getConfigWindowByName(pElem->Attribute("name"));
		if(!cw){
			debug("ConfigWindow %s not found to load the configuration\n",
					pElem->Attribute("name"));
			pElem = pElem->NextSiblingElement("ConfigWindow");
			continue;
		}

		// load the configuration
		err = cw->loadConfig(pElem);
		if(err != NO_ERROR){
			debug("Some error ocurr when loading the config in %s: %d\n",
					pElem->Attribute("name"), err);
			return err;
		}

		pElem = pElem->NextSiblingElement("ConfigWindow");
	}

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode ConfigWindowManager::getConfig(std::auto_ptr<TiXmlElement> &xml)
{
	if(xml.get()){
		return INVALID_PARAM;
	}

	// returns the autosave document
	TiXmlElement *root = mAutosaveDoc.RootElement();
	ASSERT(root);
	root = root->FirstChildElement("ConfigWindowSections");
	ASSERT(root);
	xml.reset(root->Clone()->ToElement());
	return NO_ERROR;

	// now we iterate over all the config windows and we will create the
	// ConfigWindowSections
	std::auto_ptr<TiXmlElement> result(new TiXmlElement("ConfigWindowSections"));

	CFListIterator it = mConfigWindows.begin(), endIt = mConfigWindows.end();
	for(; it != endIt; ++it){
		// now get the xml contents of the configWindow
		TiXmlElement *content = (*it)->getConfig().release();
		if(!content){
			debug("Warning: %s have no configuration content\n", (*it)->getName().c_str());
			continue;
		}
		TiXmlElement *cw = new TiXmlElement("ConfigWindow");
		cw->SetAttribute("name", (*it)->getName().c_str());
		// else we put it in the xml
		cw->LinkEndChild(content);
		result->LinkEndChild(cw);
	}

	xml = result;
	return NO_ERROR;

}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::closeEvent(QCloseEvent *e)
{
	debug("Closing ConfigWindowManager\n");
	emit closeWindowSignal();
	QWidget::closeEvent(e);
}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::nextButtonClicked(void)
{
	if(mConfigWinIt != mConfigWindows.end() && canCloseConfigWindow(*mConfigWinIt)){
		forwardWindow();
	} else {
		// do nothing
		showButtons();
	}

}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::backButtonClicked(void)
{
	if(mConfigWinIt != mConfigWindows.end() && canCloseConfigWindow(*mConfigWinIt)){
		backwardWindow();
	} else {
		// do nothing
		showButtons();
	}

}

////////////////////////////////////////////////////////////////////////////////
void ConfigWindowManager::configWindowFinished(int errCode, QString errInfo)
{
	// check if ocurr some error
	if(errCode != NO_ERROR){
		GUIUtils::showMessageBox("Ocurrio algun error en el ConfigWindow\n");
		debug("Error ocurr when window finish automatically: %d\t%s\n",
				errCode, errInfo.toAscii().data());
		return;
	}

	// we move forward if we can
	forwardWindow();
}

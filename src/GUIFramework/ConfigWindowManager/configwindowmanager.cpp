
#include <algorithm>

#include "DebugUtil.h"
#include "GUIUtils.h"
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
	ui.configWindowLayout->removeWidget(w);

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
	int nh = mWindowHeight - th - 100; // the size of the top menu

	w->setMaximumSize(QSize(mWindowWidth - 30,nh));


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
////////////////////////////////////////////////////////////////////////////////
ConfigWindowManager::ConfigWindowManager(QWidget *parent, int windowW,int windowH)
    : QWidget(parent),
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

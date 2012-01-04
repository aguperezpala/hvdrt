#ifndef CONFIGWINDOWMANAGER_H
#define CONFIGWINDOWMANAGER_H

#include <QtGui/QWidget>

#include <list>
#include <auto_ptr.h>

#include "ui_configwindowmanager.h"
#include "configwindow.h"
#include "tinyxml.h"


class TiXmlElement;

class ConfigWindowManager : public QWidget
{
    Q_OBJECT

public:
    ConfigWindowManager(QWidget *parent = 0, int windowW = 800,
    		int windowH = 600);
    ~ConfigWindowManager();

    /* Adds a new ConfigWindow to the list.
     * The order in which the ConfigWindows are added its how they will be
     * shown
     */
    void addNewWindow(ConfigWindow *);

    /* Removes a ConfigWindow (the first matching config window) */
    void removeWindow(ConfigWindow *);

    /* Remove all the windows */
    void removeAllWindows(void);

    /* Start the process of showing the ConfigWindows */
    void startShow(void);

    /* Restart and show the first window */
    void restart(void);

    /* Loads the configuration for every ConfigWindow that the manager is handling
     * Requires:
     * 	@xml	Where it will be extracted the "ConfigWindows" section
     * Returns errCode
     */
    errCode loadConfig(const TiXmlElement *);

    /* Returns the <ConfigWindowSections> filled with all the xml associated to
     * every ConfigWindo. Ready to put in XmlSession
     */
    errCode getConfig(std::auto_ptr<TiXmlElement> &xml);

public slots:
	// Slots
	void nextButtonClicked(void);
	void backButtonClicked(void);

	// Slot associated when the ConfigWindow is automatically closed.
	void configWindowFinished(int errCode, QString errInfo);

private:
	// Function used to disconnect all the actual signals
	void disconnectSignals(ConfigWindow *w);

	/* Function used to set and show new widget (connect signals to). */
	bool showNewWindow(ConfigWindow *w);

	/* Show buttons */
	void showButtons(void);

	/* When we have to go forward with the ConfigWindows */
	void forwardWindow(void);

	/* When we have to go backward with the ConfigWindows */
	void backwardWindow(void);

	/* Check if we can safely close the window */
	bool canCloseConfigWindow(ConfigWindow *);

	/* Returns ConfigWindow by name or 0 if not exist */
	ConfigWindow *getConfigWindowByName(const std::string &name);




private:
    typedef std::list<ConfigWindow *>	CFList;
    typedef CFList::iterator			CFListIterator;


    Ui::ConfigWindowManagerClass 	ui;
    CFList							mConfigWindows;
    CFListIterator					mConfigWinIt;

    int								mWindowWidth;
    int								mWindowHeight;


};

#endif // CONFIGWINDOWMANAGER_H

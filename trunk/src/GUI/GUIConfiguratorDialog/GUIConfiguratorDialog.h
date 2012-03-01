#ifndef GUICONFIGURATORDIALOG_H
#define GUICONFIGURATORDIALOG_H

#include <qdialog.h>

#include <auto_ptr.h>
#include <string>

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "tinyxml.h"

class TiXmlElement;

class GUIConfiguratorDialog : public QWidget
{
    Q_OBJECT

public:
    GUIConfiguratorDialog(QWidget *parent = 0, const std::string &name = "unnamed") :
    	QWidget(parent),
    	mName(name)
    {
//    	showMaximized();
//    	activateWindow();
//    	raise();
    }

    virtual ~GUIConfiguratorDialog(){};

    // returns the name of the configurator window
    const std::string &getName(void) {return mName;}

    /* Function used to load the configurations from a xml file
     * Returns:
     * 	errCode
     */
    virtual errCode loadConfig(TiXmlElement *)
    {
    	debug("Unimplemented\n");
    	return FEATURE_NOT_SUPPORTED;
    }

    /* Save the data to an xml and return it.
     * Returns:
     * 	0			on Error
     * 	xml			on success */
    virtual std::auto_ptr<TiXmlElement> getConfig(void)
	{
    	debug("unimplemented\n");
    	return std::auto_ptr<TiXmlElement> (0);
	}

    /* We have to implement the signal used to advise when the actual Dialog
     * is finish
     */
signals:
    	// Advise when the configurator dialog is finish sending the errCode
    	// associated
    	void doneSignal(int);



private:
    std::string		mName;

};

#endif //GUICONFIGURATORDIALOG_H
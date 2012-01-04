#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QtGui/QWidget>

#include <string>
#include <auto_ptr.h>

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "tinyxml.h"


class TiXmlElement;

class ConfigWindow : public QWidget
{
    Q_OBJECT

public:
    ConfigWindow(QWidget *parent = 0, const std::string &name = "unnamed") :
    	QWidget(parent),
    	mName(name)
    {

    }

    virtual ~ConfigWindow(){};

	// returns the name of the configurator window
	inline const std::string &getName(void) {return mName;}

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
		return std::auto_ptr<TiXmlElement>(0);
	}

	/* Function used to retrieve the info about the ConfigWindow. This is
	 * an optional function.
	 */
	virtual QString getInfo(void) const {return "";}

	/* Function called when the user click on "Next Button" and the ConfigWindow
	 * will be hide and show the next one. This function shall return NO_ERROR
	 * if the ConfigWindow can be closed, or the error and the string error
	 * by param
	 */
	virtual errCode finish(QString &error) = 0;

	/* We have to implement the signal used to advise when the actual ConfigWindow
	 * is finish automatically.
	 */
signals:
	// Advise when the configurator dialog is finish sending the errCode
	// associated and the info about the error associated.
	void doneSignal(int, QString info);

private:
    std::string		mName;
};

#endif // CONFIGWINDOW_H

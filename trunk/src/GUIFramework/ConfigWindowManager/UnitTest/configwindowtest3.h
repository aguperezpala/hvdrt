#ifndef CONFIGWINDOWTEST3_H
#define CONFIGWINDOWTEST3_H

#include <QtGui/QWidget>
#include "ui_configwindowtest3.h"

#include "configwindow.h"

class ConfigWindowTest3 : public ConfigWindow
{
    Q_OBJECT

public:
    ConfigWindowTest3(QWidget *parent = 0);
    ~ConfigWindowTest3();

    /* Function used to retrieve the info about the ConfigWindow. This is
   	 * an optional function.
   	 */
   	virtual QString getInfo(void) const
   	{
   		return "Esto es una prueba de ConfigWindowTest3.......";
   	}

   	/* Function called when the user click on "Next Button" and the ConfigWindow
   	 * will be hide and show the next one. This function shall return NO_ERROR
   	 * if the ConfigWindow can be closed, or the error and the string error
   	 * by param
   	 */
   	virtual errCode finish(QString &error)
   	{
   		return NO_ERROR;
   	}


private:
    Ui::ConfigWindowTest3Class ui;
};

#endif // CONFIGWINDOWTEST3_H

#ifndef CONFIGWINDOWTEST2_H
#define CONFIGWINDOWTEST2_H

#include <QtGui/QWidget>
#include "ui_configwindowtest2.h"

#include "configwindow.h"

class ConfigWindowTest2 : public ConfigWindow
{
    Q_OBJECT

public:
    ConfigWindowTest2(QWidget *parent = 0);
    ~ConfigWindowTest2();

    /* Function used to retrieve the info about the ConfigWindow. This is
	 * an optional function.
	 */
	virtual QString getInfo(void) const
	{
		return "Esto es una prueba de ConfigWindowTest2.......";
	}

	/* Function called when the user click on "Next Button" and the ConfigWindow
	 * will be hide and show the next one. This function shall return NO_ERROR
	 * if the ConfigWindow can be closed, or the error and the string error
	 * by param
	 */
	virtual errCode finish(QString &error)
	{
		static int err = 3;

		if(err > 0){
			err--;
			error = "Testeando a ver que pasa con el error, solo una vez aparece";
			return INVALID_PARAM;
		}

		return NO_ERROR;

	}

private:
    Ui::ConfigWindowTest2Class ui;
};

#endif // CONFIGWINDOWTEST2_H

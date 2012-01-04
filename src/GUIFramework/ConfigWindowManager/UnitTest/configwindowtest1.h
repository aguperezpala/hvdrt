#ifndef CONFIGWINDOWTEST1_H
#define CONFIGWINDOWTEST1_H

#include <QtGui/QWidget>
#include "ui_configwindowtest1.h"
#include "configwindow.h"

class ConfigWindowTest1 : public ConfigWindow
{
    Q_OBJECT

public:
    ConfigWindowTest1(QWidget *parent = 0);
    ~ConfigWindowTest1();

    /* Function used to retrieve the info about the ConfigWindow. This is
	 * an optional function.
	 */
	virtual QString getInfo(void) const
	{
		return "Esto es una prueba de ConfigWindowTest1, hay que ver como queda toda\n"
				" este comentario asi puesto si no se defasa o queda medio mocaso\n"
				"Vamos agregarle un par de enters tambien\n\n\nAca tamos";
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
    Ui::ConfigWindowTest1Class ui;
};

#endif // CONFIGWINDOWTEST1_H

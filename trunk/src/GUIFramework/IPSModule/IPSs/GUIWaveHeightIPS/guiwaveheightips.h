#ifndef GUIWAVEHEIGHTIPS_H
#define GUIWAVEHEIGHTIPS_H

#include <QtGui/QWidget>
#include "ui_guiwaveheightips.h"

#include <fstream>
#include <auto_ptr.h>

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "WaveHeightIPS.h"
#include "Timestamp.h"
#include "Frame.h"
#include "GUIUtils.h"

#include "guiimageprocessingsystem.h"


class GUIWaveHeightIPS : public GUIImageProcessingSystem
{
    Q_OBJECT

public:
    GUIWaveHeightIPS(QWidget *parent = 0,  int windowW = 800, int windowH = 600);
    ~GUIWaveHeightIPS();

    /* Returns the name of the IPS */
    virtual const std::string &getName(void) const;

    /* Returns the info of the IPS */
    virtual const std::string &getInfo(void) const;


    /* Initialize the Image Processing System.
	 * Returns errorCode
	 */
	virtual errCode initialize(void);

	/* Executes the IPS and blocks until it finish.
	 * Returns:
	 * 	errCode
	 */
	virtual errCode execute(void);

private:
	Ui::GUIWaveHeightIPSClass ui;
};

#endif // GUIWAVEHEIGHTIPS_H

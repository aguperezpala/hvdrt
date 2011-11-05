#ifndef GUIWAVEHEIGHTIPS_H
#define GUIWAVEHEIGHTIPS_H

#include <QtGui/QWidget>
#include "ui_guiwaveheightips.h"

#include "DebugUtil.h"
#include "GlobalDefines.h"
#include "WaveHeightIPS.h"
#include "guiimageprocessingsystem.h"
#include "imagegeneratorconfigurator.h"

class GUIWaveHeightIPS : public GUIImageProcessingSystem
{
    Q_OBJECT

public:
    GUIWaveHeightIPS(QWidget *parent = 0);
    ~GUIWaveHeightIPS();

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

    // The real IPS
    WaveHeightIPS				mWaveHIPS;
    ImageGeneratorConfigurator	mImgGenWin;


};

#endif // GUIWAVEHEIGHTIPS_H

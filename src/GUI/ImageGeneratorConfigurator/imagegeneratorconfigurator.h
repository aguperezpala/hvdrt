#ifndef IMAGEGENERATORCONFIGURATOR_H
#define IMAGEGENERATORCONFIGURATOR_H

#include <QtGui/QWidget>
#include <qdialog.h>
#include "ui_imagegeneratorconfigurator.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>

#include <auto_ptr.h>

#include "DebugUtil.h"
#include "ImageGenerator.h"
#include "FrameListener.h"
#include "GUIConfiguratorDialog.h"

class ImageGeneratorConfigurator : public GUIConfiguratorDialog
{
    Q_OBJECT

public:
    ImageGeneratorConfigurator(QWidget *parent = 0);
    ~ImageGeneratorConfigurator();

    /* configures an ImageGenerator */
    void setImgGenerator(ImageGenerator *);

    /* Function used to load the configurations from a xml file
	 * Returns:
	 * 	errCode
	 */
	virtual errCode loadConfig(TiXmlElement *);

	/* Save the data to an xml and return it.
	 * Returns:
	 * 	0			on Error
	 * 	xml			on success */
	virtual std::auto_ptr<TiXmlElement> getConfig(void);


public slots:
    void onSourceCameraClicked(void);
    void onSourceFileClicked(void);
    void onSetParamettersClicked(void);
    void onGetParamettersClicked(void);
    void onDoneClicked(void);



private:



private:
    /* Function used to show the resolution of the ImageGenerator */
    void showResolution(void);

    /* set resolution */
    bool setResolution(double width, double height);

    /* get properties */
    void getProperties(void);

    /* Show input from source */
    void showSourceInput(void);


private:
    Ui::ImageGeneratorConfiguratorClass ui;

    ImageGenerator							*mImgGen;
    bool									mShowInfo;
    QImage									mShowImage;

    // Save properties
    std::string								mSourceStr;
    std::string								mTypeStr;
    std::auto_ptr<TiXmlElement>				mSaveXml;


};

#endif // IMAGEGENERATORCONFIGURATOR_H

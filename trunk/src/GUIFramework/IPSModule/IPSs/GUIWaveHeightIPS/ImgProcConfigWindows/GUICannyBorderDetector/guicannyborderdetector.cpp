#include "guicannyborderdetector.h"

#include <opencv2/core/core.hpp>

#include "DebugUtil.h"
#include "GUIUtils.h"


////////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::setCannyImgProcData(void)
{

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUICannyBorderDetector::GUICannyBorderDetector(ImageGenerator *ig, QWidget *parent)
    : ImgProcConfigWindowsImgProcConfigWindows(ig, parent, "GUICannyBorderDetector"),
      mCannyImgProc(0)
{
	ui.setupUi(this);

	// connect the signals
	QObject::connect(ui.threshole1_slider,SIGNAL(valueChanged(int)), this,
			SLOT(onSlide1Change(int)));
	QObject::connect(ui.threshole2_slider,SIGNAL(valueChanged(int)), this,
			SLOT(onSlide2Change(int)));

	QObject::connect(ui.cannyBuild_button,SIGNAL(clicked(bool)), this,
				SLOT(onButtonCannyPressed(void)));
	QObject::connect(ui.showReal_button,SIGNAL(clicked(bool)), this,
				SLOT(onButtonShowRealPressed(void)));

}

////////////////////////////////////////////////////////////////////////////////
GUICannyBorderDetector::~GUICannyBorderDetector()
{

}

////////////////////////////////////////////////////////////////////////////////
errCode GUICannyBorderDetector::loadConfig(const TiXmlElement *)
{
	// TODO:


}

////////////////////////////////////////////////////////////////////////////////
std::auto_ptr<TiXmlElement> GUICannyBorderDetector::getConfig(void) const
{
	// TODO:


}

////////////////////////////////////////////////////////////////////////////////
QString GUICannyBorderDetector::getInfo(void) const
{
	// TODO:

	return "NO INFO BY NOW"
}

////////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::windowVisible(void)
{
	// TODO:


}

////////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::windowInvisible(void)
{
	// TODO:


}

////////////////////////////////////////////////////////////////////////////////
errCode GUICannyBorderDetector::finish(QString &error)
{
	// TODO:


}


///////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::setCannyImgProcessor(ImageProcessor *ip)
{
	ASSERT(ip);
	if(ip->getName() != "CannyBorderDetector"){
		debug("Error setting the ip %s \n", ip->getName().c_str());
		return;
	}
	mCannyImgProc = ip;
}

///////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::onSlide1Change(int v)
{
	QString num;
	num.setNum(v);
	ui.threshole1_text->setText(num);
}

///////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::onSlide2Change(int v)
{
	QString num;
	num.setNum(v);
	ui.threshole2_text->setText(num);
}

///////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::onButtonCannyPressed(void)
{
	ASSERT(mCannyImgProc);
	if(mFrame.data.empty()){
		// get the img first
		mImgGenerator->captureFrame(mFrame);
	}

	// get the params
	bool ok = false;
	double th1 = ui.threshole1_text->text().toDouble(&ok);
	if(!ok){
		GUIUtils::showMessageBox("Threshole1 debe ser valor numerico");
		return;
	}

	double th2 = ui.threshole2_text->text().toDouble(&ok);
	if(!ok){
		GUIUtils::showMessageBox("Threshole2 debe ser valor numerico");
		return;
	}

	// set the values to the ImgProcessor

	// work on a copy of the image
	cv::Mat aux = mFrame.data.clone();
	errCode err = mCannyImgProc->processData(aux);
	if(err != NO_ERROR){
		GUIUtils::showMessageBox("Error al intentar aplicar el algoritmo: " + QString::number(err));
		return;
	}

	// now
	mThreshold1 = th1;
	mThreshold2 = th2;
	mL2Gradient = ui.l2Gradient_checkBox->isChecked();

	showImage(mTransformedImage);
}

///////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::onButtonShowRealPressed(void)
{
	showImage(mOriginalImage);
}




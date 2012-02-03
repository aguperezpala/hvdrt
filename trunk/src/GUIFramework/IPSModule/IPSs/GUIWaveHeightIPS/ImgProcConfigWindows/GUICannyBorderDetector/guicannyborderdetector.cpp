#include "guicannyborderdetector.h"

#include <opencv2/core/core.hpp>

#include "DebugUtil.h"
#include "GUIUtils.h"
#include "CannyBorderDetector.h"


////////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::setCannyImgProcData(void)
{
	ASSERT(mCannyImgProc);

	errCode err = mCannyImgProc->setParameter(CannyBorderDetector::THRESHOLD_1,
			ui.threshole1_text->text().toDouble(0));
	ASSERT(err == NO_ERROR);

	err = mCannyImgProc->setParameter(CannyBorderDetector::THRESHOLD_2,
			ui.threshole2_text->text().toDouble(0));
	ASSERT(err == NO_ERROR);

	err = mCannyImgProc->setParameter(CannyBorderDetector::L2_GRADIENT,
			ui.l2Gradient_checkBox->isChecked());
	ASSERT(err == NO_ERROR);
}

////////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::showImage(const cv::Mat &img)
{
	if(img.empty()){
		return;
	}
	QImage qImg;
	GUIUtils::IplImage2QImage(img, qImg);

	ui.image_label->setScaledContents(true);

	ui.image_label->setPixmap(QPixmap::fromImage(qImg));
	ui.image_label->setScaledContents(true);

}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUICannyBorderDetector::GUICannyBorderDetector(ImageGenerator *ig, QWidget *parent)
    : ImgProcConfigWindows(ig, parent, "GUICannyBorderDetector"),
      mCannyImgProc(0),
      mPerspectiveRectImgProc(0)
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
errCode GUICannyBorderDetector::loadConfig(const TiXmlElement *elem)
{
	ASSERT(elem);

	const TiXmlElement *auxElem = elem->FirstChildElement("GUICannyBorderDetector");
	if(!auxElem){
		debug("Invalid xml, GUICannyBorderDetector not found\n");
		GUIUtils::showMessageBox("XML Invalido, GUICannyBorderDetector no encontrado");
		return INVALID_PARAM;
	}

	// check L2Gradient
	auxElem = auxElem->FirstChildElement("L2Gradient");
	if(!auxElem){
		debug("Invalid xml, L2Gradient not found\n");
		GUIUtils::showMessageBox("XML Invalido, L2Gradient no encontrado");
		return INVALID_PARAM;
	}
	QString checked = auxElem->Attribute("checked");
	if(checked == "true"){
		ui.l2Gradient_checkBox->setChecked(true);
	} else {
		ui.l2Gradient_checkBox->setChecked(false);
	}

	auxElem = auxElem->NextSiblingElement("Threshold1");
	if(!auxElem){
		debug("Invalid xml, Threshold1 not found\n");
		GUIUtils::showMessageBox("XML Invalido, Threshold1 no encontrado");
		return INVALID_PARAM;
	}
	QString t1Str = auxElem->Attribute("value");
	ui.threshole1_text->setText(t1Str);

	auxElem = auxElem->NextSiblingElement("Threshold2");
	if(!auxElem){
		debug("Invalid xml, Threshold2 not found\n");
		GUIUtils::showMessageBox("XML Invalido, Threshold2 no encontrado");
		return INVALID_PARAM;
	}
	QString t2Str = auxElem->Attribute("value");
	ui.threshole2_text->setText(t2Str);

	bool ok;
	ui.threshole1_slider->setSliderPosition(t1Str.toInt(&ok));
	ui.threshole2_slider->setSliderPosition(t2Str.toInt(&ok));

	// sets the configuration to the img proc
	setCannyImgProcData();

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
std::auto_ptr<TiXmlElement> GUICannyBorderDetector::getConfig(void) const
{
	std::auto_ptr<TiXmlElement> result(new TiXmlElement("GUICannyBorderDetector"));

	TiXmlElement *gradient = new TiXmlElement("L2Gradient");
	if(ui.l2Gradient_checkBox->isChecked()){
		gradient->SetAttribute("checked", "true");
	} else {
		gradient->SetAttribute("checked", "false");
	}
	result->LinkEndChild(gradient);

	TiXmlElement *th1 = new TiXmlElement("Threshold1");
	th1->SetAttribute("value", ui.threshole1_text->text().toAscii().data());
	result->LinkEndChild(th1);

	TiXmlElement *th2 = new TiXmlElement("Threshold2");
	th2->SetAttribute("value", ui.threshole2_text->text().toAscii().data());
	result->LinkEndChild(th2);


	return result;
}

////////////////////////////////////////////////////////////////////////////////
QString GUICannyBorderDetector::getInfo(void) const
{

	return "Seleccione los distintos posibles valores del algoritmo \"Canny\" "
			"utilizado para la deteccion de contornos. Utilice el boton "
			"\"Aplicar Canny\" para ver la imagen luego de ser aplicado"
			" el alogirtmo.";
}

////////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::windowVisible(void)
{
	bool ok;
	// set the slides in the correct position
	ui.threshole1_slider->setValue(ui.threshole1_text->text().toInt(&ok));
	ui.threshole2_slider->setValue(ui.threshole2_text->text().toInt(&ok));

	// always get a new fresh image
	mImgGenerator->captureFrame(mFrame);

	// apply the transformation
	ASSERT(mPerspectiveRectImgProc);
	mPerspectiveRectImgProc->processData(mFrame.data);

	showImage(mFrame.data);
}

////////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::windowInvisible(void)
{
	// TODO:


}

////////////////////////////////////////////////////////////////////////////////
errCode GUICannyBorderDetector::finish(QString &error)
{
	// set the values to the image proc
	setCannyImgProcData();

	return NO_ERROR;
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
void GUICannyBorderDetector::setPerspectiveRectifierIP(ImageProcessor *ip)
{
	ASSERT(ip);
	mPerspectiveRectImgProc = ip;
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
	ASSERT(mPerspectiveRectImgProc);


	if(mFrame.data.empty()){
		// get the img first
		mImgGenerator->captureFrame(mFrame);

		// apply the perspective transformation
		errCode err = mPerspectiveRectImgProc->processData(mFrame.data);
		if(err != NO_ERROR){
			GUIUtils::showMessageBox("Error al intentar aplicar el transformador de"
					" perspectiva: " + QString::number(err));
			return;
		}

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
	setCannyImgProcData();

	// work on a copy of the image
	cv::Mat aux = mFrame.data.clone();

	errCode err = mCannyImgProc->processData(aux);
	if(err != NO_ERROR){
		GUIUtils::showMessageBox("Error al intentar aplicar el algoritmo: " + QString::number(err));
		return;
	}

	showImage(aux);
}

///////////////////////////////////////////////////////////////////////////////
void GUICannyBorderDetector::onButtonShowRealPressed(void)
{
	showImage(mFrame.data);
}




#include "guimiddlepointclipping.h"

#include "DebugUtil.h"
#include "GUIUtils.h"
#include "MiddlePointClipping.h"
#include "WaveHeightAnalyzer.h"


////////////////////////////////////////////////////////////////////////////////
void GUIMiddlePointClipping::refreshFrame(void)
{
	if(!mFrame.data.empty()){
		return;
	}

	errCode err = mImgGenerator->captureFrame(mFrame);
	ASSERT(err == NO_ERROR);
	// apply the perspective
	err = mPerspectiveIP->processData(mFrame.data);
	ASSERT(err == NO_ERROR);
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIMiddlePointClipping::saveMiddlePointClippingData(void)
{
	ASSERT(mMiddlePointCIP);

	// get the point
	const std::vector<QPoint> &points = mZoomedLabel.getPoints();
	ASSERT(points.size() == 1);

	QPoint p = points.front();
	errCode err = mMiddlePointCIP->setParameter(MiddlePointClipping::MIDDLE_POINT_X,
			p.x());
	ASSERT(err == NO_ERROR);

	bool ok;
	err = mMiddlePointCIP->setParameter(MiddlePointClipping::NUM_COLUMNS,
			ui.numColsLineEdit->text().toInt(&ok));
	if(!ok){
		GUIUtils::showMessageBox("El numero de columnas debe ser un valor numerico");
		return INCOMPLETE_CONFIGURATION;
	}
	ASSERT(err == NO_ERROR);

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIMiddlePointClipping::saveWaveHeightAnalyzerData(void)
{
	ASSERT(mWaveHeightAIP);

	// get the point
	const std::vector<QPoint> &points = mZoomedLabel.getPoints();
	ASSERT(points.size() == 1);

	QPoint p = points.front();

	// set the middle point
	errCode err = mWaveHeightAIP->setParameter(WaveHeightAnalyzer::POS_WAVE_REPOSE,
			p.y());
	ASSERT(err == NO_ERROR);

	// set the analyze zone to be all the img after been cut by the clipping IP
	bool ok;
	double imgSizeX = ui.numColsLineEdit->text().toInt(&ok);
	if(!ok){
		GUIUtils::showMessageBox("El numero de columnas debe ser un valor numerico");
		return INCOMPLETE_CONFIGURATION;
	}
	double imgSizeY = mFrame.data.rows;

	err = mWaveHeightAIP->setParameter(WaveHeightAnalyzer::TLX_ZONE, 0);
	ASSERT(err == NO_ERROR);

	err = mWaveHeightAIP->setParameter(WaveHeightAnalyzer::TLY_ZONE, 0);
	ASSERT(err == NO_ERROR);

	err = mWaveHeightAIP->setParameter(WaveHeightAnalyzer::BRX_ZONE, imgSizeX);
	ASSERT(err == NO_ERROR);

	err = mWaveHeightAIP->setParameter(WaveHeightAnalyzer::BRY_ZONE, imgSizeY);
	ASSERT(err == NO_ERROR);

	return NO_ERROR;

}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
GUIMiddlePointClipping::GUIMiddlePointClipping(ImageGenerator *ig, QWidget *parent)
    :  ImgProcConfigWindows(ig, parent, "GUIMiddlePointClipping"),
       mZoomedLabel(this),
       mPerspectiveIP(0),
       mMiddlePointCIP(0),
       mWaveHeightAIP(0)
{
	ui.setupUi(this);
	mZoomedLabel.clearPoints();
	mZoomedLabel.setLabelX(ui.xPosLabel);
	mZoomedLabel.setLabelY(ui.yPosLabel);
	mZoomedLabel.setZoomLabel(ui.zoomLabel);
	mZoomedLabel.setMaxNumberPoints(1);
	ui.scrollArea->setWidget(&mZoomedLabel);

	QObject::connect(ui.clearPointsButton,SIGNAL(clicked(bool)), this,
					SLOT(onClearClicked(void)));
}

////////////////////////////////////////////////////////////////////////////////
GUIMiddlePointClipping::~GUIMiddlePointClipping()
{

}

////////////////////////////////////////////////////////////////////////////////
errCode GUIMiddlePointClipping::loadConfig(const TiXmlElement *elem)
{
	ASSERT(elem);

	const TiXmlElement *auxElem = elem->FirstChildElement("GUIMiddlePointClipping");
	if(!auxElem){
		debug("Invalid xml, CoordsInterpreterConfigurator not found\n");
		GUIUtils::showMessageBox("XML Invalido, GUIMiddlePointClipping no fue encontrado");
		return INVALID_PARAM;
	}

	refreshFrame();
	// if no image was set then we set one
	if(mZoomedLabel.getImage().isNull()){
		// set the img
		QImage img;
		GUIUtils::IplImage2QImage(mFrame.data, img);
		mZoomedLabel.setImage(img);
	}


	mZoomedLabel.clearPoints();

	// parse the middle point
	const TiXmlElement *element = auxElem->FirstChildElement("MiddlePoint");
	if(!element){
		debug("Invalid xml, Point not found\n");
		GUIUtils::showMessageBox("XML Invalido, MiddlePoint no encontrado");
		return INVALID_PARAM;
	}
	QString xStr = element->Attribute("x");
	QString yStr = element->Attribute("y");
	bool ok;
	int x = xStr.toInt(&ok);
	int y = yStr.toInt(&ok);

	mZoomedLabel.addPoint(x,y);

	// parse the num of columns
	element = auxElem->FirstChildElement("NumColumns");
	if(!element){
		debug("Invalid xml, NumColumns not found\n");
		GUIUtils::showMessageBox("XML Invalido, NumColumns no encontrado");
		return INVALID_PARAM;
	}
	QString cols = element->Attribute("value");
	ui.numColsLineEdit->setText(cols);

	// set the values to the
	QString error;

	return finish(error);
}


////////////////////////////////////////////////////////////////////////////////
std::auto_ptr<TiXmlElement> GUIMiddlePointClipping::getConfig(void) const
{
	const std::vector<QPoint> &points = mZoomedLabel.getPoints();
	if(points.size() != 1){
		return std::auto_ptr<TiXmlElement>(0);
	}
	QPoint p = points.front();

	std::auto_ptr<TiXmlElement> result(new TiXmlElement("GUIMiddlePointClipping"));

	// save the middle point
	TiXmlElement *point = new TiXmlElement("MiddlePoint");
	QString xStr = QString::number(p.x());
	QString yStr = QString::number(p.y());
	point->SetAttribute("x", xStr.toAscii().data());
	point->SetAttribute("y", yStr.toAscii().data());

	result->LinkEndChild(point);

	// set the num of columns
	TiXmlElement *cols = new TiXmlElement("NumColumns");
	cols->SetAttribute("value", ui.numColsLineEdit->text().toAscii().data());


	result->LinkEndChild(cols);

	return result;
}


////////////////////////////////////////////////////////////////////////////////
QString GUIMiddlePointClipping::getInfo(void) const
{
	return "Seleccione el punto correspondiente a la altura de ola en reposo. "
			"Seleccione adicionalmente la cantidad de columnas que se quieren"
			" analizar (preferentemente 5).";
}


////////////////////////////////////////////////////////////////////////////////
void GUIMiddlePointClipping::windowVisible(void)
{
	ASSERT(mMiddlePointCIP);
	ASSERT(mPerspectiveIP);
	ASSERT(mWaveHeightAIP);

	errCode err = mImgGenerator->captureFrame(mFrame);
	ASSERT(err == NO_ERROR);
	// apply the perspective
	err = mPerspectiveIP->processData(mFrame.data);
	ASSERT(err == NO_ERROR);

//	// check if we have some image set
//	if(!mZoomedLabel.getImage().isNull()){
//		// we already have some image, return
//		return;
//	}
	// set the img
	QImage img;
	GUIUtils::IplImage2QImage(mFrame.data, img);
	mZoomedLabel.setImage(img);

	mZoomedLabel.drawPoints();

}


////////////////////////////////////////////////////////////////////////////////
errCode GUIMiddlePointClipping::finish(QString &error)
{
	refreshFrame();
	const std::vector<QPoint> &points = mZoomedLabel.getPoints();
	if(points.size() != 1){
		error = "Hemos marcado " + QString::number(points.size()) + " y solo"
				" debemos marcar 1 solo";
				return INCOMPLETE_CONFIGURATION;
	}

	// save the middle point
	errCode err = saveMiddlePointClippingData();
	if(err != NO_ERROR){
		error = "Error seteando el punto medio para el clipping";
		return err;
	}
	err = saveWaveHeightAnalyzerData();
	if(err != NO_ERROR){
		error = "Error seteando la info al WaveHeightAnalyzer";
		return err;
	}

	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
void GUIMiddlePointClipping::onClearClicked(void)
{
	mZoomedLabel.clearPoints();
}


////////////////////////////////////////////////////////////////////////////////
void GUIMiddlePointClipping::setPerspectiveRectifierIP(ImageProcessor *ip)
{
	ASSERT(ip);

	mPerspectiveIP = ip;
}

////////////////////////////////////////////////////////////////////////////////
void GUIMiddlePointClipping::setMiddlePointClippingIP(ImageProcessor *ip)
{
	ASSERT(ip);
	ASSERT(ip->getName() == "MiddlePointClipping");

	mMiddlePointCIP = ip;
}

////////////////////////////////////////////////////////////////////////////////
void GUIMiddlePointClipping::setWaveHeightAnalyzerIP(ImageProcessor *ip)
{
	ASSERT(ip);
	ASSERT(ip->getName() == "WaveHeightAnalyzer");

	mWaveHeightAIP = ip;
}




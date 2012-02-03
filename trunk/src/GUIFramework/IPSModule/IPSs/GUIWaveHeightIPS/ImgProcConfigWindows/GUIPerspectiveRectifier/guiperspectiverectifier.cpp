#include <qpoint.h>

#include <algorithm>


#include "guiperspectiverectifier.h"
#include "PerspectiveRectifier.h"
#include "WaveHeightAnalyzer.h"
#include "GUIUtils.h"

#define CHECK_IMG_PROC_ERR(x)	if(x != NO_ERROR){debug("Error here\n"); return x;}


static bool sortFunction(const QPoint &p1, const QPoint &p2)
{
	return (p1.x() < p2.x());
}

////////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveRectifier::sortPoints(const std::vector<QPoint> &input,
		std::vector<QPoint> &output) const
{
	ASSERT(input.size() == 4);
	output.clear();

	std::vector<QPoint> aux = input;
	std::sort(aux.begin(), aux.end(), sortFunction);

	// now we have ordered the points from the most left to the most right
	// the first two are the tl and bl (depending on the Y), and the thirthd and
	// fourth are the br and tr.
	QPoint *tl, *bl, *tr, *br;

	if(aux[0].y() < aux[1].y()) {
		tl = &(aux[0]);
		bl = &(aux[1]);
	} else {
		tl = &(aux[1]);
		bl = &(aux[0]);
	}

	if(aux[2].y() < aux[3].y()){
		tr = &(aux[2]);
		br = &(aux[3]);
	} else {
		tr = &(aux[3]);
		br = &(aux[2]);
	}

	// push the points in the correct oreder TopLeft, TopRight, BottomLeft, BottomRight
	output.push_back(*tl);
	output.push_back(*tr);
	output.push_back(*bl);
	output.push_back(*br);
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIPerspectiveRectifier::setPointsToIP(void)
{
	const std::vector<QPoint> &points = mZoomedLabel.getPoints();
	if(!mImgProc || points.size() != 4){
		return INCOMPLETE_CONFIGURATION;
	}

	// sort and set the points
	std::vector<QPoint> sortedPoints;
	sortPoints(points, sortedPoints);
	errCode err = NO_ERROR;

	err = mImgProc->setParameter(PerspectiveRectifier::TLX_POINT_SRC,
			sortedPoints[0].x());
	CHECK_IMG_PROC_ERR(err);
	err = mImgProc->setParameter(PerspectiveRectifier::TLY_POINT_SRC,
			sortedPoints[0].y());
	CHECK_IMG_PROC_ERR(err);

	err = mImgProc->setParameter(PerspectiveRectifier::TRX_POINT_SRC,
			sortedPoints[1].x());
	CHECK_IMG_PROC_ERR(err);
	err = mImgProc->setParameter(PerspectiveRectifier::TRY_POINT_SRC,
			sortedPoints[1].y());
	CHECK_IMG_PROC_ERR(err);

	err = mImgProc->setParameter(PerspectiveRectifier::BLX_POINT_SRC,
			sortedPoints[2].x());
	CHECK_IMG_PROC_ERR(err);
	err = mImgProc->setParameter(PerspectiveRectifier::BLY_POINT_SRC,
			sortedPoints[2].y());
	CHECK_IMG_PROC_ERR(err);

	err = mImgProc->setParameter(PerspectiveRectifier::BRX_POINT_SRC,
			sortedPoints[3].x());
	CHECK_IMG_PROC_ERR(err);
	err = mImgProc->setParameter(PerspectiveRectifier::BRY_POINT_SRC,
			sortedPoints[3].y());
	CHECK_IMG_PROC_ERR(err);

	err = mImgProc->setParameter(PerspectiveRectifier::TLX_POINT_DST,
			0);
	CHECK_IMG_PROC_ERR(err);
	err = mImgProc->setParameter(PerspectiveRectifier::TLY_POINT_DST,
			0);
	CHECK_IMG_PROC_ERR(err);

	err = mImgProc->setParameter(PerspectiveRectifier::TRX_POINT_DST,
			mImgSizeX);
	CHECK_IMG_PROC_ERR(err);
	err = mImgProc->setParameter(PerspectiveRectifier::TRY_POINT_DST,
			0);
	CHECK_IMG_PROC_ERR(err);

	err = mImgProc->setParameter(PerspectiveRectifier::BLX_POINT_DST,
			0);
	CHECK_IMG_PROC_ERR(err);
	err = mImgProc->setParameter(PerspectiveRectifier::BLY_POINT_DST,
			mImgSizeY);
	CHECK_IMG_PROC_ERR(err);

	err = mImgProc->setParameter(PerspectiveRectifier::BRX_POINT_DST,
			mImgSizeX);
	CHECK_IMG_PROC_ERR(err);
	err = mImgProc->setParameter(PerspectiveRectifier::BRY_POINT_DST,
			mImgSizeY);
	CHECK_IMG_PROC_ERR(err);

	err = mImgProc->setParameter(PerspectiveRectifier::SET_POINTS, 1);
	CHECK_IMG_PROC_ERR(err);


	return NO_ERROR;
}

////////////////////////////////////////////////////////////////////////////////
errCode GUIPerspectiveRectifier::setRectangleSizeToIP(void)
{
	ASSERT(mWaveHAnalyzerIP);

	// get the size input by the user
	bool ok = false;
	double ySize = ui.rectHeightLineEdit->text().toDouble(&ok);
	if(!ok){
		GUIUtils::showMessageBox("La altura del rectangulo debe ser un numero");
		return INCOMPLETE_CONFIGURATION;
	}

	// now we have the size of the image and we want the
	// SIZE_RELATION,	// the relation between pixels and millimeters (pixels/m)
	double relation = static_cast<double>(mImgSizeY) / ySize;

	return mWaveHAnalyzerIP->setParameter(WaveHeightAnalyzer::SIZE_RELATION, relation);
}


////////////////////////////////////////////////////////////////////////////////
double GUIPerspectiveRectifier::getValue(const TiXmlElement *elem, const char *attr, bool *ok)
{
	if(!elem || !attr){
		return -1.0;
	}

	double result = 0.0;
	const char *value = elem->Attribute(attr);
	if(value){
		QString val = value;
		result = val.toDouble(ok);
	}
	return result;
}

////////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveRectifier::setValue(TiXmlElement *elem, const char *child,
		const char *attr, double value) const
{
	ASSERT(elem);
	ASSERT(child);
	ASSERT(attr);

	TiXmlElement *childElem = new TiXmlElement(child);
	QString num = QString::number(value);
	childElem->SetAttribute(attr, num.toAscii().data());
	elem->LinkEndChild(childElem);

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
GUIPerspectiveRectifier::GUIPerspectiveRectifier(ImageGenerator *ig, QWidget *parent)
    : ImgProcConfigWindows(ig, parent, "GUIPerspectiveRectifier"),
      mZoomedLabel(this),
      mImgProc(0),
      mWaveHAnalyzerIP(0)
{
	ui.setupUi(this);

	mZoomedLabel.clearPoints();
	mZoomedLabel.setLabelX(ui.xPosLabel);
	mZoomedLabel.setLabelY(ui.yPosLabel);
	mZoomedLabel.setZoomLabel(ui.zoomLabel);
	mZoomedLabel.setMaxNumberPoints(4);
	ui.scrollArea->setWidget(&mZoomedLabel);

	QObject::connect(ui.clearPointsButton,SIGNAL(clicked(bool)), this,
					SLOT(onClearClicked(void)));
	QObject::connect(ui.previewButton,SIGNAL(clicked(bool)), this,
					SLOT(onPreviewClicked(void)));

}

////////////////////////////////////////////////////////////////////////////////
GUIPerspectiveRectifier::~GUIPerspectiveRectifier()
{

}

////////////////////////////////////////////////////////////////////////////////
errCode GUIPerspectiveRectifier::loadConfig(const TiXmlElement *elem)
{
	const TiXmlElement *auxElem = elem->FirstChildElement("GUIPerspectiveRectifier");
	if(!auxElem){
		debug("Invalid xml, GUIPerspectiveTransformator not found\n");
		GUIUtils::showMessageBox("XML invalido, no se encontro GUIPerspectiveRectifier");
		return INVALID_PARAM;
	}

	// get all the points of the rectangle
	bool ok;
	double value;
	int px, py;

	// clear all the points
	mZoomedLabel.clearPoints();

	// update the image if we have not img
	if(mZoomedLabel.getImage().isNull()){
		updateImg();
	}

	px = getValue(auxElem->FirstChildElement("TLX_POINT_SRC"), "value", &ok);
	py = getValue(auxElem->FirstChildElement("TLY_POINT_SRC"), "value", &ok);
	mZoomedLabel.addPoint(px,py);

	px = getValue(auxElem->FirstChildElement("TRX_POINT_SRC"), "value", &ok);
	py = getValue(auxElem->FirstChildElement("TRY_POINT_SRC"), "value", &ok);
	mZoomedLabel.addPoint(px,py);

	px = getValue(auxElem->FirstChildElement("BLX_POINT_SRC"), "value", &ok);
	py = getValue(auxElem->FirstChildElement("BLY_POINT_SRC"), "value", &ok);
	mZoomedLabel.addPoint(px,py);

	px = getValue(auxElem->FirstChildElement("BRX_POINT_SRC"), "value", &ok);
	py = getValue(auxElem->FirstChildElement("BRY_POINT_SRC"), "value", &ok);
	mZoomedLabel.addPoint(px,py);

	debug("mZoomedLabel points: %d\n", mZoomedLabel.getPoints().size());

	errCode err = setPointsToIP();
	if(err != NO_ERROR){
		return err;
	}

	debug("Setting rectangleSize\n");

	// get the rectangle size
	value = getValue(auxElem->FirstChildElement("RectangleSize"), "ySize", &ok);
	ui.rectHeightLineEdit->setText(QString::number(value));
	err = setRectangleSizeToIP();

	return err;
}


////////////////////////////////////////////////////////////////////////////////
std::auto_ptr<TiXmlElement> GUIPerspectiveRectifier::getConfig(void) const
{
	// save the points
	const std::vector<QPoint> &points = mZoomedLabel.getPoints();
	if(points.size() != 4){
		GUIUtils::showMessageBox("Cuidado: se esta guardando en el xml menos puntos"
				" de los necesarios para la configuracion.");
		return std::auto_ptr<TiXmlElement> (0);
	}

	std::auto_ptr<TiXmlElement> result(new TiXmlElement("GUIPerspectiveRectifier"));

	// add the points
	std::vector<QPoint> sortedPoints;
	sortPoints(points, sortedPoints);
	// push the points in the correct oreder TopLeft, TopRight, BottomLeft, BottomRight
	setValue(result.get(), "TLX_POINT_SRC", "value", sortedPoints[0].x());
	setValue(result.get(), "TLY_POINT_SRC", "value", sortedPoints[0].y());

	setValue(result.get(), "TRX_POINT_SRC", "value", sortedPoints[1].x());
	setValue(result.get(), "TRY_POINT_SRC", "value", sortedPoints[1].y());

	setValue(result.get(), "BLX_POINT_SRC", "value", sortedPoints[2].x());
	setValue(result.get(), "BLY_POINT_SRC", "value", sortedPoints[2].y());

	setValue(result.get(), "BRX_POINT_SRC", "value", sortedPoints[3].x());
	setValue(result.get(), "BRY_POINT_SRC", "value", sortedPoints[3].y());

	// rectangle
	bool ok;
	setValue(result.get(), "RectangleSize", "ySize",
			ui.rectHeightLineEdit->text().toDouble(&ok));

	return result;
}



////////////////////////////////////////////////////////////////////////////////
QString GUIPerspectiveRectifier::getInfo(void) const
{
	// TODO:
	return "NO INFO";
}

////////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveRectifier::windowVisible(void)
{
	// we always will get a new fresh image (this will solve the problem
	// when using camera)
	updateImg();
	if(!mZoomedLabel.getPoints().empty()){
		// show the points
		mZoomedLabel.drawPoints();
	}

	// update the image if and olny if we have no img
//	if(mFrame.data.empty()){
//		updateImg();
//	}
}


////////////////////////////////////////////////////////////////////////////////
errCode GUIPerspectiveRectifier::finish(QString &error)
{
	ASSERT(mImgProc);
	ASSERT(mWaveHAnalyzerIP);

	// check how many points we have
	const std::vector<QPoint> &points = mZoomedLabel.getPoints();
	if(points.size() != 4){
		error = "Hemos seleccionado " + QString::number(points.size()) +
				" y tenemos que seleccionar solo 4 puntos (los que forman "
				"el rectangulo)";
		return INCOMPLETE_CONFIGURATION;
	}

	// set the points to the ImageProcessor
	errCode result = setPointsToIP();
	if(result != NO_ERROR){
		error = "Se produjo un error seteando los puntos al Rectificador";
		return result;
	}

	// set the rectangle size
	if((result = setRectangleSizeToIP()) != NO_ERROR){
		error = "Se produjo un error intentando establecer la altura del rectangulo";
		return result;
	}

	// everything ok

	return NO_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveRectifier::updateImg(void)
{
	if(mImgGenerator->captureFrame(mFrame) != NO_ERROR){
		GUIUtils::showMessageBox("Error obteniendo imagen del ImageGenerator\n");
		return;
	}

	mImgSizeX = mFrame.data.cols;
	mImgSizeY = mFrame.data.rows;



	// we have the image, show it
	QImage qimg;
	GUIUtils::IplImage2QImage(mFrame.data, qimg);
	mZoomedLabel.setImage(qimg);

}

////////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveRectifier::setPerspectiveRectifierIP(ImageProcessor *ip)
{
	ASSERT(ip);
	if(ip->getName() != "PerspectiveRectifier"){
		debug("Error: Invalid ImageProcessor set\n");
		ASSERT(false);
		return;
	}

	mImgProc = ip;
}

////////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveRectifier::setWaveHeightAnalyzerIP(ImageProcessor *ip)
{
	ASSERT(ip);
	if(ip->getName() != "WaveHeightAnalyzer"){
		debug("Error: Invalid ImageProcessor set\n");
		ASSERT(false);
		return;
	}

	mWaveHAnalyzerIP = ip;
}



////////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveRectifier::onPreviewClicked(void)
{

	// check that we have the Image processor associated
	if(!mImgProc){
		GUIUtils::showMessageBox("Error: No hay un ImageProcessor seteado para "
				"una preview");
		return;
	}

	// else, create the preview and show it
	const std::vector<QPoint> &points = mZoomedLabel.getPoints();
	if(points.size() != 4){
		GUIUtils::showMessageBox("Hemos seleccionado " + QString::number(points.size()) +
				" y tenemos que seleccionar solo 4 puntos (los que forman "
				"el rectangulo)");
		return;
	}

	// set the points to the ImageProcessor
	errCode result = setPointsToIP();
	if(result != NO_ERROR){
		GUIUtils::showMessageBox("Se produjo un error seteando los puntos al Rectificador");
		return;
	}


	// we are ready to show the preview
	cv::Mat aux = mFrame.data.clone();
	cv::namedWindow("Perspective Preview");

	// apply the perspective transform
	if(mImgProc->processData(aux) != NO_ERROR){
		GUIUtils::showMessageBox("No se pudo aplicar la transformacion! :(");
		return;
	}

	cv::imshow("Perspective Preview", aux);
}

////////////////////////////////////////////////////////////////////////////////
void GUIPerspectiveRectifier::onClearClicked(void)
{
	mZoomedLabel.clearPoints();
}






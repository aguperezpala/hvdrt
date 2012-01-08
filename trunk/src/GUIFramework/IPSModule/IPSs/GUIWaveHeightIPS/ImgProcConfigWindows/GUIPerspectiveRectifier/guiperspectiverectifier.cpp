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
		std::vector<QPoint> &output)
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
	// SIZE_RELATION,	// the relation between pixels and millimeters (pixels/mm)
	double relation = static_cast<double>(mImgSizeY) / ySize;

	return mWaveHAnalyzerIP->setParameter(WaveHeightAnalyzer::SIZE_RELATION, relation);
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
errCode GUIPerspectiveRectifier::loadConfig(const TiXmlElement *)
{
	// TODO:

	return NO_ERROR;
}


////////////////////////////////////////////////////////////////////////////////
std::auto_ptr<TiXmlElement> GUIPerspectiveRectifier::getConfig(void) const
{
	// TODO:

	return std::auto_ptr<TiXmlElement>(0);
}



////////////////////////////////////////////////////////////////////////////////
QString GUIPerspectiveRectifier::getInfo(void) const
{
	// TODO:
	return "NO INFO";
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






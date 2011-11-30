#include "coordsinterpreterconfigurator.h"
#include "GUIUtils.h"

CoordsInterpreterConfigurator::CoordsInterpreterConfigurator(QWidget *parent)
    : GUIConfiguratorDialog(parent, "CoordsInterpreterConfigurator")
{
	ui.setupUi(this);
	mMouseLabel = new MouseQLabel(this);
	mMouseLabel->setObjectName(QString::fromUtf8("img_label"));
	mScrollArea.setWidget(mMouseLabel);
	ui.verticalLayout->insertWidget(0,&mScrollArea);
	mMouseLabel->setLabelX(ui.x_label);
	mMouseLabel->setLabelY(ui.y_label);
	mMouseLabel->setZoomLabel(ui.zoom_label);

	mMouseLabel->grabMouse();
	mMouseLabel->setMouseTracking(true);

	QObject::connect(ui.clear_button,SIGNAL(clicked(bool)), this,
				SLOT(onClearClicked(void)));
	QObject::connect(ui.done_button,SIGNAL(clicked(bool)), this,
					SLOT(onDoneClicked(void)));

}

CoordsInterpreterConfigurator::~CoordsInterpreterConfigurator()
{
	delete mMouseLabel;
}

///////////////////////////////////////////////////////////////////////////////
void CoordsInterpreterConfigurator::setImage(const cv::Mat &img)
{
	if(!mImage.empty()){
		mImage.release();
	}

	mImage = img.clone();
	QImage qimg;
	GUIUtils::IplImage2QImage(mImage, qimg);
	mMouseLabel->setImage(qimg);

}

///////////////////////////////////////////////////////////////////////////////
errCode CoordsInterpreterConfigurator::loadConfig(TiXmlElement *elem)
{
	ASSERT(elem);

	TiXmlElement *auxElem = elem->FirstChildElement("CoordsInterpreterConfigurator");
	if(!auxElem){
		debug("Invalid xml, CoordsInterpreterConfigurator not found\n");
		GUIUtils::showMessageBox("Invalid xml, CoordsInterpreterConfigurator not found");
		return INVALID_PARAM;
	}

	// we only have to parse the middle point
	auxElem = auxElem->FirstChildElement("Point");
	if(!auxElem){
		debug("Invalid xml, Point not found\n");
		GUIUtils::showMessageBox("Invalid xml, Point not found");
		return INVALID_PARAM;
	}
	QString xStr = auxElem->Attribute("x");
	QString yStr = auxElem->Attribute("y");
	bool ok;
	int x = xStr.toInt(&ok);
	int y = yStr.toInt(&ok);

	mMouseLabel->addPoint(x,y);

}

///////////////////////////////////////////////////////////////////////////////
std::auto_ptr<TiXmlElement> CoordsInterpreterConfigurator::getConfig(void)
{
	std::auto_ptr<TiXmlElement> result(new TiXmlElement("CoordsInterpreterConfigurator"));

	TiXmlElement *point = new TiXmlElement("Point");
	QString xStr = QString::number(mPoint.x);
	QString yStr = QString::number(mPoint.y);
	point->SetAttribute("x", xStr.toAscii().data());
	point->SetAttribute("y", yStr.toAscii().data());

	result->LinkEndChild(point);

	return result;
}

///////////////////////////////////////////////////////////////////////////////
void CoordsInterpreterConfigurator::onClearClicked(void)
{
	mMouseLabel->clearPoints();
}

///////////////////////////////////////////////////////////////////////////////
void CoordsInterpreterConfigurator::onDoneClicked(void)
{
	// check that we have 1 points
	if(mMouseLabel->getPoints().size() != 1){
		GUIUtils::showMessageBox("Error: We have to only 1 point, the middle point\n");
		return;
	}

	const std::vector<QPoint> &points = mMouseLabel->getPoints();
	ASSERT(points.size() == 1);
	mPoint.x = points[0].x();
	mPoint.y = points[0].y();

	// everything ok, emit the signal
	emit doneSignal(NO_ERROR);
}

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

	// everything ok, close the window
	close();
}

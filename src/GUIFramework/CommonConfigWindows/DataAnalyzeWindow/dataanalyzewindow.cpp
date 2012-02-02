#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qfile.h>
#include <qfiledialog.h>

#include "GUIUtils.h"
#include "DebugUtil.h"
#include "dataanalyzewindow.h"



////////////////////////////////////////////////////////////////////////////////
#ifdef DEBUG
void DataAnalyzeWindow::insertTestCurve(void)
{

	static QwtPlotCurve c,g;
	QVector<double> vx,vy;
	c.setPen(QPen(Qt::red));

	for(int i = 0; i < 100; ++i){
		vx.push_back(i);
		vy.push_back(i);
	}
	c.setSamples(vx, vy);
	c.attach(ui.mainqwtPlot);


	vx.clear();
	vy.clear();
	for(int i = 0; i < 100; ++i){
		vx.push_back(i);
		vy.push_back(i*i);
	}
	g.setPen(QPen(Qt::green));
	g.setSamples(vx, vy);
	g.attach(ui.mainqwtPlot);
}
#endif


////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::configurePlots(void)
{
	QwtPlot *mainPlot = ui.mainqwtPlot;
	QwtPlot *spectralPlot = ui.secondqwtPlot;

	// main plot
	mainPlot->setAxisTitle(QwtPlot::xBottom, "segundos");
	mainPlot->setAxisTitle(QwtPlot::yLeft, "metros");
	mainPlot->setAxisAutoScale(QwtPlot::xBottom, true);
	mainPlot->setAxisAutoScale(QwtPlot::yLeft, true);
	mMainGird.attach(mainPlot);

	 // panning with the left mouse button
	mPlanner1 = new QwtPlotPanner( mainPlot->canvas() );
	 // zoom in/out with the wheel
	mMagnifier1 = new QwtPlotMagnifier( mainPlot->canvas() );

	// Spectral Plot
	spectralPlot->setAxisTitle(QwtPlot::xBottom, "segundos");
	spectralPlot->setAxisTitle(QwtPlot::yLeft, "metros");
	mSecondGird.attach(spectralPlot);

	 // panning with the left mouse button
	mPlanner2 = new QwtPlotPanner( spectralPlot->canvas() );
	 // zoom in/out with the wheel
	mMagnifier2 = new QwtPlotMagnifier( spectralPlot->canvas() );


}

////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::showCorrectCheckboxs(void)
{
	// disable all the "datos" that we can't show
	switch(mCurves.size()){
	case 0:
		ui.datos1CheckBox->setEnabled(false);
		ui.datos2CheckBox->setEnabled(false);
		ui.datos3CheckBox->setEnabled(false);
		break;
	case 1:
		ui.datos1CheckBox->setEnabled(true);
		ui.datos2CheckBox->setEnabled(false);
		ui.datos3CheckBox->setEnabled(false);
		break;
	case 2:
		ui.datos1CheckBox->setEnabled(true);
		ui.datos2CheckBox->setEnabled(true);
		ui.datos3CheckBox->setEnabled(false);
		break;
	case 3:
		ui.datos1CheckBox->setEnabled(true);
		ui.datos2CheckBox->setEnabled(true);
		ui.datos3CheckBox->setEnabled(true);
		break;
	}
}


////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::fillmainPlot(CurveData &c)
{
	// we only show the curve
	c.getDataCurve().show();

}

void DataAnalyzeWindow::fillsecondPlot(CurveData &c)
{
	c.getSpectralCurve().show();
}

////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::fillAdditionalInfo(CurveData &c)
{
	QString h = QString::number(c.getH());
	QString hs = QString::number(c.getHs());
	QString tp = QString::number(c.getTp());

	ui.hsLabel->setText(hs);
	ui.hLabel->setText(h);
	ui.tpLabel->setText(tp);

}

////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::hideAllInfo(CurveData &c)
{
	c.getDataCurve().hide();
	c.getSpectralCurve().hide();

	if(getNumCheckboxSelected() == 1){
		// remove the label info
		QString str0 = QString::number(0);
		ui.hsLabel->setText(str0);
		ui.hLabel->setText(str0);
		ui.tpLabel->setText(str0);
	}
}

////////////////////////////////////////////////////////////////////////////////
int DataAnalyzeWindow::getNumCheckboxSelected(void) const
{
	int count = 0;
	if(ui.datos1CheckBox->isChecked()) ++count;
	if(ui.datos2CheckBox->isChecked()) ++count;
	if(ui.datos3CheckBox->isChecked()) ++count;

	return count;
}

////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::initializeCurveData(CurveData &c)
{
	// attach the curves to the plot's
	QwtPlot *mainPlot = ui.mainqwtPlot;
	QwtPlot *spectralPlot = ui.secondqwtPlot;

	c.getDataCurve().detach();
	c.getDataCurve().attach(mainPlot);
	c.getDataCurve().hide();

	c.getSpectralCurve().detach();
	c.getSpectralCurve().attach(spectralPlot);
	c.getSpectralCurve().hide();
}

////////////////////////////////////////////////////////////////////////////////
bool DataAnalyzeWindow::parseFile(const QString &fname, QVector<double> &xs,
  		QVector<double> &ys)
{
	QFile file(fname);

	file.open(QFile::ReadOnly);

	if(!file.isOpen()){
		GUIUtils::showMessageBox("Error al abrir el archivo " + fname);
		return false;
	}

	// we will parse the file depending of the extension of the file
	int index = fname.lastIndexOf(QChar('.'));
	if(index < 0){
		GUIUtils::showMessageBox("Formato de archivo desconocido");
		return false;
	}

	// get all the data we need to work with the file
	int skipFirstnthLines = 0, timeOffset = 0, heightOffset = 0;

	QString ext = fname.mid(index+1, fname.size()-index);
	if(ext == "csv"){
		skipFirstnthLines = 2;
		timeOffset = 3;
		heightOffset = 4;
	} else if(ext == "agu"){
		skipFirstnthLines = 1;
		timeOffset = 0;
		heightOffset = 1;
	} else {
		GUIUtils::showMessageBox("Formato " + ext + " desconocido");
		return false;
	}

	char auxBuff[512];
	// skip first lines
	for(int i = 0; i < skipFirstnthLines; ++i){
		file.readLine(auxBuff, 511);
	}

	xs.clear();
	ys.clear();

	bool ok;
	// now we read line per line and parse the data
	while(file.canReadLine()){
		if(file.readLine(auxBuff, 511) < 0){
			break;
		}
		// split the string
		QString auxData(auxBuff);
		QStringList values = auxData.split(QChar(','));
		if((values.size() <= timeOffset) ||
				(values.size() <= heightOffset)){
			GUIUtils::showMessageBox("Error al parsear el archivo " + fname +
					". Cantidad incorrecta de columnas");
			return false;
		}

		double time = values[timeOffset].toDouble(&ok);
		if(!ok){
			GUIUtils::showMessageBox("Error al parsear \"time\" del archivo " + fname);
			return false;
		}
		double height = values[heightOffset].toDouble(&ok);
		if(!ok){
			GUIUtils::showMessageBox("Error al parsear \"height\" del archivo " + fname);
			return false;
		}

		// else everything ok, put it in the vectors
		xs.push_back(time);
		ys.push_back(height);
	}

	// everything is ok
	return true;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
DataAnalyzeWindow::DataAnalyzeWindow(QWidget *parent)
    : QWidget(parent),
      mPlanner1(0),
      mMagnifier1(0)
{
	ui.setupUi(this);

	// Connect all the buttons
	QObject::connect(ui.loadFileButton,SIGNAL(clicked(bool)), this,
			SLOT(onLoadFileClicked(void)));
	QObject::connect(ui.datos1CheckBox,SIGNAL(clicked(bool)), this,
			SLOT(onCheckbox1Clicked(bool)));
	QObject::connect(ui.datos2CheckBox,SIGNAL(clicked(bool)), this,
			SLOT(onCheckbox2Clicked(bool)));
	QObject::connect(ui.datos3CheckBox,SIGNAL(clicked(bool)), this,
			SLOT(onCheckbox3Clicked(bool)));

	// configure the plots
	configurePlots();


	showCorrectCheckboxs();

#ifdef DEBUG
//	insertTestCurve();
#endif
}

////////////////////////////////////////////////////////////////////////////////
DataAnalyzeWindow::~DataAnalyzeWindow()
{
	delete mPlanner1;
	delete mPlanner2;
	delete mMagnifier1;
	delete mMagnifier2;

	for(int i = mCurves.size()-1; i >= 0; --i) delete mCurves[i];

}

////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::onLoadFileClicked(void)
{
	// check if we can load a new data
	if(mCurves.size() >= 3){
		GUIUtils::showMessageBox("Error: Solo se pueden cargar hasta 3 datos");
		return;
	}

	QString filename = QFileDialog::getOpenFileName(0, "Cargar datos", ".",
			"*.agu *.csv");
	if(filename.isEmpty()){
		return;
	}

	// check if we can parse it
	QVector<double> xs, ys;
	if(!parseFile(filename,xs, ys)){
		return;
	}

	// we could parse it, create a new curve
	CurveData *curve = new CurveData;
	initializeCurveData(*curve);
	mCurves.push_back(curve);
	curve->loadData(xs, ys);

	// now let available the label
	showCorrectCheckboxs();

	// if is the first curve, then we show in all the plotters and info
	if(mCurves.size() == 1){
		ui.datos1CheckBox->setChecked(true);
		// show the curve
		fillAdditionalInfo(*curve);
		fillmainPlot(*curve);
		fillsecondPlot(*curve);
	}


}

////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::onCheckbox1Clicked(bool c)
{
	ASSERT(mCurves.size() >= 1);

	if(c){
		if(getNumCheckboxSelected() == 1){
			// we have to show all the info
			fillAdditionalInfo(*(mCurves[0]));
		}
		fillmainPlot(*(mCurves[0]));
		fillsecondPlot(*(mCurves[0]));
	} else {
		// hide all
		hideAllInfo(*(mCurves[0]));

	}
}


////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::onCheckbox2Clicked(bool c)
{
	ASSERT(mCurves.size() >= 2);

	if(c){
		if(getNumCheckboxSelected() == 1){
			// we have to show all the info
			fillAdditionalInfo(*(mCurves[1]));
		}
		fillmainPlot(*(mCurves[1]));
		fillsecondPlot(*(mCurves[1]));
	} else {
		// hide all
		hideAllInfo(*(mCurves[1]));

	}
}

////////////////////////////////////////////////////////////////////////////////
void DataAnalyzeWindow::onCheckbox3Clicked(bool c)
{
	ASSERT(mCurves.size() >= 3);


	if(c){
		if(getNumCheckboxSelected() == 1){
			// we have to show all the info
			fillAdditionalInfo(*(mCurves[2]));
		}
		fillmainPlot(*(mCurves[2]));
		fillsecondPlot(*(mCurves[2]));
	} else {
		// hide all
		hideAllInfo(*(mCurves[2]));

	}
}


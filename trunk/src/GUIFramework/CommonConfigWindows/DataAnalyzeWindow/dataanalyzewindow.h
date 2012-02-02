#ifndef DATAANALYZEWINDOW_H
#define DATAANALYZEWINDOW_H

#include <QtGui/QWidget>
#include <qwt_plot_grid.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>

#include "CurveData.h"

#include "ui_dataanalyzewindow.h"

class DataAnalyzeWindow : public QWidget
{
    Q_OBJECT

public:
    DataAnalyzeWindow(QWidget *parent = 0);
    ~DataAnalyzeWindow();


    // Load a curve from file
    void loadFile(const QString &filename);

    // Clear all
    void clear(void);

private slots:
	void onLoadFileClicked(void);
	void onCheckbox1Clicked(bool);
	void onCheckbox2Clicked(bool);
	void onCheckbox3Clicked(bool);

private:
#ifdef DEBUG
    void insertTestCurve(void);
#endif


private:
    // configure MainPlot/SecondPlot
    void configurePlots(void);

    // get min and max values of all the curves of the axis
    void getMaxAndMinAxisValues(double &maxX, double &minX, double &maxY,
    		double &minY);

    // auto adjust plotters scales
    void adjustPlottersScales(void);

    // enable/disable checkboxs
    void showCorrectCheckboxs(void);

    // fill the plotters with a certain curve
    void fillmainPlot(CurveData &c);
    void fillsecondPlot(CurveData &c);

    // fill additional info from a certain curvedata
    void fillAdditionalInfo(CurveData &c);

    // hide all the curve info from the window
    void hideAllInfo(CurveData &c);

    // return the number of checkBox selected
    int getNumCheckboxSelected(void) const;

    // initialize a CurveData
    void initializeCurveData(CurveData &c);

    // load a curve from file
    void loadCurveFromFile(const QString &filename);

    // Parse a file, this function should return two vectors, one of time
    // and other of wave height. All files have to be in the same scale
    // (seconds and meters or whatever we use)
    bool parseFile(const QString &fname, QVector<double> &xs,
    		QVector<double> &ys);

private:
    Ui::DataAnalyzeWindowClass ui;
    QwtPlotGrid 		mMainGird;
    QwtPlotGrid 		mSecondGird;
    QwtPlotPanner		*mPlanner1;
    QwtPlotMagnifier	*mMagnifier1;
    QwtPlotPanner		*mPlanner2;
    QwtPlotMagnifier	*mMagnifier2;
    QwtPlotZoomer		*mZoomer1;
    QwtPlotZoomer		*mZoomer2;


    // Curves used
    QVector<CurveData*> mCurves;
};

#endif // DATAANALYZEWINDOW_H

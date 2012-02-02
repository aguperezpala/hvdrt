#ifndef DATAANALYZEWINDOW_H
#define DATAANALYZEWINDOW_H

#include <QtGui/QWidget>
#include <qwt_plot_grid.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

#include "CurveData.h"

#include "ui_dataanalyzewindow.h"

class DataAnalyzeWindow : public QWidget
{
    Q_OBJECT

public:
    DataAnalyzeWindow(QWidget *parent = 0);
    ~DataAnalyzeWindow();



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


    // Curves used
    QVector<CurveData*> mCurves;
};

#endif // DATAANALYZEWINDOW_H

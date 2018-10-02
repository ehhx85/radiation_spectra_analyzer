#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// --- Header Definitions ---
#include <QDir>
#include <QMainWindow>
#include <QSlider>
#include <QString>
#include <QTimer>

#include "myspectra.h"
#include "qcustomplot.h"

// --- Main Window Class ---
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // --- Main Window Constructor / Destructor ---
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    // --- File Menu ---
    void save();
    void saveAs();
    void close();
    void quit();

    // --- Spectra Menu ---
    void loadDataFile();
    void loadDataSum();
    void loadDataAverage();
    void loadDatbFile();
    void loadDatbSum();
    void loadDatbAverage();

    // --- Processing Menu---
    void processSpectraReset();
    void processSpectraAdd();
    void processSpectraSubtract();
    void processSpectraDivide();

    // --- Help Menu ---
    void about();
    void aboutQt();
    void aboutAuthor();

    // --- Update Histogram ---
    void updateGUI();
    void updateHistogramsLinear();
    void updateHistogramsLog();
    void updateHistogramData();
    void updateHistogramBins();
    void updateHistogramYAxis();
    void updateHistogramROI();
    void updateHistogramStatistics();

    // --- Update Sliders ---
    void updateSliders();
    void updateSliderBinMin();
    void updateSliderBinMax();
    void updateSliderYMin();
    void updateSliderYMax();
    void updateSliderROIMin();
    void updateSliderROIMax();
    void updateSliderLCDs();

    // --- Other Slots ---
    int getDigits(int value);

private:
    // --- Main Window ---
    Ui::MainWindow *ui;

    // --- ---
    QDir inputDirectory;
    QDir outputDirectory;

    // --- Menus ---
    QMenu *fileMenu;
    QMenu *spectraMenu;
    QMenu *processMenu;
    QMenu *helpMenu;

    // --- File Menu Actions ---
    QAction *openExampleAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;
    QAction *exitAction;

    // --- Spectra Menu Actions ---
    QAction *loadDataFileAction;
    QAction *loadDataSumAction;
    QAction *loadDataAverageAction;
    QAction *loadDatbFileAction;
    QAction *loadDatbSumAction;
    QAction *loadDatbAverageAction;

    // --- Proces Menu Actions ---
    QAction *processSpectraResetAction;
    QAction *processSpectraAddAction;
    QAction *processSpectraSubtractAction;
    QAction *processSpectraDivideeAction;

    // --- Help Menu Actions ---
    QAction *aboutAction;
    QAction *aboutQtAction;
    QAction *aboutAuthorAction;

    // --- Histograms ---
    QCPBars *barsSpectra1;
    QCPBars *barsSpectra2;
    QCPBars *barsSpectra3;
    QCPBars *barsROI1;
    QCPBars *barsROI2;
    QCPBars *barsROI3;
    QCPBars *barsIndex1;
    QCPBars *barsIndex2;
    QCPBars *barsIndex3;

    // --- Spectra Class Objects ---
    mySpectra testSpectra1;
    mySpectra testSpectra2;
    mySpectra testSpectra3;

    // --- Build Methods ---
    void buildMenus();
    void buildComboBoxes();
    void buildButtons();
    void buildSliderBars();
    void buildLCDs();
    void buildHistograms();

    // --- Status Bar ---
    void menuStatus(QString menuString, QString subMenuString);
    void appendStatus(QString newString);
    void appendToStatusBarNoFile();
    void appendToStatusBarNoFolder();

    // --- File I/O ----
    QString getFilePath();
    QStringList getFileList();
    void saveToFile(QString filePath);




};

#endif // MAINWINDOW_H

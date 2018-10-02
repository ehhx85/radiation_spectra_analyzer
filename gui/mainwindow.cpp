// ----- Standard Includes ----------------------------------------------------
#include <cstdlib>
#include <ctime>
#include <iostream>

// ----- Qt Creator Includes --------------------------------------------------
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QStatusBar>

// ----- Main Window: Class Constructor / Destructor --------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    testSpectra1(mySpectra(16384,"Test Spectra 1")),
    testSpectra2(mySpectra(16384,"Test Spectra 2")),
    testSpectra3(mySpectra(16384,"Test Spectra 3"))
{
    ui->setupUi(this);

    buildMenus();
    buildComboBoxes();
    buildButtons();
    buildSliderBars();
    buildLCDs();
    buildHistograms();
    updateGUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ----- ----------------------------------------------------------------------
void MainWindow::buildMenus()
{
    // --- New Menu Bars ---
    fileMenu = menuBar()->addMenu(tr("&File"));
    spectraMenu = menuBar()->addMenu(tr("&Spectra"));
    processMenu = menuBar()->addMenu(tr("&Process"));
    helpMenu = menuBar()->addMenu(tr("&Help"));

    // --- New Menu Actions ---
    saveAction = new QAction(tr("&Save"), this);
    saveAsAction = new QAction(tr("Save &As..."), this);
    closeAction = new QAction(tr("&Close"), this);
    exitAction = new QAction(tr("&Exit"), this);

    loadDataFileAction = new QAction(tr("A from File"), this);
    loadDataSumAction = new QAction(tr("A from Folder Sum"), this);
    loadDataAverageAction = new QAction(tr("A from Folder Average"), this);
    loadDatbFileAction = new QAction(tr("B from File"), this);
    loadDatbSumAction = new QAction(tr("B from Folder Sum"), this);
    loadDatbAverageAction = new QAction(tr("B from Folder Average"), this);

    processSpectraResetAction = new QAction(tr("&Reset"), this);
    processSpectraAddAction = new QAction(tr("&Addition"), this);
    processSpectraSubtractAction = new QAction(tr("&Subtraction"), this);
    processSpectraDivideeAction = new QAction(tr("&Normalize"), this);

    aboutAction = new QAction(tr("&About This Application"), this);
    aboutQtAction = new QAction(tr("&About Qt"), this);
    aboutAuthorAction = new QAction(tr("&About Author"), this);

    // --- Set Menu Shortcuts ---
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    closeAction->setShortcut(QKeySequence::Close);
    exitAction->setShortcut(QKeySequence::Quit);

    loadDataFileAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_1));
    loadDatbFileAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_2));

    loadDataSumAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_4));
    loadDatbSumAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_5));

    loadDataAverageAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_7));
    loadDatbAverageAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_8));

    processSpectraResetAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_0));
    processSpectraAddAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Plus));
    processSpectraSubtractAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Minus));
    processSpectraDivideeAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Slash));

    // --- Connect actions to slots triggered at menu ---
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(quit()));

    connect(loadDataFileAction, SIGNAL(triggered()), this, SLOT(loadDataFile()));
    connect(loadDataSumAction, SIGNAL(triggered()), this, SLOT(loadDataSum()));
    connect(loadDataAverageAction, SIGNAL(triggered()), this, SLOT(loadDataAverage()));
    connect(loadDatbFileAction, SIGNAL(triggered()), this, SLOT(loadDatbFile()));
    connect(loadDatbSumAction, SIGNAL(triggered()), this, SLOT(loadDatbSum()));
    connect(loadDatbAverageAction, SIGNAL(triggered()), this, SLOT(loadDatbAverage()));

    connect(processSpectraResetAction, SIGNAL(triggered()), this, SLOT(processSpectraReset()));
    connect(processSpectraAddAction, SIGNAL(triggered()), this, SLOT(processSpectraAdd()));
    connect(processSpectraSubtractAction, SIGNAL(triggered()), this, SLOT(processSpectraSubtract()));
    connect(processSpectraDivideeAction, SIGNAL(triggered()), this, SLOT(processSpectraDivide()));

    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutAuthorAction, SIGNAL(triggered()), this, SLOT(aboutAuthor()));

    // --- Add the actions to the file menu bar ---
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(closeAction);
    fileMenu->addAction(exitAction);

    spectraMenu->addAction(loadDataFileAction);
    spectraMenu->addAction(loadDataSumAction);
    spectraMenu->addAction(loadDataAverageAction);
    spectraMenu->addAction(loadDatbFileAction);
    spectraMenu->addAction(loadDatbSumAction);
    spectraMenu->addAction(loadDatbAverageAction);

    processMenu->addAction(processSpectraResetAction);
    processMenu->addAction(processSpectraAddAction);
    processMenu->addAction(processSpectraSubtractAction);
    processMenu->addAction(processSpectraDivideeAction);

    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
    helpMenu->addAction(aboutAuthorAction);
}

void MainWindow::buildComboBoxes()
{

}

void MainWindow::buildButtons()
{
    connect(ui->pushButtonResetScale, SIGNAL(pressed()), this, SLOT(updateSliders()));
    connect(ui->pushButtonROIPeak, SIGNAL(pressed()), this, SLOT(updateHistogramStatistics()));
}

void MainWindow::buildSliderBars()
{
    updateSliders();

    connect(ui->sliderBinMin, SIGNAL(valueChanged(int)), this, SLOT(updateHistogramBins()));
    connect(ui->sliderBinMax, SIGNAL(valueChanged(int)), this, SLOT(updateHistogramBins()));
    connect(ui->sliderROIMin, SIGNAL(valueChanged(int)), this, SLOT(updateHistogramROI()));
    connect(ui->sliderROIMax, SIGNAL(valueChanged(int)), this, SLOT(updateHistogramROI()));
    connect(ui->sliderYMin, SIGNAL(valueChanged(int)), this, SLOT(updateHistogramYAxis()));
    connect(ui->sliderYMax, SIGNAL(valueChanged(int)), this, SLOT(updateHistogramYAxis()));

    connect(ui->sliderBinMin, SIGNAL(sliderMoved(int)), this, SLOT(updateSliderBinMin()));
    connect(ui->sliderBinMax, SIGNAL(sliderMoved(int)), this, SLOT(updateSliderBinMax()));
    connect(ui->sliderROIMin, SIGNAL(sliderMoved(int)), this, SLOT(updateSliderROIMin()));
    connect(ui->sliderROIMax, SIGNAL(sliderMoved(int)), this, SLOT(updateSliderROIMax()));
    connect(ui->sliderYMin, SIGNAL(sliderMoved(int)), this, SLOT(updateSliderYMin()));
    connect(ui->sliderYMax, SIGNAL(sliderMoved(int)), this, SLOT(updateSliderYMax()));

    ui->sliderBinMin->setTracking(true);
    ui->sliderBinMax->setTracking(true);
    ui->sliderROIMin->setTracking(true);
    ui->sliderROIMax->setTracking(true);
    ui->sliderYMin->setTracking(true);
    ui->sliderYMax->setTracking(true);
}

void MainWindow::buildLCDs()
{
    ui->lcdBinMin->setDigitCount(5);
    ui->lcdBinMax->setDigitCount(5);

    ui->lcdROIMin->setDigitCount(5);
    ui->lcdROIMax->setDigitCount(5);

    ui->lcdYMin->setDigitCount(9);
    ui->lcdYMax->setDigitCount(9);

    updateSliderLCDs();
}

void MainWindow::buildHistograms()
{
    barsSpectra1 = new QCPBars(ui->qCustomPlot1->xAxis, ui->qCustomPlot1->yAxis);
    barsSpectra2 = new QCPBars(ui->qCustomPlot2->xAxis, ui->qCustomPlot2->yAxis);
    barsSpectra3 = new QCPBars(ui->qCustomPlot3->xAxis, ui->qCustomPlot3->yAxis);
    barsROI1 = new QCPBars(ui->qCustomPlot1->xAxis, ui->qCustomPlot1->yAxis);
    barsROI2 = new QCPBars(ui->qCustomPlot2->xAxis, ui->qCustomPlot2->yAxis);
    barsROI3 = new QCPBars(ui->qCustomPlot3->xAxis, ui->qCustomPlot3->yAxis);
    barsIndex1 = new QCPBars(ui->qCustomPlot1->xAxis, ui->qCustomPlot1->yAxis);
    barsIndex2 = new QCPBars(ui->qCustomPlot2->xAxis, ui->qCustomPlot2->yAxis);
    barsIndex3 = new QCPBars(ui->qCustomPlot3->xAxis, ui->qCustomPlot3->yAxis);

    barsSpectra1->setName("Spectra 1");
    barsSpectra2->setName("Spectra 2");
    barsSpectra3->setName("Spectra 3");
    barsROI1->setName("ROI 1");
    barsROI2->setName("ROI 2");
    barsROI3->setName("ROI 3");
    barsIndex1->setName("");
    barsIndex2->setName("");
    barsIndex3->setName("");

    QPen barOutlineColor = QPen(Qt::black);
    barsSpectra1->setPen(barOutlineColor);
    barsSpectra2->setPen(barOutlineColor);
    barsSpectra3->setPen(barOutlineColor);

    barOutlineColor = QPen(Qt::blue);
    barsROI1->setPen(barOutlineColor);
    barsROI2->setPen(barOutlineColor);
    barsROI3->setPen(barOutlineColor);

    barOutlineColor = QPen(Qt::red);
    barsIndex1->setPen(barOutlineColor);
    barsIndex2->setPen(barOutlineColor);
    barsIndex3->setPen(barOutlineColor);

    QBrush barFillColor = QColor(0,0,0,50);
    barsSpectra1->setBrush(barFillColor);
    barsSpectra2->setBrush(barFillColor);
    barsSpectra3->setBrush(barFillColor);

    barFillColor = QColor(0,0,255,25);
    barsROI1->setBrush(barFillColor);
    barsROI2->setBrush(barFillColor);
    barsROI3->setBrush(barFillColor);

    barFillColor = QColor(255,0,255,75);
    barsIndex1->setBrush(barFillColor);
    barsIndex2->setBrush(barFillColor);
    barsIndex3->setBrush(barFillColor);

    connect(ui->pushButtonLinearScale, SIGNAL(pressed()), this, SLOT(updateHistogramsLinear()));
    connect(ui->pushButtonLogScale, SIGNAL(pressed()), this, SLOT(updateHistogramsLog()));

    updateHistogramsLog();
    updateHistogramData();
    updateHistogramBins();
    updateHistogramROI();
    updateHistogramYAxis();
}

// ----- File Menu Action Slots -----------------------------------------------
void MainWindow::save()
{
    menuStatus("File","Save");

    QString tmp = QDir::homePath();
            tmp += QString("/Desktop");
            tmp += QString("/tmp_");
            tmp += QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss");
            tmp += QString(".spe");
    saveToFile(tmp);
}

void MainWindow::saveAs()
{
    menuStatus("File","Save As...");

    QFileDialog dialog(this);

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);

    dialog.setDirectory(QDir::homePath() + QString("/Desktop"));
    dialog.setNameFilter(tr("MAESTRO Spectra (*.spe)"));
    dialog.setDefaultSuffix("spe");

    if (dialog.exec()) {
        QStringList filePath = dialog.selectedFiles();
        saveToFile(filePath.at(0));
    }
    else {
        appendStatus(" ... save canceled");
    }
}

void MainWindow::saveToFile(QString filePath)
{
    if(filePath.isNull())
    {
        return;
    }
    appendStatus(QString("Saving spectra to file ... ") + filePath);
    testSpectra3.saveSpectra(filePath,0); // Adjust this value for output precision
}

void MainWindow::close()
{
    menuStatus("File","Close");

}

void MainWindow::quit()
{
    menuStatus("File","Quit");
    QApplication::quit();
}

// ----- Spectra Menu Action Slots --------------------------------------------
void MainWindow::loadDataFile()
{
    menuStatus("Spectra","Load A from file");
    QString filePath = getFilePath();

    if (filePath.isNull()) { return; }

    mySpectra spectraFile(filePath);

    testSpectra1 = spectraFile;
    testSpectra3 = spectraFile;
    updateHistogramData();
}

void MainWindow::loadDataSum()
{
    menuStatus("Spectra","Load A Sum");
    QStringList fileList = getFileList();

    if (fileList.isEmpty()) { return; }

    mySpectra spectraFile(fileList);

    testSpectra1 = spectraFile;
    testSpectra3 = spectraFile;
    updateHistogramData();
}

void MainWindow::loadDataAverage()
{
    menuStatus("Spectra","Load A Average");
    QStringList fileList = getFileList();

    if (fileList.isEmpty()) { return; }

    mySpectra spectraFile(fileList);
    spectraFile /= fileList.size();

    testSpectra1 = spectraFile;
    testSpectra3 = spectraFile;
    updateHistogramData();
}

void MainWindow::loadDatbFile()
{
    menuStatus("Spectra","Load B from file");
    QString tmp = getFilePath();

    if (tmp.isNull()) { return; }

    appendStatus(QString("... loading file @ ") + tmp);

    mySpectra spectraFile(tmp);

    testSpectra2 = spectraFile;
    updateHistogramData();
}

void MainWindow::loadDatbSum()
{
    menuStatus("Spectra","Load B Average");
    QStringList fileList = getFileList();

    if (fileList.isEmpty()) { return; }

    mySpectra spectraFile(fileList);

    testSpectra2 = spectraFile;
    updateHistogramData();
}

void MainWindow::loadDatbAverage()
{
    menuStatus("Spectra","Load B Sum");
    QStringList fileList = getFileList();

    if (fileList.isEmpty()) { return; }

    mySpectra spectraFile(fileList);
    spectraFile /= fileList.size();

    testSpectra2 = spectraFile;
    updateHistogramData();
}

QString MainWindow::getFilePath()
{
    QFileDialog dialog(this);

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setViewMode(QFileDialog::Detail);

    dialog.setDirectory(QDir::homePath()+QString("/Desktop"));
    dialog.setNameFilter(tr("MAESTRO (*.spe)"));
    dialog.setDefaultSuffix("spe");

    if (dialog.exec())
    {
        QStringList fileList = dialog.selectedFiles();
        QString filePath = fileList.at(0);

        appendStatus(QString("... loading file @ ") + filePath);

        return filePath;
    }
    else
    {
        appendToStatusBarNoFile();
        return QString();
    }
}

QStringList MainWindow::getFileList()
{
    QFileDialog dialog(this);

    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    dialog.setViewMode(QFileDialog::Detail);
    dialog.setDirectory(QDir::homePath()+QString("/Desktop"));

    if (dialog.exec()) {
        QStringList folderList = dialog.selectedFiles();
        QString folderPath = folderList.at(0);
        QDir fileDirectory = QDir(folderPath);

        appendStatus(QString("... loading batch folder @ ") + folderPath);

        QStringList filters;
        filters.append("*.spe");
        fileDirectory.setNameFilters(filters);

        QFileInfoList fileList = fileDirectory.entryInfoList();
        QStringList fileStringList;

        for (int i=0; i<fileList.size(); i++)
        {
            QString tmp = fileList.at(i).filePath();
            fileStringList.append(tmp);
        }

        return fileStringList;
    }
    else
    {
        appendToStatusBarNoFolder();
        return QStringList();
    }
}

// ----- Spectra Menu Action Slots --------------------------------------------
void MainWindow::processSpectraReset()
{
    mySpectra tmpSpectra(256,"Spectra Reset");
    testSpectra1 = tmpSpectra;
    testSpectra2 = tmpSpectra;
    testSpectra3 = tmpSpectra;
    updateHistogramData();
}

void MainWindow::processSpectraAdd()
{
    testSpectra3 = testSpectra1;
    testSpectra3 += testSpectra2;
    updateHistogramData();
}

void MainWindow::processSpectraSubtract()
{
    testSpectra3 = testSpectra1;
    testSpectra3 -= testSpectra2;
    updateHistogramData();
}

void MainWindow::processSpectraDivide()
{
    testSpectra3 = testSpectra1;
    testSpectra3 /= testSpectra2;
    updateHistogramData();
}

// ----- Help Menu Action Slots -----------------------------------------------
void MainWindow::about()
{
    menuStatus("Help","About Application");
}

void MainWindow::aboutQt()
{

}

void MainWindow::aboutAuthor()
{
    menuStatus("Help","About Author");
}

// ----- Histogram Slots ------------------------------------------------------
void MainWindow::updateGUI()
{
    ui->qCustomPlot1->replot();
    ui->qCustomPlot2->replot();
    ui->qCustomPlot3->replot();
}

void MainWindow::updateHistogramsLinear()
{
    QSharedPointer<QCPAxisTicker> linearTicker(new QCPAxisTicker);

    ui->qCustomPlot1->yAxis->setScaleType(QCPAxis::stLinear);
    ui->qCustomPlot2->yAxis->setScaleType(QCPAxis::stLinear);
    ui->qCustomPlot3->yAxis->setScaleType(QCPAxis::stLinear);

    ui->qCustomPlot1->yAxis->setTicker(linearTicker);
    ui->qCustomPlot2->yAxis->setTicker(linearTicker);
    ui->qCustomPlot3->yAxis->setTicker(linearTicker);

    ui->qCustomPlot1->yAxis->setNumberFormat("gb");
    ui->qCustomPlot2->yAxis->setNumberFormat("gb");
    ui->qCustomPlot3->yAxis->setNumberFormat("gb");

    ui->qCustomPlot1->yAxis->setNumberPrecision(0);
    ui->qCustomPlot2->yAxis->setNumberPrecision(0);
    ui->qCustomPlot3->yAxis->setNumberPrecision(0);

    updateGUI();
}

void MainWindow::updateHistogramsLog()
{
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);

    ui->qCustomPlot1->yAxis->setScaleType(QCPAxis::stLogarithmic);
    ui->qCustomPlot2->yAxis->setScaleType(QCPAxis::stLogarithmic);
    ui->qCustomPlot3->yAxis->setScaleType(QCPAxis::stLogarithmic);

    ui->qCustomPlot1->yAxis->setTicker(logTicker);
    ui->qCustomPlot2->yAxis->setTicker(logTicker);
    ui->qCustomPlot3->yAxis->setTicker(logTicker);

    ui->qCustomPlot1->yAxis->setNumberFormat("eb");
    ui->qCustomPlot2->yAxis->setNumberFormat("eb");
    ui->qCustomPlot3->yAxis->setNumberFormat("eb");

    ui->qCustomPlot1->yAxis->setNumberPrecision(0);
    ui->qCustomPlot2->yAxis->setNumberPrecision(0);
    ui->qCustomPlot3->yAxis->setNumberPrecision(0);

    updateGUI();
}

void MainWindow::updateHistogramData()
{
    barsSpectra1->setData(testSpectra1.vBins,testSpectra1.vCounts);
    barsSpectra2->setData(testSpectra2.vBins,testSpectra2.vCounts);
    barsSpectra3->setData(testSpectra3.vBins,testSpectra3.vCounts);

    ui->labelInfo1ID->setText(testSpectra1.ID());
    ui->labelInfo2ID->setText(testSpectra2.ID());
    ui->labelInfo3ID->setText(testSpectra3.ID());

    ui->labelInfo1Name->setText(testSpectra1.fileName());
    ui->labelInfo2Name->setText(testSpectra2.fileName());
    ui->labelInfo3Name->setText(testSpectra3.fileName());

    ui->labelInfo1DateTime->setText(testSpectra1.runDateTime().toString());
    ui->labelInfo2DateTime->setText(testSpectra2.runDateTime().toString());
    ui->labelInfo3DateTime->setText(testSpectra3.runDateTime().toString());

    ui->labelInfo1Description->setText(testSpectra1.detectorDescription());
    ui->labelInfo2Description->setText(testSpectra2.detectorDescription());
    ui->labelInfo3Description->setText(testSpectra3.detectorDescription());

    ui->labelInfo1DetectorID->setText(QString::number(testSpectra1.detectorNumber()));
    ui->labelInfo2DetectorID->setText(QString::number(testSpectra2.detectorNumber()));
    ui->labelInfo3DetectorID->setText(QString::number(testSpectra3.detectorNumber()));

    ui->labelInfo1Software->setText(testSpectra1.softwareVersion());
    ui->labelInfo2Software->setText(testSpectra2.softwareVersion());
    ui->labelInfo3Software->setText(testSpectra3.softwareVersion());

    ui->labelInfo1DynamicRange->setText(QString::number(testSpectra1.binSize()));
    ui->labelInfo2DynamicRange->setText(QString::number(testSpectra2.binSize()));
    ui->labelInfo3DynamicRange->setText(QString::number(testSpectra3.binSize()));

    ui->labelInfo1PresetTime->setText(QString::number(testSpectra1.presetTime()));
    ui->labelInfo2PresetTime->setText(QString::number(testSpectra2.presetTime()));
    ui->labelInfo3PresetTime->setText(QString::number(testSpectra3.presetTime()));

    ui->labelInfo1ActualTime->setText(QString::number(testSpectra1.actualTime()));
    ui->labelInfo2ActualTime->setText(QString::number(testSpectra2.actualTime()));
    ui->labelInfo3ActualTime->setText(QString::number(testSpectra3.actualTime()));

    ui->labelInfo1DeadTime->setText(QString::number(testSpectra1.deadTime()));
    ui->labelInfo2DeadTime->setText(QString::number(testSpectra2.deadTime()));
    ui->labelInfo3DeadTime->setText(QString::number(testSpectra3.deadTime()));

    updateSliders();
    updateHistogramBins();
    updateHistogramYAxis();
    updateHistogramROI();
    updateGUI();
}

void MainWindow::updateHistogramBins()
{
    ui->qCustomPlot1->xAxis->setRange(ui->sliderBinMin->value(), ui->sliderBinMax->value());
    ui->qCustomPlot3->xAxis->setRange(ui->sliderBinMin->value(), ui->sliderBinMax->value());

    double tmpMin = testSpectra2.binSize() * ui->sliderBinMin->value() / ui->sliderBinMax->maximum();
    double tmpMax = testSpectra2.binSize() * ui->sliderBinMax->value() / ui->sliderBinMax->maximum();

    ui->qCustomPlot2->xAxis->setRange(tmpMin, tmpMax);

    updateGUI();
    updateSliderLCDs();
}

void MainWindow::updateHistogramYAxis()
{
    ui->qCustomPlot1->yAxis->setRange(ui->sliderYMin->value(), ui->sliderYMax->value());
    ui->qCustomPlot2->yAxis->setRange(ui->sliderYMin->value(), ui->sliderYMax->value());
    ui->qCustomPlot3->yAxis->setRange(ui->sliderYMin->value(), ui->sliderYMax->value());

    updateGUI();
    updateSliderLCDs();
}

void MainWindow::updateHistogramROI()
{
    QVector<double> binsROI1, binsROI2, binsROI3, dataROI1, dataROI2, dataROI3;
    double tmpAvg1, tmpAvg2, tmpAvg3;
    double tmpSum1 = 0;
    double tmpSum2 = 0;
    double tmpSum3 = 0;

    double tmpMin = 1.0 * ui->sliderROIMin->value();
    double tmpMax = 1.0 * ui->sliderROIMax->value();

    for (int i=tmpMin; i<tmpMax; i++)
    {
        binsROI1.append(i);
        binsROI3.append(i);

        dataROI1.append(testSpectra1.vCounts.at(i));
        dataROI3.append(testSpectra3.vCounts.at(i));

        tmpSum1 = tmpSum1 + testSpectra1.vCounts.at(i);
        tmpSum3 = tmpSum3 + testSpectra3.vCounts.at(i);
    }

    tmpAvg1 = tmpSum1 / (tmpMax - tmpMin);
    tmpAvg3 = tmpSum3 / (tmpMax - tmpMin);

    tmpMin = tmpMin / testSpectra1.binSize() * testSpectra2.binSize();
    tmpMax = tmpMax / testSpectra1.binSize() * testSpectra2.binSize();

    for (int i=tmpMin; i<tmpMax; i++)
    {
        binsROI2.append(i);
        dataROI2.append(testSpectra2.vCounts.at(i));
        tmpSum2 = tmpSum2 + testSpectra2.vCounts.at(i);
    }

    tmpAvg2 = tmpSum2 / (tmpMax - tmpMin);

    barsROI1->setData(binsROI1, dataROI1);
    barsROI2->setData(binsROI2, dataROI2);
    barsROI3->setData(binsROI3, dataROI3);

    ui->label1ROILower->setText(QString::number(binsROI1.first()));
    ui->label2ROILower->setText(QString::number(binsROI2.first()));
    ui->label3ROILower->setText(QString::number(binsROI3.first()));

    ui->label2ROIUpper->setText(QString::number(binsROI2.last()));
    ui->label1ROIUpper->setText(QString::number(binsROI1.last()));
    ui->label3ROIUpper->setText(QString::number(binsROI3.last()));

    ui->label1ROISum->setText(QString::number(tmpSum1));
    ui->label2ROISum->setText(QString::number(tmpSum2));
    ui->label3ROISum->setText(QString::number(tmpSum3));

    ui->label1ROIAverage->setText(QString::number(tmpAvg1));
    ui->label2ROIAverage->setText(QString::number(tmpAvg2));
    ui->label3ROIAverage->setText(QString::number(tmpAvg3));

    updateGUI();
    updateSliderLCDs();
}

void MainWindow::updateHistogramStatistics()
{
    double tmpMin = 1.0 * ui->sliderROIMin->value();
    double tmpMax = 1.0 * ui->sliderROIMax->value();

    QVector<double> tmpBin1, tmpBin2, tmpBin3, tmpPeak1, tmpPeak2, tmpPeak3;

    tmpBin1.fill(tmpMin,1);
    tmpBin2.fill(tmpMin,1);
    tmpBin3.fill(tmpMin,1);

    tmpPeak1.fill(0,1);
    tmpPeak2.fill(0,1);
    tmpPeak3.fill(0,1);

    for (int i=tmpMin; i<tmpMax; i++)
    {
        if (testSpectra1.vCounts.at(i) > tmpPeak1.first())
        {
            tmpBin1.replace(0,i);
            tmpPeak1.replace(0,testSpectra1.vCounts.at(i));
        }
        if (testSpectra3.vCounts.at(i) > tmpPeak3.first())
        {
            tmpBin3.replace(0,i);;
            tmpPeak3.replace(0,testSpectra3.vCounts.at(i));
        }
    }

    tmpMin = tmpMin / testSpectra1.binSize() * testSpectra2.binSize();
    tmpMax = tmpMax / testSpectra1.binSize() * testSpectra2.binSize();

    for (int i=tmpMin; i<tmpMax; i++)
    {
        if (testSpectra2.vCounts.at(i) > tmpPeak2.first())
        {
            tmpBin2.replace(0,i);
            tmpPeak2.replace(0,testSpectra2.vCounts.at(i));
        }
    }

    barsIndex1->setData(tmpBin1,tmpPeak1);
    barsIndex2->setData(tmpBin2,tmpPeak2);
    barsIndex3->setData(tmpBin3,tmpPeak3);

    ui->label1PeakBin->setText(QString::number(tmpBin1.first()));
    ui->label2PeakBin->setText(QString::number(tmpBin2.first()));
    ui->label3PeakBin->setText(QString::number(tmpBin3.first()));

    ui->label1PeakValue->setText(QString::number(tmpPeak1.first()));
    ui->label2PeakValue->setText(QString::number(tmpPeak2.first()));
    ui->label3PeakValue->setText(QString::number(tmpPeak3.first()));

    updateGUI();
}

// ----- Slider Slots ---------------------------------------------------------
void MainWindow::updateSliders()
{
    int tmpOffset = 2;
    int tmpRangeMax = testSpectra1.binSize() - 1;
    int tmpRangeMin = tmpRangeMax - tmpOffset;

    ui->sliderBinMin->setRange(0, tmpRangeMin);
    ui->sliderBinMax->setRange(tmpOffset, tmpRangeMax);

    ui->sliderROIMin->setRange(0, tmpRangeMin);
    ui->sliderROIMax->setRange(tmpOffset, tmpRangeMax);

    double tmpRange = testSpectra1.maxIntensity();
    if (testSpectra2.maxIntensity() > tmpRange)
    {
        tmpRange = testSpectra2.maxIntensity();
    }
    if (testSpectra3.maxIntensity() > tmpRange)
    {
        tmpRange = testSpectra3.maxIntensity();
    }

    ui->sliderYMin->setRange(1, tmpRange - 1);
    ui->sliderYMax->setRange(2, tmpRange);
    ui->sliderYMin->setSingleStep(10);
    ui->sliderYMax->setSingleStep(10);

    // --- Reset sliders to limits ---
    ui->sliderBinMin->setValue(ui->sliderBinMin->minimum());
    ui->sliderBinMax->setValue(ui->sliderBinMax->maximum());
    ui->sliderROIMin->setValue(ui->sliderROIMin->minimum());
    ui->sliderROIMax->setValue(ui->sliderROIMax->minimum());
    ui->sliderYMin->setValue(ui->sliderYMin->minimum());
    ui->sliderYMax->setValue(ui->sliderYMax->maximum());

    // --- Adjust the LCDs ---
    ui->lcdBinMin->setDigitCount(MainWindow::getDigits(ui->sliderBinMax->maximum()));
    ui->lcdBinMax->setDigitCount(MainWindow::getDigits(ui->sliderBinMax->maximum()));
    ui->lcdROIMin->setDigitCount(MainWindow::getDigits(ui->sliderROIMax->maximum()));
    ui->lcdROIMax->setDigitCount(MainWindow::getDigits(ui->sliderROIMax->maximum()));
    ui->lcdYMin->setDigitCount(MainWindow::getDigits(ui->sliderYMax->maximum()));
    ui->lcdYMax->setDigitCount(MainWindow::getDigits(ui->sliderYMax->maximum()));

    updateSliderLCDs();
}

void MainWindow::updateSliderBinMin()
{
    ui->sliderBinMax->setMinimum(ui->sliderBinMin->sliderPosition()+2);
    ui->sliderROIMax->setMinimum(ui->sliderBinMin->sliderPosition()+1);
    ui->sliderROIMin->setMinimum(ui->sliderBinMin->sliderPosition());
    updateSliderLCDs();
}

void MainWindow::updateSliderBinMax()
{
    ui->sliderBinMin->setMaximum(ui->sliderBinMax->sliderPosition()-2);
    ui->sliderROIMin->setMaximum(ui->sliderBinMax->sliderPosition()-1);
    ui->sliderROIMax->setMaximum(ui->sliderBinMax->sliderPosition());
    updateSliderLCDs();
}

void MainWindow::updateSliderROIMin()
{
    ui->sliderROIMax->setMinimum(ui->sliderROIMin->sliderPosition()+1);
    updateSliderLCDs();
}

void MainWindow::updateSliderROIMax()
{
    ui->sliderROIMin->setMaximum(ui->sliderROIMax->sliderPosition()-1);
    updateSliderLCDs();
}

void MainWindow::updateSliderYMin()
{
    ui->sliderYMax->setMinimum(ui->sliderYMin->sliderPosition()+1);
    updateSliderLCDs();
}

void MainWindow::updateSliderYMax()
{
    ui->sliderYMin->setMaximum(ui->sliderYMax->sliderPosition()-1);
    updateSliderLCDs();
}

void MainWindow::updateSliderLCDs()
{
    ui->lcdBinMin->display(ui->sliderBinMin->sliderPosition());
    ui->lcdBinMax->display(ui->sliderBinMax->sliderPosition());
    ui->lcdROIMin->display(ui->sliderROIMin->sliderPosition());
    ui->lcdROIMax->display(ui->sliderROIMax->sliderPosition());
    ui->lcdYMin->display(ui->sliderYMin->sliderPosition());
    ui->lcdYMax->display(ui->sliderYMax->sliderPosition());
}

// ----- Status Bar -----------------------------------------------------------
void MainWindow::menuStatus(QString menuString,
                                              QString subMenuString)
{
    QString S = " | ";
    QString tmp = menuString + S + subMenuString + S;
    statusBar()->showMessage(tmp);
}

void MainWindow::appendStatus(QString newString)
{
    statusBar()->showMessage(statusBar()->currentMessage()+newString);
}

void MainWindow::appendToStatusBarNoFile()
{
    appendStatus(QString("... no file was selected. Load canceled."));
}

void MainWindow::appendToStatusBarNoFolder()
{
    appendStatus(QString("... no folder was selected. Batch load canceled."));
}

// --- Other Slots ------------------------------------------------------------
int MainWindow::getDigits(int value)
{
    value = abs(value);
    return (value < 10 ? 1 :
           (value < 100 ? 2 :
           (value < 1000 ? 3 :
           (value < 10000 ? 4 :
           (value < 100000 ? 5 :
           (value < 1000000 ? 6 :
           (value < 10000000 ? 7 :
           (value < 100000000 ? 8 :
           (value < 1000000000 ? 9 :
            10)))))))));
}

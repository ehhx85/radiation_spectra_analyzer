#ifndef MYSPECTRA_H
#define MYSPECTRA_H

#include <iostream>
#include <cmath>

#include <QDateTime>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QVector>

class mySpectra
{

private:
    // --- Build Type ---
    QString constructorType;

    // --- Spectra Header Information ---
    QString spectraFileName;
    QString spectraID;
    unsigned int spectraDetectorNumber;
    QString spectraDetectorDescription;
    QString spectraSoftwareVersion;
    QDateTime spectraRunDateTime;

    // --- Spectra Data Information
    unsigned int spectraPresetTime;
    unsigned int spectraActualTime;
    unsigned int spectraDeadTime;
    double deadTimeFrac;
    unsigned int maxBin;
    unsigned int dynamicRange;
    double intensityRange;

    // --- Spectra Data Vectors ---

    void setDeadTime();
    void setDynamicRange();
    void setDynamicRange(unsigned int value);
    void setIntensityRange();
    void buildBins();
    void buildCountRate();
    void resetCounts();
    void rebinCounts(const QVector<double>& vOld,
                     QVector<double>& vNew,
                     unsigned int newDynamicRange) const;

public:
    // --- Class Static Members ---
    static const unsigned int DRValues = 16;
    static const QVector<unsigned int> DRList;

    // --- Constructor / Destructor ---
    mySpectra();
    mySpectra(unsigned int value);
    mySpectra(unsigned int value, QString name);
    mySpectra(QString filePath);
    mySpectra(QStringList fileList);
    ~mySpectra();

    // --- Operator Overloads ---
    mySpectra operator + (const mySpectra& RHS);
    mySpectra& operator += (const mySpectra& RHS);

    mySpectra operator - (const mySpectra& RHS);
    mySpectra& operator -= (const mySpectra& RHS);

    mySpectra operator * (const double RHS);
    mySpectra& operator *= (const double RHS);

    mySpectra operator / (const mySpectra& RHS);
    mySpectra& operator /= (const mySpectra& RHS);
    mySpectra operator / (const double RHS);
    mySpectra& operator /= (const double RHS);

    // --- Standard Class Methods ---
    void clear();
    void saveSpectra(QString filePath, int modifier);

    // --- Accessors ---
    unsigned int binSize();
    double maxIntensity();
    QString fileName();
    QString ID();
    unsigned int detectorNumber();
    QString detectorDescription();
    QString softwareVersion();
    QDateTime runDateTime();
    unsigned int presetTime();
    unsigned int actualTime();
    unsigned int deadTime();
    unsigned int maxDR(unsigned int range1, unsigned int range2);
    unsigned int minDR(unsigned int range1, unsigned int range2);

    // --- Distributions ---
    void setRandomDistribution(unsigned int intensityRange);
    void setNormalDistribution(double offset, double sigma);
    void setUniformDistribution();

    // --- ---
    QVector<double> vBins;
    QVector<double> vCounts;
    QVector<double> vCountRate;
};

#endif // MYSPECTRA_H

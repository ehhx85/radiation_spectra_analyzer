#include "myspectra.h"

const QVector<unsigned int> mySpectra::DRList = {
    1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768};


// ----- Class Constructor / Destructor ---------------------------------------
mySpectra::mySpectra()
{
    clear();
    constructorType = "[DEFAULT]";
    spectraFileName = "Default Spectra";
}

mySpectra::mySpectra(unsigned int value)
{
    clear();
    constructorType = "[BIN SIZE]";
    spectraFileName = "Test Spectra";
    spectraID = "Empty spectra object created by default";
    spectraDetectorNumber = 0;
    spectraDetectorDescription = "No detector information";
    maxBin = value - 1;
    setDynamicRange();
    buildBins();
    resetCounts();
    buildCountRate();
}

mySpectra::mySpectra(unsigned int value, QString name)
{
    clear();
    constructorType = QString("[")+name+QString("]");
    spectraFileName = name;
    spectraID = "Empty spectra object created by default";
    spectraDetectorNumber = 0;
    spectraDetectorDescription = "No detector information";
    maxBin = value - 1;
    setDynamicRange();
    buildBins();
    resetCounts();
    buildCountRate();
}

mySpectra::mySpectra(QString path)
{
    clear();
    constructorType = "[FILE]";
    spectraFileName = path;

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {return;}
    QTextStream in(&file);
    QString line;

    for(unsigned int i=1; i<13; i++) // scan the header lines
    {
        line = in.readLine();

        unsigned int lineOffset = 0;

        QRegularExpression re;
        QRegularExpressionMatch match;

        QString tmp;

        switch (i) {
        case 1:
            // $SPEC_ID:
            break;
        case 2:
            spectraID = line;
            break;
        case 3:
            // $SPEC_REM:
            break;
        case 4:
            lineOffset = 5;
            for(unsigned int j=0; j<line.size()-lineOffset; j++)
            {
                tmp.replace(j,1,line.at(j+lineOffset));
            }
            spectraDetectorNumber = tmp.toInt();
            break;
        case 5:
            lineOffset = 9;
            for(unsigned int j=0; j<line.size()-lineOffset; j++)
            {
                spectraDetectorDescription.replace(j,1,line.at(j+lineOffset));
            }
            break;
        case 6:
            lineOffset = 4;
            for(unsigned int j=0; j<line.size()-lineOffset; j++)
            {
                spectraSoftwareVersion.replace(j,1,line.at(j+lineOffset));
            }
            break;
        case 7:
            // $DATE_MEA:
            break;
        case 8:
            tmp = QString("MM/dd/yyyy hh:mm:ss");
            spectraRunDateTime = QDateTime::fromString(line, tmp);
            break;
        case 9:
            // $MEAS_TIM:
            break;
        case 10:
            re.setPattern("(\\d+) (\\d+)");
            match = re.match(line);
            if (match.hasMatch())
            {
                spectraPresetTime = match.captured(1).toInt();
                spectraActualTime = match.captured(2).toInt();

                setDeadTime();
            }
            break;
        case 11:
            // $DATA:
            break;
        case 12:
            re.setPattern("(\\d+) (\\d+)");
            match = re.match(line);
            if (match.hasMatch())
            {
                //double tmp = match.captured(1).toInt(); // minimum value
                maxBin = match.captured(2).toInt();
            }
            setDynamicRange();
            break;
        default:
            break;
        }
    }

    buildBins();
    resetCounts();
    for(unsigned int i=0; i<dynamicRange; i++) // scan  the data array
    {
        line = in.readLine();
        vCounts.replace(i,line.toDouble());
    }

    while(!in.atEnd()) // scan the remaining text in file
    {
        line = in.readLine();
        // TODO : add functions to extract the calibration data
    }

    setIntensityRange();
    buildCountRate();
}

mySpectra::mySpectra(QStringList fileList)
{
    clear();
    constructorType = "[FILE LIST]";

    for (int i=0; i<fileList.size(); i++)
    {
        QString tmpPath = fileList.at(i);
        mySpectra tmpSpectraSample(tmpPath);

        if (i==0)
        {
            *this = tmpSpectraSample;
        }
        else
        {
            *this += tmpSpectraSample;
        }
    }   
}

mySpectra::~mySpectra()
{

}

// ----- -----
void mySpectra::clear()
{
    constructorType.clear();

    spectraFileName.clear();
    spectraID = "";
    spectraDetectorNumber = 0;
    spectraDetectorDescription = "";
    spectraSoftwareVersion = QString("Qt ") + QT_VERSION_STR;
    spectraRunDateTime = QDateTime::currentDateTime();

    spectraPresetTime = 1;
    spectraActualTime = 1;
    spectraDeadTime = 0;
    deadTimeFrac = 0.0;
    maxBin = 255;
    intensityRange = 0.0;

    setDynamicRange();
    buildBins();
    resetCounts();
    buildCountRate();
}


// ----- File Output --------------------------------------------------------
void mySpectra::saveSpectra(QString filePath, int modifier)
{
    QFile file(filePath);
    if(file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&file);
        out << "$SPEC_ID:\n";
        if (spectraID.isEmpty())
        {
            out << "no description" << endl;
        }
        else
        {
            out << spectraID << endl;
        }
        out << "$SPEC_REM:" << endl;
        out << "DET# " << spectraDetectorNumber << endl;
        out << "DETDESC# " << spectraDetectorDescription << endl;
        out << "AP# " << spectraSoftwareVersion << endl;
        out << "$DATE_MEA:" << endl;
        out << spectraRunDateTime.toString("MM/dd/yyyy hh:mm:ss") << endl;
        out << "$MEAS_TIM:" << endl;
        out << spectraPresetTime << " " << spectraActualTime << endl;
        out << "$DATA:" << endl;
        out << "0 " << dynamicRange-1 << endl;
        for(unsigned int i=0; i<dynamicRange; i++)
        {
            if (modifier == -1)
            {
                out << vCounts.at(i);
            }
            else {
                unsigned int tmp = vCounts.at(i);
                out << tmp << endl;
            }
        }
        out << "$ROI:" << endl;
        out << 0 << endl;
        out << "$PRESETS:" << endl;
        out << "Live Time" << endl;
        out << spectraPresetTime << endl;
        out << 0 << endl;
        out << "$ENER_FIT:" << endl;
        out << "0.000000 0.000000" << endl;
        out << "$MCA_CAL:" << endl;
        out << "0" << endl;
        out << "0.000000E+000 0.000000E+000 0.000000E+000" << endl;
        out << "$SHAPE_CAL:" << endl;
        out << "0" << endl;
        out << "0.000000E+000 0.000000E+000 0.000000E+000" << endl;
    }
}

// ----- Rebinning Functions --------------------------------------------------
void mySpectra::rebinCounts(const QVector<double>& oldVector,
                            QVector<double>& newVector,
                            unsigned int newDynamicRange) const
{
    unsigned int oldDynamicRange = oldVector.size();
    if(newDynamicRange == oldDynamicRange) {
        newVector.clear();
        newVector = oldVector; // return original count vector with no resize
    }
    else {
        newVector.clear();
        newVector.fill(0,newDynamicRange);
        unsigned int scale=0;

        if(newDynamicRange>oldDynamicRange) {
            scale = newDynamicRange/oldDynamicRange;

            for(unsigned int iOld=0; iOld<oldDynamicRange; iOld++)
            {
                for(unsigned int iNew=iOld*scale; iNew<((iOld+1)*scale); iNew++)
                {
                    double tmp = oldVector.at(iOld) / scale;
                    newVector.replace(iNew,tmp);
                }
            }
        }
        else {
            scale = oldDynamicRange/newDynamicRange;

            for(unsigned int iNew=0; iNew<newDynamicRange; iNew++)
            {
                double tmp=0;
                for(unsigned int iOld=iNew*scale; iOld<((iNew+1)*scale); iOld++)
                {
                    tmp = tmp + oldVector.at(iOld);
                }
                newVector.replace(iNew,tmp);
            }
        }
    }
}
// ----- Operator Overloads ---------------------------------------------------
mySpectra mySpectra::operator + (const mySpectra& RHS)
{
    mySpectra temp = *this;
    temp += RHS;
    return temp;
}

mySpectra& mySpectra::operator += (const mySpectra& RHS)
{
    spectraSoftwareVersion = "Spectra Addition";

    spectraActualTime += RHS.spectraActualTime;
    spectraPresetTime += RHS.spectraPresetTime;
    setDeadTime();

    unsigned int tmpDynamicRange = maxDR(dynamicRange, RHS.dynamicRange);

    QVector<double> tmpResult(tmpDynamicRange);
    QVector<double> tmpLeft(tmpDynamicRange);
    QVector<double> tmpRight(tmpDynamicRange);

    rebinCounts(vCounts,tmpLeft,tmpDynamicRange);
    rebinCounts(RHS.vCounts,tmpRight,tmpDynamicRange);

    for(unsigned int i=0; i<tmpDynamicRange; i++)
    {
        double tmp = tmpLeft.at(i) + tmpRight.at(i);
        tmpResult.replace(i,tmp);
    }

    rebinCounts(tmpResult,vCounts,dynamicRange);
    setIntensityRange();
    buildCountRate();

    return *this;
}

mySpectra mySpectra::operator - (const mySpectra& RHS)
{
    mySpectra temp = *this;
    temp -= RHS;
    return temp;
}

mySpectra& mySpectra::operator -= (const mySpectra& RHS)
{
    spectraSoftwareVersion = "Spectra Subtraction";

    unsigned int tmpDynamicRange = maxDR(dynamicRange, RHS.dynamicRange);

    QVector<double> tmpResult(tmpDynamicRange);
    QVector<double> tmpLeft(tmpDynamicRange);
    QVector<double> tmpRight(tmpDynamicRange);

    rebinCounts(vCounts,tmpLeft,tmpDynamicRange);
    rebinCounts(RHS.vCounts,tmpRight,tmpDynamicRange);

    double timeDiff = 1.0 * spectraPresetTime / RHS.spectraActualTime;

    for(unsigned int i=0; i<tmpDynamicRange; i++)
    {
        double tmp = tmpLeft.at(i) - (tmpRight.at(i) * timeDiff);
        tmpResult.replace(i,tmp);

        if(tmpResult.at(i)<0)
        {
            tmpResult.replace(i,0);
        }
    }

    rebinCounts(tmpResult,vCounts,dynamicRange);
    setIntensityRange();
    buildCountRate();

    return *this;
}


mySpectra mySpectra::operator * (const double RHS)
{
    mySpectra temp = *this;
    temp *= RHS;
    return temp;
}

mySpectra& mySpectra::operator *= (const double RHS)
{
    spectraActualTime *= RHS;
    spectraPresetTime *= RHS;
    setDeadTime();

    for(unsigned int i=0; i<dynamicRange; i++)
    {
        vCounts.replace(i,(vCounts.at(i)*RHS));
    }

    setIntensityRange();
    buildCountRate();

    return *this;
}

mySpectra mySpectra::operator / (const mySpectra& RHS)
{
    mySpectra temp = *this;
    temp /= RHS;
    return temp;
}

mySpectra& mySpectra::operator /= (const mySpectra& RHS)
{
    spectraSoftwareVersion = "Spectra Divison";

    unsigned int tmpDynamicRange = minDR(dynamicRange, RHS.dynamicRange);

    QVector<double> tmpResult(tmpDynamicRange);
    QVector<double> tmpLeft(tmpDynamicRange);
    QVector<double> tmpRight(tmpDynamicRange);

    rebinCounts(vCounts,tmpLeft,tmpDynamicRange);
    rebinCounts(RHS.vCounts,tmpRight,tmpDynamicRange);

    double timeDiff = 1.0 * spectraPresetTime / RHS.spectraPresetTime;

    for(unsigned int i=0; i<tmpDynamicRange; i++)
    {
        if (tmpRight.at(i)>0)
        {
            double tmp = tmpLeft.at(i) / tmpRight.at(i) / timeDiff;
            tmpResult.replace(i,tmp);
        }
        else
        {
            tmpResult.replace(i,0);
        }
    }

    rebinCounts(tmpResult,vCounts,dynamicRange);
    setIntensityRange();
    buildCountRate();

    return *this;
}

mySpectra mySpectra::operator / (const double RHS)
{
    mySpectra temp = *this;
    temp /= RHS;
    return temp;
}

mySpectra& mySpectra::operator /= (const double RHS)
{
    spectraActualTime /= RHS;
    spectraPresetTime /= RHS;
    setDeadTime();

    for(unsigned int i=0; i<dynamicRange; i++)
    {
        double tmp = 1.0 * vCounts.at(i) / RHS;
        vCounts.replace(i,tmp);
    }

    setIntensityRange();
    buildCountRate();

    return *this;
}

// ----- Acessors -------------------------------------------------------------
unsigned int mySpectra::binSize()
{
    return dynamicRange;
}

double mySpectra::maxIntensity()
{
    return intensityRange;
}

QString mySpectra::fileName()
{
    return spectraFileName;
}

QString mySpectra::ID()
{
    return spectraID;
}

unsigned int mySpectra::detectorNumber()
{
    return spectraDetectorNumber;
}

QString mySpectra::detectorDescription()
{
    return spectraDetectorDescription;
}

QString mySpectra::softwareVersion()
{
    return spectraSoftwareVersion;
}

QDateTime mySpectra::runDateTime()
{
    return spectraRunDateTime;
}

unsigned int mySpectra::presetTime()
{
    return spectraPresetTime;
}

unsigned int mySpectra::actualTime()
{
    return spectraActualTime;
}

unsigned int mySpectra::deadTime()
{
    return spectraDeadTime;
}

unsigned int mySpectra::maxDR(unsigned int range1, unsigned int range2)
{
    unsigned int tmp = range1;
    if (range2>tmp)
    {
        tmp = range2;
    }
    return tmp;
}

unsigned int mySpectra::minDR(unsigned int range1, unsigned int range2)
{
    unsigned int tmp = range1;
    if (range2<tmp)
    {
        tmp = range2;
    }
    return tmp;
}

// ----- Mutators -------------------------------------------------------------
void mySpectra::setDeadTime()
{
    spectraDeadTime = spectraActualTime - spectraPresetTime;
    deadTimeFrac = 1.0 * spectraDeadTime / spectraPresetTime;
}

void mySpectra::setDynamicRange(unsigned int value)
{
    maxBin = value - 1;
    setDynamicRange();
}

void mySpectra::setDynamicRange()
{
    for(unsigned int i=0; i<DRValues; i++ )
    {
        if(maxBin<DRList.at(i))
        {
            dynamicRange = DRList.at(i);
            break;
        }
    }
    maxBin = dynamicRange - 1;
}

void mySpectra::setIntensityRange()
{
    intensityRange = 0;
    for(unsigned int i=0; i<dynamicRange; i++)
    {
        double tmp = vCounts.at(i);
        if (tmp > intensityRange)
        {
            intensityRange = tmp;
        }
    }
}

void mySpectra::resetCounts()
{
    vCounts.clear();
    vCounts.fill(0,dynamicRange);
}

void mySpectra::buildBins()
{
    vBins.clear();
    vBins.fill(0,dynamicRange);
    for(unsigned int i=0; i<dynamicRange; i++)
    {
        vBins.replace(i,i);
    }
}

void mySpectra::buildCountRate()
{
    vCountRate.clear();
    vCountRate.fill(0,dynamicRange);
    for(unsigned int i=0; i<dynamicRange; i++)
    {
        double tmp = 1.0 * vCounts.at(i) / spectraActualTime;
        vCountRate.replace(i,tmp);
    }
}

// ----- Distributions --------------------------------------------------------
void mySpectra::setUniformDistribution()
{
    buildBins();
    resetCounts();
    for(unsigned int i=0; i<dynamicRange; i++)
    {
        double tmp = 1.0 / dynamicRange;
        vCounts.replace(i, tmp);
    }
    buildCountRate();
}

void mySpectra::setRandomDistribution(unsigned int maxValue)
{
    buildBins();
    resetCounts();
    for(unsigned int i=0; i<dynamicRange; i++)
    {
        unsigned int tmp = rand() % maxValue;
        vCounts.replace(i, tmp);
    }
    buildCountRate();
}

void mySpectra::setNormalDistribution(double offset, double sigma)
{
    if(std::abs(offset) > 1.0) { offset = 0; }
    if(sigma<=0) { sigma=0.001; };

    double mu0 = maxBin / 2;
    double mu = mu0 * (1+offset);

    double sig2 = std::pow(sigma,2.0);
    double pi = std::acos(-1);

    buildBins();
    resetCounts();
    for(unsigned int i=0; i<dynamicRange; i++)
    {
        double tmp = 0;
        tmp = 1.0 / std::sqrt(2.0*sig2*pi);
        tmp = tmp * std::exp(-std::pow((i-mu),2.0)/(2.0*sig2));

        // Rescale normalized distribution to number of total counts
        unsigned int totalCounts = 100000000;
        tmp = 1.0 * tmp * totalCounts / dynamicRange;

        vCounts.replace(i,tmp);
    }
    buildCountRate();
}

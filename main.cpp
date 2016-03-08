/*
* Copyright (C) 2016  Integrated Computer Solutions, Inc
* 54 B Middlesex Turnpike, Bedford, MA 01730
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#if ! defined QT_NO_DEBUG_OUTPUT
#define QT_NO_DEBUG_OUTPUT
#endif
#include <QDebug>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>
#include <QJsonArray>

#include <QCoreApplication>
#include <QCommandLineParser>
#include "MediaInfo/MediaInfo.h"
#include "MediaInfo/MediaInfoList.h"

using namespace MediaInfoLib;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // Create Options for MediaInfo, there is a bunch more but these seem somewhat most relevant
    QStringList generalParams;
    generalParams << "CompleteName" << "FolderName" << "FileName" << "FileExtension"
                  << "FileSize" << "Duration" << "FrameRate" << "BitRate" << "BitRate_Mode/String"
                  << "Format" << "InternetMediaType"
                  << "Title" << "Season" << "Movie"
                  << "Album" << "Album_More" << "Album/Performer"
                  << "Part" << "Track" << "Track/Position" << "Compilation"
                  << "Performer" << "Genre" << "Mood"
                  << "Released_Date" << "Original/Released_Date" << "Recorded_Date"
                  << "Encoded_Application" << "Encoded_Library" << "BPM"
                  << "Cover" << "Cover_Mime" << "Lyrics"
                  << "Added_Date";
    QString generalInform;
    generalInform="General;";
    foreach(QString s, generalParams) {
        generalInform += QString("\%%1\%|").arg(s);
    }
    generalInform+="\\n";
    // Create a MediaInforList object, this is for parsing entire directories
    MediaInfoList MI;
    MI.Option(__T("ParseSpeed"), __T("0"));
    MI.Option(__T("Language"), __T("raw"));
    MI.Option(__T("ReadByHuman"), __T("0"));
    MI.Option(__T("Legacy"), __T("0"));

    // Setup some command line options
    QCommandLineParser cmdLine;
    QCommandLineOption parametersOptions(QStringList() << "p" << "params");
    cmdLine.addOption(parametersOptions);
    QCommandLineOption codecsOptions(QStringList() << "c" << "codecs");
    cmdLine.addOption(codecsOptions);
    QCommandLineOption outputOptions(QStringList() << "o" << "output",  QCoreApplication::translate("main", "Destination file."),"std");
    cmdLine.addOption(outputOptions);
    QCommandLineOption versionOptions(QStringList() << "v" << "version");
    cmdLine.addOption(versionOptions);
    cmdLine.addHelpOption();
    cmdLine.process(a);

    if(cmdLine.isSet(versionOptions)){
        QString infoVersion=QString::fromStdWString(MI.Option(__T("Info_Version"), __T("0.7.82;MediaInfo_Example;0.7.82")).c_str());
        qInfo() << "QIcsMediaInfo version 1.0 using MediaInforLib version:" << infoVersion;
        exit(0);
    }
    if(cmdLine.isSet(parametersOptions)){
        QString infoParameters=QString::fromStdWString( MI.Option(__T("Info_Parameters")));
        qInfo() << qPrintable("Info_Parameters:\n") << qPrintable(infoParameters);
        exit(0);
    }
    if(cmdLine.isSet(codecsOptions)){
        QString infoCodecs=QString::fromStdWString( MI.Option(__T("Info_Codecs")).c_str() );
        qInfo() << qPrintable("Codecs:\n") << qPrintable(infoCodecs);
        exit(0);
    }
    QString dirPath;
    if(cmdLine.positionalArguments().count() > 0)
        dirPath = cmdLine.positionalArguments()[0];
    if (dirPath.isNull()) {
        qWarning() << "Must specify directory to parse";
        exit(1);
    }
    QDir inDir(dirPath);
    if(!inDir.exists() || !inDir.isReadable()){
        qWarning() << dirPath << "does not exist or is not readable";
        exit(1);
    }
    QFile outFile;
    QTextStream outStream(stdout);
    //if -o and given a destination
    if(cmdLine.isSet(outputOptions) && cmdLine.value(outputOptions) != "std" ){
        outFile.setFileName(cmdLine.value(outputOptions));
        if(outFile.open(QIODevice::WriteOnly)){
            outStream.setDevice(&outFile);
        }
    }
    // This causes MediaInfo to open all files in the directory
    int nfiles = MI.Open(dirPath.toStdWString(),MediaInfoLib::FileOption_NoRecursive);
    qDebug() << Q_FUNC_INFO << "Opened " << nfiles << " files";
    // Now we query MediaInfoLib for the data we are interested and receive everything in one string
    MI.Option(QStringLiteral("Inform").toStdWString(), generalInform.toStdWString());
    QString informOptionResult=QString::fromStdWString(MI.Inform());
    // Done - be good and close the MediaInfo object
    MI.Close();

    // Trivial bit of QJson<Magic> and output
    QJsonArray audioFiles;
    QJsonArray videoFiles;
    QStringList informResult=informOptionResult.split('\n',QString::SkipEmptyParts);
    if (informOptionResult.isEmpty()) {
        qWarning() << "could not find audio or video files in:" << dirPath;
        exit(0);
    }
    QVariantMap resMap;
    foreach (QString res, informResult) {
        QStringList resList=res.split("|");
        Q_ASSERT((resList.count()-1)==generalParams.count());
        for (int i=0;i<resList.count()-1;++i) {
            resMap[generalParams[i]] = resList[i];
        }
        QJsonObject resObject=QJsonObject::fromVariantMap(resMap);
        QString mimeType=resMap["InternetMediaType"].toString();
        if (mimeType.startsWith("audio")) audioFiles.append(resObject);
        else if (mimeType.startsWith("video")) videoFiles.append(resObject);
        else {
            qWarning() << Q_FUNC_INFO << "mimetype for file" << resMap["CompleteName"]<< "not one of audio or video but" << resMap["InternetMediaType"];
        }
    }
    QJsonObject jsonObject;
    if (!audioFiles.isEmpty()) jsonObject.insert("audio",audioFiles);
    if (!videoFiles.isEmpty()) jsonObject.insert("video",videoFiles);
    QJsonDocument jsonDoc(jsonObject);
    outStream << qPrintable(jsonDoc.toJson());
}

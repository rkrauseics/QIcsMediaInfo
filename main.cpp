#if ! defined QT_NO_DEBUG_OUTPUT
//#define QT_NO_DEBUG_OUTPUT
#endif
#include <QTextStream>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>
#include <QRegularExpression>

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QTimer>
#include "MediaInfo/MediaInfo.h"
#include "MediaInfo/MediaInfoList.h"

using namespace MediaInfoLib;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int nfiles  = 0;

    QMap<QString, QStringList> parametersMap;
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
    parametersMap.insert("General",generalParams);
    QString generalInform;
    generalInform="General;";
    foreach(QString s, generalParams) {
        generalInform += QString("\%%1\%:").arg(s);
    }
    generalInform+="\\n";

    QStringList videoParams;
    videoParams << "Format"<<"BitRate" << "BitRate_Mode/String"
                << "Width" << "Height" << "DisplayAspectRatio/String" << "Rotation/String"
                << "FrameRate" << "BitDepth" << "Title" << "Language/String";
    parametersMap.insert("Video",videoParams);
    QString videoInform;
    videoInform="Video;";
    foreach(QString s, videoParams) {
        videoInform += QString("\%%1\%:").arg(s);
    }
    videoInform+="\\n";

    QStringList audioParams;
    audioParams << "Format" << "BitRate" << "BitRate_Mode/String"
                << "Channel(s)" << "SamplingRate" << "StreamSize"
                << "Title" << " Language/String";
    parametersMap.insert("Audio",audioParams);
    QString audioInform;
    audioInform="Audio;";
    foreach(QString s, audioParams) {
        audioInform += QString("\%%1\%:").arg(s);
    }
    audioInform+="\\n";

    MediaInfoList MI;
    QMap<QString, QString> infoMap;
    MI.Option(__T("ParseSpeed"), __T("0"));
    MI.Option(__T("Language"), __T("raw"));
    MI.Option(__T("ReadByHuman"), __T("0"));
    MI.Option(__T("Legacy"), __T("0"));

//    QString infoVersion=QString::fromStdWString(MI.Option(__T("Info_Version"), __T("0.7.82;MediaInfo_Example;0.7.82")).c_str());
//    qDebug() << Q_FUNC_INFO << "Info_Version" << infoVersion;

    // TODO:
    QCommandLineParser cmdLine;
    QCommandLineOption parametersOptions(QStringList() << "p" << "params");
    cmdLine.addOption(parametersOptions);
    QCommandLineOption codecsOptions(QStringList() << "c" << "codecs");
    cmdLine.addOption(codecsOptions);
    QCommandLineOption generalOptions("g");
    cmdLine.addOption(generalOptions);
    QCommandLineOption soundOptions("s");
    cmdLine.addOption(soundOptions);
    QCommandLineOption videoOptions("v");
    cmdLine.addOption(videoOptions);
    QCommandLineOption allOptions("a");
    cmdLine.addOption(allOptions);
    QCommandLineOption outputOptions(QStringList() << "o" << "output",  QCoreApplication::translate("main", "Destination file."),"std");
    cmdLine.addOption(outputOptions);

    cmdLine.process(a);

    // if --params
    if(cmdLine.isSet(parametersOptions)){
        QString infoParameters=QString::fromStdWString( MI.Option(__T("Info_Parameters")));
        qDebug() << Q_FUNC_INFO << "\r\n\r\nInfo_Parameters\r\n" << qPrintable(infoParameters);
    }
    // if --codecs
    if(cmdLine.isSet(codecsOptions)){
        QString infoCodecs=QString::fromStdWString( MI.Option(__T("Info_Codecs")).c_str() );
        qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nInfo_Codecs\r\n") << qPrintable(infoCodecs);
    }

//    qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nOpen\r\n");
    QString dirPath ="";
    if(cmdLine.positionalArguments().count() > 0)
        dirPath = cmdLine.positionalArguments()[0];

    QDir inDir(dirPath);
    if(!inDir.exists()){
        MI.Close();
        qFatal( dirPath.toLocal8Bit() + " does not exist");
    }
    else{

        QFile outFile;
        QTextStream outStream(stderr);
        //if -o and given a destination
        if(cmdLine.value(outputOptions) != "std" ){
            outFile.setFileName(cmdLine.value(outputOptions));
            if(outFile.open(QIODevice::WriteOnly)){
                outStream.setDevice(&outFile);
            }
        }

        nfiles = MI.Open(dirPath.toStdWString(),MediaInfoLib::FileOption_NoRecursive);
        outStream << Q_FUNC_INFO << "Opened " << nfiles << " files";

//        qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nInform with Complete=false\r\n");
//        MI.Option(__T("Complete"));
//        QString standardInform=QString::fromStdWString(MI.Inform().c_str());
//        qDebug() << Q_FUNC_INFO << qPrintable(standardInform);


//        MI.Option(__T("Complete"), __T("1"));
//        QString completeInform=QString::fromStdWString(MI.Inform().c_str());
//        qDebug() << Q_FUNC_INFO << qPrintable(completeInform);

        // if -a || -g
        if(cmdLine.isSet(generalOptions) || cmdLine.isSet(allOptions)){
            MI.Option(QStringLiteral("Inform").toStdWString(), generalInform.toStdWString());
            QString informOptionExample=QString::fromStdWString(MI.Inform());
            infoMap.insert("General", informOptionExample);
//            qDebug() << qPrintable("\r\n\r\nGeneral Inform\r\n") << qPrintable(informOptionExample);
        }

        // if -a || -v
        if(cmdLine.isSet(videoOptions) || cmdLine.isSet(allOptions)){
            MI.Option(QStringLiteral("Inform").toStdWString(), videoInform.toStdWString());
            QString videoOptionExample=QString::fromStdWString(MI.Inform());
            infoMap.insert("Video", videoOptionExample);
//            qDebug() << qPrintable("\r\n\r\nVideo Inform\r\n") << qPrintable(videoOptionExample);
        }

        // if -a || -s
        if(cmdLine.isSet(soundOptions) || cmdLine.isSet(allOptions)){
            MI.Option(QStringLiteral("Inform").toStdWString(), audioInform.toStdWString());
            QString audioOptionExample=QString::fromStdWString(MI.Inform());
            infoMap.insert("Audio", audioOptionExample);
//            qDebug() << qPrintable("\r\n\r\nAudio Inform\r\n") << qPrintable(audioOptionExample);
        }


        QJsonDocument outDoc;
        foreach(QString section, infoMap.keys())
        {
            QStringList informResult = infoMap[section].split(QRegularExpression("\r?\n"));
            if(informResult.last().isEmpty())
                informResult.pop_back();
            QVariantMap resMap;

            outStream << qPrintable(section) <<" Information";

            foreach (QString res, informResult) {
                QStringList resList = res.split(QRegularExpression(":(?!\\\\)"));
                if(resList.last().isEmpty())
                    resList.pop_back();
                Q_ASSERT(resList.count() == parametersMap[section].count());
                for (int i = 0;i < parametersMap[section].count(); i++) {
                    resMap[parametersMap[section][i]] = resList[i];
                }
                QJsonObject resObject=QJsonObject::fromVariantMap(resMap);
                QString mimeType=resMap["InternetMediaType"].toString().toLower();
                if(!mimeType.contains("audio") && !mimeType.contains("video")){
                    qWarning() << Q_FUNC_INFO << "mimetype for file" << resMap["CompleteName"]<< "not one of audio or video but" << resMap["InternetMediaType"].toString();
                }
                else{
                    outDoc.setObject(resObject);
                        outStream << qPrintable(outDoc.toJson());

                }
            }
        }
    }

//        QString generalFileSize=QString::fromStdWString(MI.Get(1,Stream_General, 0, __T("FileSize"), Info_Text, Info_Name));
//        qDebug() << Q_FUNC_INFO << "General: FileSize:" << generalFileSize;

    MI.Close();
    QTimer::singleShot(nfiles * 100, &a, SLOT(quit()));
    return a.exec();
}

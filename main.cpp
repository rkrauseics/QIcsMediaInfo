#if ! defined QT_NO_DEBUG_OUTPUT
//#define QT_NO_DEBUG_OUTPUT
#endif
#include <QDebug>

#include <QCoreApplication>

#include "MediaInfo/MediaInfo.h"
#include "MediaInfo/MediaInfoList.h"

using namespace MediaInfoLib;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QStringList generalParams;
    generalParams << "CompleteName" << "FolderName" << "FileName" << "FileExtension"
                  << "FileSize" << "Duration" << "FrameRate" << "BitRate" << "BitRate_Mode/String"
                  << "Format" << "InternetMediaType"
                  << "Title" << "Season" << "Movie"
                  << "Album" << "Album_More" << "Album/Performer"
                  << "Part" << "Track" << "Track/Position" << "Compilation"
                  << "Performer" << "Genre" << "Mood\\n%"
                  << "Released_Date" << "Original/Released_Date" << "Recorded_Date"
                  << "Encoded_Application" << "Encoded_Library" << "BPM"
                  << "Cover" << "Cover_Mime" << "Lyrics"
                  << "Added_Date";

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
    QString audioInform;
    audioInform="Audio;";
    foreach(QString s, audioParams) {
        audioInform += QString("\%%1\%:").arg(s);
    }
    audioInform+="\\n";

    MediaInfoList MI;

    MI.Option(__T("ParseSpeed"), __T("0"));
    MI.Option(__T("Language"), __T("raw"));
    MI.Option(__T("ReadByHuman"), __T("0"));
    MI.Option(__T("Legacy"), __T("0"));

    QString infoVersion=QString::fromStdWString(MI.Option(__T("Info_Version"), __T("0.7.82;MediaInfo_Example;0.7.82")).c_str());
    qDebug() << Q_FUNC_INFO << "Info_Version" << infoVersion;

    // TODO:
    // Commandline options for
    // -p --params
    // -c --codecs
    // -g general -s sound -v video -a all
    // output is json
    // argument is a directory
    //
// if --params
//        QString infoParameters=QString::fromStdWString( MI.Option(__T("Info_Parameters")));
//        qDebug() << Q_FUNC_INFO << "\r\n\r\nInfo_Parameters\r\n" << qPrintable(infoParameters);

// if --codecs
//        QString infoCodecs=QString::fromStdWString( MI.Option(__T("Info_Codecs")).c_str() );
//        qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nInfo_Codecs\r\n") << qPrintable(infoCodecs);

        qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nOpen\r\n");
        const int nfiles=MI.Open(__T("/Volumes/rkrause/mm2_test"));
        qDebug() << Q_FUNC_INFO << "Opened" << nfiles << "files";

//        qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nInform with Complete=false\r\n");
//        MI.Option(__T("Complete"));
//        QString standardInform=QString::fromStdWString(MI.Inform().c_str());
//        qDebug() << Q_FUNC_INFO << qPrintable(standardInform);


//        MI.Option(__T("Complete"), __T("1"));
//        QString completeInform=QString::fromStdWString(MI.Inform().c_str());
//        qDebug() << Q_FUNC_INFO << qPrintable(completeInform);

        // if -a || -g
        MI.Option(QStringLiteral("Inform").toStdWString(), generalInform.toStdWString());
        QString informOptionExample=QString::fromStdWString(MI.Inform());
        qDebug() << qPrintable("\r\n\r\nGeneral Inform\r\n") << qPrintable(informOptionExample);

        // if -a || -v
        MI.Option(QStringLiteral("Inform").toStdWString(), videoInform.toStdWString());
        QString videoOptionExample=QString::fromStdWString(MI.Inform());
        qDebug() << qPrintable("\r\n\r\nVideo Inform\r\n") << qPrintable(videoOptionExample);

        // if -a || -s
        MI.Option(QStringLiteral("Inform").toStdWString(), audioInform.toStdWString());
        QString audioOptionExample=QString::fromStdWString(MI.Inform());
        qDebug() << qPrintable("\r\n\r\nAudio Inform\r\n") << qPrintable(audioOptionExample);

        //
//        QString generalFileSize=QString::fromStdWString(MI.Get(1,Stream_General, 0, __T("FileSize"), Info_Text, Info_Name));
//        qDebug() << Q_FUNC_INFO << "General: FileSize:" << generalFileSize;

        MI.Close();

//    return a.exec();
}


//nfiles=MI.Open(dirPath.toStdWString(),MediaInfoLib::FileOption_NoRecursive);
//QStringList informResult=informOptionExample.split('\n',QString::SkipEmptyParts);
//QVariantMap resMap;
//foreach (QString res, informResult) {
//    qDebug() << Q_FUNC_INFO << res;
//    QStringList resList=res.split(":");
////        qDebug() << resList.count() << generalParams.count();
//    Q_ASSERT((resList.count()-1)==generalParams.count());
//    for (int i=0;i<resList.count()-1;++i) {
//        qDebug() << generalParams[i] << ":" << resList[i];
//        resMap[generalParams[i]] = resList[i];
//    }
//    QJsonObject resObject=QJsonObject::fromVariantMap(resMap);
//    QString mimeType=resMap["InternetMediaType"].toString();
//    if (mimeType.startsWith("audio")) m_audioFiles.append(resObject);
//    else if (mimeType.startsWith("video")) m_videoFiles.append(resObject);
//    else {
//        qWarning() << Q_FUNC_INFO << "mimetype for file" << resMap["CompleteName"]<< "not one of audio or video but" << resMap["InternetMediaType"];
//    }
//    qDebug() << Q_FUNC_INFO << "resObject" << resObject;
//}

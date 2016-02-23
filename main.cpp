/*  Copyright (c) MediaArea.net SARL. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license that can
 *  be found in the License.html file in the root of the source tree.
 */

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// Example for MediaInfoLib
// Command line version
//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#if ! defined QT_NO_DEBUG_OUTPUT
//#define QT_NO_DEBUG_OUTPUT
#endif
#include <QDebug>

#include <QCoreApplication>

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

//#include "ZenLib/Ztring.h" //Note : I need it for universal atoi, but you have not to use it for be able to use MediaInfoLib
//#include "MediaInfoDLL/MediaInfoDLL_Static.h"
#include "MediaInfo/MediaInfo.h"
#include "MediaInfo/MediaInfoList.h"

//using namespace MediaInfoDLL;
using namespace MediaInfoLib;
//using namespace ZenLib;

//#define USE_STD_STRING 1

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


        //Information about MediaInfo
        MediaInfoList MI;

        MI.Option(__T("ParseSpeed"), __T("0"));
        MI.Option(__T("Language"), __T("raw"));
        MI.Option(__T("ReadByHuman"), __T("0"));
        MI.Option(__T("Legacy"), __T("0"));

#if USE_STD_STRING==1
        String To_Display=MI.Option(__T("Info_Version"), __T("0.7.82;MediaInfo_Example;0.7.82")).c_str();
        //        ZenLib::Ztring To_Display=MI.Option(__T("Info_Version"), __T("0.7.0.0;MediaInfoDLL_Example_MSVC;0.7.0.0")).c_str();
#endif
        QString infoVersion=QString::fromStdWString(MI.Option(__T("Info_Version"), __T("0.7.82;MediaInfo_Example;0.7.82")).c_str());
        qDebug() << Q_FUNC_INFO << "Info_Version" << infoVersion;

#if USE_STD_STRING==1
        To_Display += __T("\r\n\r\nInfo_Parameters\r\n");
        To_Display += MI.Option(__T("Info_Parameters")).c_str();
#endif
//        QString infoParameters=QString::fromStdWString( MI.Option(__T("Info_Parameters")));
//        qDebug() << Q_FUNC_INFO << "\r\n\r\nInfo_Parameters\r\n" << qPrintable(infoParameters);

#if USE_STD_STRING==1
        To_Display += __T("\r\n\r\nInfo_Capacities\r\n");
        To_Display += MI.Option(__T("Info_Capacities")).c_str();
#endif
//        QString infoCapacities=QString::fromStdWString( MI.Option(__T("Info_Capacities")).c_str() );
//        qDebug() << Q_FUNC_INFO << "infoCapacities" << qPrintable(infoCapacities);

#if USE_STD_STRING==1
        To_Display += __T("\r\n\r\nInfo_Codecs\r\n");
        To_Display += MI.Option(__T("Info_Codecs")).c_str();
#endif
//        QString infoCodecs=QString::fromStdWString( MI.Option(__T("Info_Codecs")).c_str() );
//        qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nInfo_Codecs\r\n") << qPrintable(infoCodecs);

        qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nOpen\r\n");
        const int nfiles=MI.Open(__T("/Volumes/rkrause/mm2_test"));
        qDebug() << Q_FUNC_INFO << "Opened" << nfiles << "files";
//        qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nInform with Complete=false\r\n");
//        MI.Option(__T("Complete"));
//        QString standardInform=QString::fromStdWString(MI.Inform().c_str());
//        qDebug() << Q_FUNC_INFO << qPrintable(standardInform);

#if USE_STD_STRING==1
        //An example of how to use the library
        To_Display += __T("\r\n\r\nOpen\r\n");
//        MI.Open(__T("Example.ogg"));
        MI.Open(__T("/Volumes/PATRIOT/mm_test/t1.mp3"));
        To_Display += __T("\r\n\r\nInform with Complete=false\r\n");
        MI.Option(__T("Complete"));
        To_Display += MI.Inform().c_str();
#endif

//        MI.Option(__T("Complete"), __T("1"));
//        QString completeInform=QString::fromStdWString(MI.Inform().c_str());
//        qDebug() << Q_FUNC_INFO << qPrintable(completeInform);

#if USE_STD_STRING==1
        To_Display += __T("\r\n\r\nInform with Complete=true\r\n");
        MI.Option(__T("Complete"), __T("1"));
        To_Display += MI.Inform().c_str();
#endif


#if USE_STD_STRING==1

        To_Display += __T("\r\n\r\nCustom Inform\r\n");
        MI.Option(__T("Inform"), __T("General;Example : FileSize=%FileSize%"));
        To_Display += MI.Inform().c_str();
#endif
        MI.Option(__T("Inform"), __T("General;%CompleteName%:%FolderName%:%FileName%:%FileExtension%:\\n"
                                     "%FileSize%:%Duration%:%FrameRate%:%BitRate%:%BitRate_Mode/String%:\\n"
                                     "%Format%:%InternetMediaType%:\\n"
                                     "%Title%:%Album%:%Album_More%:%Album/Performer%:\\n"
                                     "%Part%:%Track%:%Track/Position%:%Compilation%:\\n"
                                     "%Performer%:%Genre%:%Mood\\n%"
                                     "%Released_Date%:%Original/Released_Date%:%Recorded_Date%:\\n"
                                     "%Encoded_Application%:%Encoded_Library%:%BPM%:\\n"
                                     "%Cover%:%Cover_Mime%:%Lyrics%:\\n"
                                     "%Added_Date%:\\n"));
        QString informOptionExample=QString::fromStdWString(MI.Inform());
        qDebug() << qPrintable("\r\n\r\nCustom Inform\r\n") << qPrintable(informOptionExample);

        MI.Option(__T("Inform"), __T("Video;%Format%,%BitRate%:%BitRate_Mode/String%\\n"
                                     "%Width%:%Height%:%DisplayAspectRatio/String%:%Rotation/String%:\\n"
                                     "%FrameRate%:%BitDepth%:\\n"
                                     "%Title%:%Language/String%:\\n"));
        QString videoOptionExample=QString::fromStdWString(MI.Inform());
        qDebug() << qPrintable("\r\n\r\nVideo Inform\r\n") << qPrintable(videoOptionExample);

        MI.Option(__T("Inform"), __T("Audio;%Format%,%BitRate%:%BitRate_Mode/String%\\n"
                                     "%Channel(s)%:\\n"
                                     "%SamplingRate%:%StreamSize%:\\n"
                                     "%Title%:%Language/String%:\\n"));
        QString audioOptionExample=QString::fromStdWString(MI.Inform());
        qDebug() << qPrintable("\r\n\r\nAudio Inform\r\n") << qPrintable(audioOptionExample);

//        QString generalFileSize=QString::fromStdWString(MI.Get(1,Stream_General, 0, __T("FileSize"), Info_Text, Info_Name));
//        qDebug() << Q_FUNC_INFO << "General: FileSize:" << generalFileSize;

#if USE_STD_STRING==1
        To_Display += __T("\r\n\r\nGet with Stream=General and Parameter=\"FileSize\"\r\n");
        To_Display += MI.Get(Stream_General, 0, __T("FileSize"), Info_Text, Info_Name).c_str();
#endif

#if 0
        To_Display += __T("\r\n\r\nGetI with Stream=General and Parameter=46\r\n");
        To_Display += MI.Get(Stream_General, 0, 46, Info_Text).c_str();

//        To_Display += __T("\r\n\r\nCount_Get with StreamKind=Stream_Audio\r\n");
//        To_Display += ZenLib::Ztring::ToZtring(MI.Count_Get(Stream_Audio, -1)); //Warning : this is an integer
        toStringStream SS;
        SS << std::setbase(10) << MI.Count_Get(Stream_Audio);
        To_Display += SS.str();

        To_Display += __T("\r\n\r\nGet with Stream=General and Parameter=\"AudioCount\"\r\n");
        To_Display += MI.Get(Stream_General, 0, __T("AudioCount"), Info_Text, Info_Name).c_str();

        To_Display += __T("\r\n\r\nGet with Stream=Audio and Parameter=\"StreamCount\"\r\n");
        To_Display += MI.Get(Stream_Audio, 0, __T("StreamCount"), Info_Text, Info_Name).c_str();

        To_Display += __T("\r\n\r\nClose\r\n");
#endif
        MI.Close();

//        std::cout<<To_Display.To_Local().c_str()<<std::endl;
#if USE_STD_STRING==1
#ifdef UNICODE
    std::wcout << To_Display << std::endl;
#else
    std::cout  << To_Display << std::endl;
#endif

#endif

    //---------------------------------------------------------------------------

//    return a.exec();
}


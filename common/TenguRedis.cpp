// ********************************************************************************************************************
// *                 Нашлепка сверху на LoRedis, которая дополняет его специфическими для Tengu функциями             *
// *                                                                                                                  *
// *                                                                          Eugene G. Sysoletin <unclesal@mail.ru>, *
// *                                                       Ural Federal University, the Institute of radioelectronics *
// *                                                                        and information tehnology, IT department. *
// *                                                                                                                  *
// *                                                                                     created 24 apr 2017 at 19:36 *
// ********************************************************************************************************************

#include "TenguRedis.h"

using namespace std;
using namespace cv;

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  Конструктор.                                                    *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::TenguRedis::TenguRedis() 
    : LoRedis() 
{

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                            Публикация изображения.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::TenguRedis::publish ( QString channel, cv::Mat & frame ) {
    
    QString errorHere("Rediser::publish( QString, cv::Mat): ");
    if ( _asyncContext ) {
        try {
            FileStorage fs(".xml", FileStorage::WRITE + FileStorage::MEMORY );
            fs << "Mat" << frame;
            string data = fs.releaseAndGetString();
            LoRedis::publish( channel, data.c_str() );
            
        } catch ( exception & e ) {
            emit signalError( errorHere + QString( e.what() ) );
        } catch ( ... ) {
            emit signalError( errorHere + QString("неизвестная ошибка.") );
        };
    } else emit signalError( errorHere + QString("попытка публикации в не-инициализированный контекст.") );
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                  Деструктор.                                                     *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::TenguRedis::~TenguRedis() {

}



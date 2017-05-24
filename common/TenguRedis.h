// ********************************************************************************************************************
// *                 Нашлепка сверху на LoRedis, которая дополняет его специфическими для Tengu функциями             *
// *                                                                                                                  *
// *                                                                          Eugene G. Sysoletin <unclesal@mail.ru>, *
// *                                                       Ural Federal University, the Institute of radioelectronics *
// *                                                                        and information tehnology, IT department. *
// *                                                                                                                  *
// *                                                                                     created 24 apr 2017 at 18:34 *
// ********************************************************************************************************************

#pragma once

#include "opencv2/opencv.hpp"
#include <string>
#include <iostream>

#include "LoRedis.h"

namespace tengu {

    class TenguRedis : public LoRedis {
        
        Q_OBJECT
        
        public:
            TenguRedis();
            virtual ~TenguRedis();
            void publish( QString channel, cv::Mat & frame );

    };
};

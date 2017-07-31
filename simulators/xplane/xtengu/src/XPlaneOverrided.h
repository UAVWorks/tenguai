// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         X-Plane's control overrided elements                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                        Перекрытые элементы управления X-Plane                                    *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 06 may 2017 at 13:24 *
// ********************************************************************************************************************

#pragma once

#include "XPlaneChannel.h"

namespace tengu {

    // Class for overriding the aircraft control from X-Plane.  If we want to control of the aircrafts from the plug-in, 
    // we need to disable several X-Plane variables. This will provide the ability to manage and eliminate conflicts, 
    // we manages only, but not X-Plane.
    
    // Класс для перекрытия управления самолетом со стороны X-Plane. Если мы хотим управлять самолетами из плагина,
    // нам нужно запретить несколько переменых X-Plane. Кроме самой возможности управления, это еще и предотвратит
    // конфликты: управляем - только мы, но не X-Plane.
    
    class XPlaneOverrided {
        
        public:
            
            XPlaneOverrided();
            ~XPlaneOverrided();
            void override();
            void release();
            bool isOverrided();
        
        private:
            
            // ------------------------------------------------------------------------------------
            //                     Variables for take away the control from X-Plane.
            //                      Переменные для отбирания управления от X-Plane.
            // ------------------------------------------------------------------------------------
            
            // Override joystic control.
            // Перекрытие управления при помощи джойстика.
            
            XPlaneChannel __joystick;
            
            // The artifical stability system.
            // Интеллектуальная система стабилизации
            
            XPlaneChannel __artstab;
            
            // The embedded flight control system.
            // Встроенная система управления полетом.
            
            XPlaneChannel __flight_control;
            
            // The autopilot itself.
            // Автопилот как таковой.
            
            XPlaneChannel __autopilot;
            
            // The embeded engine speed (number of rotation per second) control system. 
            // Встроенная система управления оборотами двигателя.
            
            XPlaneChannel __throttle;
            
            // The possibility of arbitrarily (as client wish) motion control surfaces
            // Возможность произвольного (как угодно) перемещения управляющих поверхностей.
            
            XPlaneChannel __control_surfaces;
            
            bool __overrided;
            
    };
    
}; // namespace tengu

// ********************************************************************************************************************
// *                                     Определения класса цифрового pid-регулятора.                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                      Created 26 mar 2017 at 14:44  *
// ********************************************************************************************************************

#pragma once

#include <math.h>

#include <QString>
#include <QDebug>

#include "onboard_common.h"

// Интегрирование идет по некоему окну, определяем размерность этого окна.
#define PID_INTEGRAL_WINDOW_SIZE 5

// #define TUNING_SERIA_SIZE 100

// Максимально допустимое число итераций при настройке. На самом
// деле должно выйти раньше.
// #define TUNING_MAX_ITERATIONS 10000

// #define SERVO_INERTIA 0.5

namespace tengu {

    /**
     * @short ПИД-регулятор.
     * 
     * Регулятор - штука безразмерная по времени, он просто делает следующий шаг. Величина "шага" определяется
     * коэффициентами регулятора и содержанием окна интегрирования. Привязка регулятора ко времени
     * проводится выше, например, в классе Servo.
     */

    class PID {
        
        friend class AbstractRegulator;

        public:

            // typedef float seria_t [TUNING_SERIA_SIZE][2];

            struct pid_description_t {
                float P;
                float I;
                float D;
                bool reverse;
                // float inertia;
            };

            /**
             * @short Структура с показаниями качества регулирования по серии отсчетов.
             * 
             */
            
            struct indicators_t {

                /**
                 * @short Среднеквадратичное отклонение фактическог значения от желаемого.
                 */
                float sq_deviation;

                /**
                 * @short Максимальный процент перерегулирования.
                 */

                float max_overregulate;

                /**
                 * @short Максимальная производная (разность между соседними значениями)
                 */

                float max_difference;
            };

            PID( QString section, float output_value, float min_output_value, float max_output_value );
            PID( pid_description_t desc );
            ~PID();

            /**
             * @short Вернуть принятые коэффициенты PIDа.
             * 
             * Используется после настройки, чтобы записать коэффициенты.
             */

            pid_description_t configuration();

            float step( float input_value, float desired_value );

            // bool tuning( servos_one_channel_settings_t & ocs  );

            /**
             * @short Сформировать настроечную серию, посчитать ее показатели и выдать их наружу. 
             * 
             * "Настроечная сериия" - это два перехода сигнала, из минимума в максимум и наоборот. 
             * Иными словами, это две реакции на единичное воздействие. После формирования такой
             * серии считаются индикаторы, показатели качества регулирования. И этот насчитанный
             * результат выдается вызывающей программе.
             */

            // indicators_t rate_quality( servos_one_channel_settings_t & ocs );

        private:

            void __apply_parameters( pid_description_t params );
            
            float __min_value;
            float __max_value;
            
            /**
             * @short Текущая настройка регулятора (коэффициенты).
             */

            pid_description_t _params;
            // seria_t _seria;
            float _position;
            float _velocity;
            float _position_igral[ PID_INTEGRAL_WINDOW_SIZE ];
            float _velocity_igral[ PID_INTEGRAL_WINDOW_SIZE ];

            /**
             * @short Обнуление "серии", массива, используемого для настройки.
             */

            // void _seria_to_null();
            void __set_steady_state( float output_value );

            // indicators_t _make_indicators( servos_one_channel_settings_t & settings, bool with_out = false );
            // void _make_seria ( bool limit, servos_one_channel_settings_t & settings, bool with_out = false );
            // float _calculate_deviation( bool limit, float step, float have_deviation, servos_one_channel_settings_t & settings, bool p, bool i, bool d );
            // bool _coefficient_adopted( bool limit, float haved_deviation, servos_one_channel_settings_t & settings, bool with_out = false );
            // void _try_step_on_single_coefficient ( bool limit, float & coefficient, float step, float have_deviation, servos_one_channel_settings_t & settings );
            // int _tuning_channel_by_deviation( bool limit, float break_difference, float & have_deviation, servos_one_channel_settings_t & settings, bool p, bool i, bool d );
    };
};


// ********************************************************************************************************************
// *                                        Digital PID-controller implementation                                     *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                      Реализация класса цифрового pid-регулятора.                                 *
// *                                                                                                                  *
// * Eugene G. Sysoletin <unclesal@mail.ru>                                             Created 26 mar 2017 at 14:44  *
// ********************************************************************************************************************

#include "PID.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    Конструктор.                                                  *
// *                                                                                                                  *
// ********************************************************************************************************************
/*
ximi_qnx::PID::PID ( ximi_qnx::PID::coefficients_t & coefficients, float output_value ) {
    _coeffs = coefficients;
    _seria_to_null();
    _set_steady_state( output_value );
}
*/

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                         Конструктор от класса установок.                                         *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::PID::PID ( pid_description_t descr ) 
{
    _seria_to_null();

    _params.P = descr.P;
    _params.I = descr.I;
    _params.D = descr.D;
    _params.reverse = descr.reverse;
    
    // _coeffs.inertia = ocs.inertia;

    //if ( ocs.reverse ) {
    //    // Разумеется, "реверс" инерции не касается.
    //    _coeffs.P = - _coeffs.P;
    //    _coeffs.I = - _coeffs.I;
    //    _coeffs.D = - _coeffs.D;
    //};

    _set_steady_state( 0.0 );

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                        Обнуление "серии" - массива, используемого для настройки регулятора.                      *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::PID::_seria_to_null() {

    for ( int i=0; i<TUNING_SERIA_SIZE; i++ )
    for ( int j=0; j<2; j++ ) {
        _seria[i][j] = 0.0;
    };

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                   Вводим регулятор в установившийся режим с данным значением выхода регулятора.                  *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::PID::_set_steady_state ( float output_value ) {
    
    _position = output_value;
    _velocity = 0.0;
    
    // Установившийся режим: все окно интегрирования и дифференцирования заполняется 
    // теми же самыми значениями, что и на выходе регулятора.
    
    for ( int i=0; i<XIMI_SERVOS_PID_INTEGRAL_WINDOW_SIZE; i++ ) {
        _position_igral[i] = output_value;
        _velocity_igral[i] = 0.0;
    };
    
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                              Один "шаг" регулятора.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

float tengu::PID::step ( float input_value, float max_velocity ) {

    // Весовой коэффициент от пропорциональной составляющей.
    float error = _position - input_value;

    // Суммарная интегральная составляющая по окну интегрирования.
    float position_igral_value = 0.0;
    for ( int i=0; i<XIMI_SERVOS_PID_INTEGRAL_WINDOW_SIZE; i++ ) {
        position_igral_value += _position_igral[i] - input_value;
    };

    // Суммарная дифференциальная составляющая.
    float velocity_igral_value = 0.0;
    for ( int i=0; i<XIMI_SERVOS_PID_INTEGRAL_WINDOW_SIZE; i++ ) {
        velocity_igral_value += _velocity_igral[i];
    };

    // Полученная разница, на которую изменится выход.
    float delta = _coeffs.P * error + _coeffs.I * position_igral_value + _coeffs.D * velocity_igral_value; 

    // Желаемое значение выхода.
    float wanted_position = _position + delta;
    // Получившаяся скорость вращения.
    float wanted_speed = wanted_position - _position;
    // Ускорение вращения.
    float wanted_rate = wanted_speed - _velocity_igral[0];
    // Реальное ускорение с учетом инерции.
    wanted_rate = wanted_rate - ( wanted_rate * _coeffs.inertia );
    // Скорость.
    _velocity = _velocity + wanted_rate;

    // Конструктивное ограничение по скорости.
    if ( max_velocity ) {
        if ( _velocity > max_velocity ) {
            _velocity = max_velocity;
        }; 
        if ( _velocity < -max_velocity ) {
            _velocity = -max_velocity;
        };
    };

    // Пройденный путь, то есть текущая позиция..
    _position = _position + _velocity;

    // Сдвигаем окна таким образом, чтобы в нулевой элемент массивов можно было
    // засунуть текущее значение.
    for ( int i=XIMI_SERVOS_PID_INTEGRAL_WINDOW_SIZE-1; i>=1; i-- ) {
        _position_igral[i] = _position_igral[i-1];
        _velocity_igral[i] = _velocity_igral[i-1];
    };

    // Нулевой элемент - это текущее значение.
    _position_igral[0] = _position;
    _velocity_igral[0] = _velocity;

    // std::cout << "in the one step: P="<<_coeffs.P <<", I="<<_coeffs.I<<", D="<<_coeffs.D<< ", dp=" << dp << ", di=" << di << ", dd=" << dd << ", delta=" << delta << ", output="<<_position<<"\n";

    return _position;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                            Анализ полученного в ходе настройки регулятора массива данных.                        *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::PID::indicators_t tengu::PID::_make_indicators ( servos_one_channel_settings_t & settings, bool with_out ) {

    indicators_t result;
    result.sq_deviation = 0.0;
    result.max_difference = 0.0;
    result.max_overregulate = 0.0;

    float min_value = settings.min_value;
    float max_value = settings.max_value;

    for ( int i = 0; i<TUNING_SERIA_SIZE; i++ ) {

        // Разница между полученным выходом и желаемым значением.
        float plus_error = _seria[i][0] - max_value;
        float minus_error = _seria[i][1] - min_value;

        float plus_relativeError = plus_error * 100.0 / max_value;
        if ( plus_relativeError < 0 ) plus_relativeError = -1.0 * plus_relativeError;
        float minus_relativeError = minus_error * 100.0 / min_value;
        if ( minus_relativeError < 0 ) minus_relativeError = -1.0 * minus_relativeError;

        // Квадрат относительной разницы.
        float plus_sqd = plus_error * plus_error; // pow( difference, 2 );
        float minus_sqd = minus_error * minus_error;

        // Пока в девиации собираем - квадраты. Но в процентах.
        result.sq_deviation += plus_sqd + minus_sqd;

        // Отклонение между двумя соседними отсчетами (производная, скорость изменения).
        if ( i > 0 ) {

            float plus_sd = _seria[i][0] - _seria[ i-1 ][0];
            if ( plus_sd < 0 ) plus_sd = -plus_sd;

            float minus_sd = _seria[i][1] - _seria[ i-1 ][1];
            if ( minus_sd < 0 ) minus_sd = -minus_sd;

            if ( plus_sd > result.max_difference ) result.max_difference = plus_sd;
            if ( minus_sd > result.max_difference ) result.max_difference = minus_sd;
        };

        // Процент перерегулирования. Считается только в том случае, если
        // реальное значение превысило желаемое.

        if ( _seria[i][0] > max_value ) {
            if ( plus_relativeError > result.max_overregulate ) result.max_overregulate = plus_relativeError;
        };

        if ( _seria[i][1] < min_value ) {
            if ( minus_relativeError > result.max_overregulate ) result.max_overregulate = minus_relativeError;
        };

    };
    // Девиацию делаем "среднеквадратической".
    result.sq_deviation = result.sq_deviation / ( float ) ( TUNING_SERIA_SIZE * 2.0 );

    return result;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *               Публичная функция : сделать "серию" и посчитать показатели качества регулирования.                 *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::PID::indicators_t tengu::PID::rate_quality( servos_one_channel_settings_t & ocs ) {
    _make_seria( true, ocs );
    indicators_t indicators = _make_indicators( ocs );
    return indicators;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                         Сделать массив с отчетами при данных коэффициентах и оценить его.                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::PID::_make_seria ( bool limit, servos_one_channel_settings_t & settings, bool with_out ) {

    // if ( with_out ) std::cout << "--- Begin _make_seria(), P=" << _coeffs.P << ", I=" << _coeffs.I << ", D=" << _coeffs.D << "\n";

    float max_velocity = 0.0;
    if ( limit ) max_velocity = settings.desired_angular_velocity / ( float ) SERVO_TIMER_FREQUENCY ;
    // Восходящий проход.
    _set_steady_state( settings.min_value );

    _seria[0][0] = settings.min_value;
    for ( int iter=1; iter< TUNING_SERIA_SIZE; iter++ ) {
        _seria[iter][0] = step( settings.max_value, max_velocity );
        // if ( with_out )  std::cout << "ToMax:" << iter << ", position=" << _position << ", velocity=" << _velocity << " (" << _velocity*SERVO_TIMER_FREQUENCY << " deg/sec)\n";
    };

    // Нисходящий проход.
    _set_steady_state( settings.max_value );
    _seria[0][1] = settings.max_value;
    for ( int iter=1; iter<TUNING_SERIA_SIZE; iter ++ ) {
        _seria[iter][1] = step( settings.min_value, max_velocity );
        // if ( with_out ) std::cout << "ToMin:" << iter << ", position=" << _position << ", velocity=" << _velocity << " (" << _velocity*SERVO_TIMER_FREQUENCY  << " deg/sec)\n";
    };

}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                  Принимается ли данное состояние коэффициентов?                                  *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::PID::_coefficient_adopted( bool limit, float haved_deviation, servos_one_channel_settings_t & settings, bool with_out ) {

    _make_seria( limit, settings, with_out );
    indicators_t indicators = _make_indicators(settings, with_out );

    float allowable_difference = settings.desired_angular_velocity / (float) SERVO_TIMER_FREQUENCY;

    if ( indicators.max_difference > allowable_difference ) {
        // std::cout << "Seria rejected because the velocity " << indicators.max_difference * SERVO_TIMER_FREQUENCY << " deg/sec is very high\n";
        return false;
    };

    if ( indicators.sq_deviation > haved_deviation ) {
        // std::cout << "Seria rejected because the deviation larger\n";
        return false;
    };

    if ( indicators.max_overregulate > settings.desired_overregulate_percent ) {
        // std::cout << "Seria rejected because percent " << indicators.max_overregulate << " is very high\n";
        return false;
    };

    // std::cout << "Seria was accepted\n";
    return true;

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                 Приложить "шаг" к данному конкретному коэффициенту.                              *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::PID::_try_step_on_single_coefficient ( bool limit, float & coefficient, float step, float have_deviation, servos_one_channel_settings_t& settings ) {
    float old_coefficient = coefficient;
    coefficient += step;
    if ( ! _coefficient_adopted( limit, have_deviation, settings ) ) {
        // Коэффициент не принят.
        coefficient = old_coefficient - step;
        if ( ! _coefficient_adopted( limit, have_deviation, settings ) ) {
            // И снова не был принят, возвращаемся к исходному.
            coefficient = old_coefficient;
        }
    };
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                  Посчитать лучшее среднеквадратическое отклонение для варианта с этим шагом.                     *
// *                                                                                                                  *
// ********************************************************************************************************************

float tengu::PID::_calculate_deviation ( bool limit, float step, float have_deviation, servos_one_channel_settings_t & settings, bool p, bool i, bool d ) {

    if ( p ) {
        // std::cout << "calculate_deviation on P, step=" << step << "\n";
        _try_step_on_single_coefficient( false, _coeffs.P, step, have_deviation, settings );
    }

    if ( i ) {
        // std::cout << "calculate deviation on I, step=" << step << "\n";
        _try_step_on_single_coefficient( true, _coeffs.I, step, have_deviation, settings );
    };

    if ( d ) {
        // std::cout << "calculate deviation on D, step=" << step << "\n";
        _try_step_on_single_coefficient( true, _coeffs.D, step, have_deviation, settings );
    };

    _make_seria( limit, settings );
    indicators_t indicators = _make_indicators( settings );
    return indicators.sq_deviation;

}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                             Настройка только пропорциональной составляющей регулятора                            *
// *                                                                                                                  *
// ********************************************************************************************************************

int tengu::PID::_tuning_channel_by_deviation ( bool limit, float break_difference, float & haved_deviation, servos_one_channel_settings_t& settings, bool p, bool i, bool d ) {
    float step = 2.0;
    haved_deviation = 1000.0;
    for ( int iter=0; iter < TUNING_MAX_ITERATIONS; iter ++ ) {
        float got_deviation = _calculate_deviation( limit, step, haved_deviation, settings, p, i, d );

        if ( ( got_deviation < haved_deviation ) && ( _coefficient_adopted( limit, haved_deviation, settings, true ) ) ) {
            // Относительная разность в полученных погрешностях.
            float difference = ( haved_deviation - got_deviation );
            if ( difference < 0 ) difference = -difference;

            difference = difference * 100.0 / haved_deviation;
            // std::cout << "+++. Difference=" << difference << ", got_deviation=" << got_deviation << "\n";
            if ( difference <= break_difference ) {
                // std::cout << "=== iteration stoped on " << iter << "\n";
                return iter;
            };

            haved_deviation = got_deviation;

        } else {
            step = step / 2.0;
            if ( step < 1e-12 ) return iter;
        }
    };
    return TUNING_MAX_ITERATIONS;
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                         Вернуть наружу подобранные в результате настройки коэффициенты.                          *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::PID::coefficients_t tengu::PID::coefficients() {
    return _coeffs;
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                               Настройка регулятора.                                              *
// *                                                                                                                  *
// ********************************************************************************************************************

bool tengu::PID::tuning ( servos_one_channel_settings_t & ocs ) {
    _seria_to_null();

    // Все коэффициенты в ноль.
    _coeffs.P = _coeffs.I = _coeffs.D = 0.0;

    // Установки на данный канал.
    // Settings::settings_t stng = settings.get();
    // Settings::one_chanel_settings_t chSettings = stng.ch[ channel ];

    // Шаг изменения коэффициентов.

    // Уже достигнутое среднеквадратическое отклонение. Пока что мы ничего не достигали,
    // так что оно составляет "много".
    float haved_deviation = 1e+10;
    int iters = _tuning_channel_by_deviation( true, 1e-3, haved_deviation, ocs, true, false, false );

    std::cout << "The P coefficient was reached for " << iters << " iterations.\n";
    if ( iters < TUNING_MAX_ITERATIONS ) {

        iters = _tuning_channel_by_deviation( true, 1e-4, haved_deviation, ocs, false, true, false );
        std::cout << "The I coefficient was reached for " << iters << " iterations\n";

        if ( iters < TUNING_MAX_ITERATIONS ) {
            iters = _tuning_channel_by_deviation( true, 1e-5, haved_deviation, ocs, false, false, true );
            std::cout << "The D coefficient was reached for " << iters << " iterations\n";

            // Последняя попытка, хотя вряд ли она улучшит ситуацию.
            if ( iters < TUNING_MAX_ITERATIONS ) {
                // iters = _tuning_channel_by_deviation( true, 1e-6, haved_deviation, chSettings, true, true, true );
                // if ( iters < TUNING_MAX_ITERATIONS ) {
                    // На пока это все, хотя можно было бы поиграться и с временем регулирования.
                    return true;

                    /*
                    std::cout << "End of tuning. P=" << _coeffs.P << ", I=" << _coeffs.I << ", D=" << _coeffs.D << "\n";
                    _make_seria( true, chSettings );

                    std::cout << "From Minimum to Maximum:\n";
                    float time = 0.0;
                    for ( int i=0; i<TUNING_SERIA_SIZE; i++ ) {
                        float velocity = 0;
                        if ( i >= 1 ) {
                            velocity = _seria[i][0] - _seria[i-1][0];
                            velocity *= SERVO_TIMER_FREQUENCY;
                        }
                        std::cout<< "Time=" << time << " s, position=" << _seria[i][0] << ", velocity=" << velocity << " deg/sec\n";
                        time += ( 1.0 / (float) SERVO_TIMER_FREQUENCY );
                    };

                    std::cout << "From Maximum to Minimum\n";
                    time = 0.0;
                    for ( int i=0; i<TUNING_SERIA_SIZE; i++ ) {
                        float velocity = 0;
                        if ( i >= 1 ) {
                            velocity = _seria[i][1] - _seria[i-1][1];
                            velocity *= SERVO_TIMER_FREQUENCY;
                        }
                        std::cout<< "Time=" << time << " s, position=" << _seria[i][1] << ", velocity=" << velocity << " deg/sec\n";
                        time += ( 1.0 / (float) SERVO_TIMER_FREQUENCY );
                    };
                    return true;
                    */

                // };
            };
        };
    };

    return false;
}


// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    Деструктор.                                                   *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::PID::~PID() {

}


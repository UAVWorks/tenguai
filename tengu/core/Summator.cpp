// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                     The summator                                                 *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Сумматор.                                                  *
// *                                                                                                                  *
// * Eugene G. Sysoletin <e.g.sysoletin@gmail.com>                                       Created 27 may 2017 at 06:32 *
// ********************************************************************************************************************

#include "Summator.h"

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                    The constructor.                                              *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                       Конструктор.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Summator::Summator( AbstractAgentKernel * parent, QString name ) 
    : AbstractAgent( parent, name )
{
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Append the input channel.                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Добавить входной канал.                                        *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Summator::addInputChannel( appendage_t channel ) {
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                 Append the output channel                                        *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                   Добавить выходной канал.                                       *
// *                                                                                                                  *
// ********************************************************************************************************************

void tengu::Summator::addOutputChannel(appendage_t channel) {
}

// ********************************************************************************************************************
// *                                                                                                                  *
// *                                                      The destructor.                                             *
// * ---------------------------------------------------------------------------------------------------------------- *
// *                                                        Деструктор.                                               *
// *                                                                                                                  *
// ********************************************************************************************************************

tengu::Summator::~Summator() {
}



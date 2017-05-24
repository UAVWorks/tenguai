########################################################################################################################
#                              The Python implementation of X-Plane redis-based plugin
#               This is some intermediate stage, just to try. The writing on the python is more faster.
# ----------------------------------------------------------------------------------------------------------------------
#                                   Реализация X-Plane/Redis.IO плагина на питоне.
#                Это - некоторая промежуточная стадия, просто чтобы попробовать. На питоне писать быстрее.
#
# Eugene G. Sysloetin <e.g.sysoletin@gmail.com>                                           Created 20 may 2017 at 10:46
########################################################################################################################

import asyncio
import asyncio_redis
from enum import Enum
import time


class AircraftState( Enum ):
    # The state is unknown.
    # Состояние неизвестно.
    NOTHING = 0
    # Stay on the stand.
    # Стоит на стоянке
    STANDS_AT_THE_PARKING = 1
    # Running-up before airborning
    # Разбег (перед отрывом)
    RUN_UP = 2


class Message:

    channel = None
    value = None

    def __init__(self, ch, val ):
        self.channel = ch
        self.value = val

class XPlaneSetter:

    messages = []

    def __init__(self):
        self._connection = None
        self.messages = []

class AbstractAircraft:

    subscribtion = [
        'altitude',
        'latitude',
        'longitude',
        'ground_speed',
        'ias',
        'heading',
        'pitch',
        'roll',
        'agl',
        'pitch_rate',
        'roll_rate',
        'yaw_rate'
    ]

    _redis = None

    def __init__( self, redis ):

        self._latitude = 0.0
        self._longitude = 0.0
        self._altitude = 0.0
        self._ground_speed = 0.0
        self._ias = 0.0
        self._heading = 0.0
        self._pitch = 0.0
        self._roll = 0.0
        self._agl = 0.0
        self._pitch_rate = 0.0
        self._roll_rate = 0.0
        self._yaw_rate = 0.0

        self._redis = redis
        self._previous_control_time = time.time()

        self._control_prefix = None

        # Состояние самолета.
        self._state = AircraftState.NOTHING

    @property
    def latitude(self):
        return self._latitude
    @latitude.setter
    def latitude(self, value ):
        l = float( value )
        self._latitude = l

    @property
    def longitude(self):
        return self._longitude
    @longitude.setter
    def longitude(self, value ):
        l = float( value )
        self._longitude = l

    @property
    def altitude(self):
        return self._altitude
    @altitude.setter
    def altitude(self, value ):
        a = float( value )
        self._altitude = a

    @property
    def ground_speed(self):
        return self._ground_speed
    @ground_speed.setter
    def ground_speed(self, value ):
        gs = float( value )
        self._ground_speed = gs

    @property
    def ias(self):
        return self._ias
    @ias.setter
    def ias(self, value ):
        fs = float(value)
        self._ias = fs

    @property
    def state(self):
        return self._state
    @state.setter
    def state(self, value ):
        self._state = value

    @property
    def heading(self):
        return self._heading
    @heading.setter
    def heading( self, value ):
        fh = float( value )
        self._heading = fh

    @property
    def pitch(self):
        return self._pitch
    @pitch.setter
    def pitch(self, value):
        fp = float(value)
        self._pitch = fp

    @property
    def roll(self):
        return self._roll
    @roll.setter
    def roll(self, value):
        fr = float( value )
        self._roll = fr

    @property
    def agl(self):
        return self._agl
    @agl.setter
    def agl(self, value ):
        fa = float( value )
        self._agl = fa

    @property
    def roll_rate(self):
        return self._roll_rate
    @roll_rate.setter
    def roll_rate(self, value ):
        fr = float( value )
        self._roll_rate = fr

    @property
    def pitch_rate(self):
        return self._pitch_rate
    @pitch_rate.setter
    def pitch_rate(self, val ):
        fp = float( val )
        self._pitch_rate = fp

    @property
    def yaw_rate(self):
        return self._yaw_rate
    @yaw_rate.setter
    def yaw_rate(self, val ):
        fy = float( val )
        self._yaw_rate = fy

    def publish(self, name, value ):
        """
        Публикация значения в редисе.
        :param name:
        :param value:
        :return:
        """
        if self._redis and self._control_prefix:
            channel = self._control_prefix + name
            msg = Message(channel, value )
            self._redis.messages.append(msg)

    def control(self):
        pass


class UserAircraft ( AbstractAircraft ):

    # Переходы состояний. Из какого состояния, в какое состояние и процедура отработки.
    _state_changes = {
        AircraftState.NOTHING : {
            AircraftState.RUN_UP : "_sc_nothing_to_runup"
        }
    }


    def __init__( self, redis ):
        AbstractAircraft.__init__( self, redis )
        self._parking_brake = 0
        self._left_brake = 0
        self._right_brake = 0
        self._control_prefix = "xtengu.control.acf_0."
        self._left_flaperon = 0
        self._right_flaperon = 0
        self._throttle = 0
        self._steering_wheel = 0
        self._left_elevator = 0
        self._right_elevator = 0
        self._left_rudder = 0
        self._right_rudder = 0


    @property
    def parking_brake(self):
        return self._parking_brake
    @parking_brake.setter
    def parking_brake(self, value ):
        ival = int( value )
        self._parking_brake = ival
        self.publish( "parking_brake", ival )

    @property
    def left_brake(self):
        return self._left_brake
    @left_brake.setter
    def left_brake(self, value ):
        iVal = int( value )
        self._left_brake = iVal
        self.publish( "left_brake", iVal )

    @property
    def right_brake(self):
        return self._right_brake
    @right_brake.setter
    def right_brake(self, value ):
        iVal = int( value )
        self._right_brake = iVal
        self.publish("right_brake", iVal )

    @property
    def state(self):
        return self._state
    @state.setter
    def state(self, value ):
        previous_state = self._state
        self._state = value
        self._state_changed( previous_state, value )

    @property
    def throttle(self):
        return self._throttle
    @throttle.setter
    def throttle(self, value):
        iVal = int( value )
        self._throttle = iVal
        self.publish( "throttle", iVal )

    @property
    def left_flaperon(self):
        return self._left_flaperon
    @left_flaperon.setter
    def left_flaperon( self, value ):
        iVal = int( value )
        self._left_flaperon = iVal
        self.publish( "left_flaperon", iVal )

    @property
    def right_flaperon(self):
        return self._right_flaperon
    @right_flaperon.setter
    def right_flaperon(self, value ):
        iVal = int( value )
        self._right_flaperon = iVal
        self.publish( "right_flaperon", iVal )

    @property
    def ias(self):
        return self._ias
    @ias.setter
    def ias( self, value ):
        fv = float(value)
        self._ias = fv

    @property
    def left_rudder(self):
        return self._left_rudder
    @left_rudder.setter
    def left_rudder(self, value):
        r = float( value )
        self._left_rudder = r
        self.publish( "left_rudder", r )

    @property
    def right_rudder(self):
        return self._right_rudder
    @right_rudder.setter
    def right_rudder(self, value ):
        r = float( value )
        self._right_rudder = r
        self.publish( "right_rudder", r )

    @property
    def steering_wheel(self):
        return self._steering_wheel

    @steering_wheel.setter
    def steering_wheel(self, value ):
        fv = float( value )
        if fv < -45.0:
            fv = -45.0
        if fv > 45.0:
            fv = 45.0
        self._steering_wheel = fv
        self.publish( "steering_wheel", fv )

    @property
    def left_elevator(self):
        return self._left_elevator
    @left_elevator.setter
    def left_elevator(self, val ):
        f = float( val )
        self._left_elevator = f
        self.publish( "left_elevator", f )

    @property
    def right_elevator(self):
        return self._right_elevator
    @right_elevator.setter
    def right_elevator(self, val ):
        f = float( val )
        self._right_elevator = f
        self.publish( "right_elevator", f )

    def _state_changed(self, oldState, newState ):
        """
        Выполнить процедуру отработки изменения состояний.
        :return:
        """
        item = self._state_changes[ oldState ]
        if not item:
            print("В переходах нет старого состояния ", oldState )
            return
        proc = item[ newState ]
        if not proc:
            print("В переходах состояний из", oldState, "нет нового состояния", newState )
            return
        if not hasattr( self, proc ):
            print("Переход из состояния", oldState, "в", newState, ", элемент", proc, "отсутствует как атрибут. ")
            return
        func = getattr( self, proc )
        func()

    def _sc_nothing_to_runup(self):
        """
        Переход из состояния "ничего" в состояние "разбег".
        :return:
        """
        self.parking_brake = 0
        self.left_brake = 0
        self.right_brake = 0
        self.throttle = 100
        self.left_flaperon = -10
        self.right_flaperon = -10

        self.left_rudder = -7
        self.right_rudder = -7
        self.steering_wheel = 7

        self.left_elevator = 0
        self.right_elevator = 0

    def control(self):
        AbstractAircraft.control( self )
        ds = -0.01 * self._yaw_rate
        gs = self.ground_speed
        print("First yaw=", str(self._yaw_rate), ",ds=", str(ds) )
        #if gs:
        #    gs = 100 * ( gs * gs )
        #    ds = ds / gs
        #print("ground speed=", str(self._ground_speed), "gs=", str(gs), "ds now=", ds )
        sw = self.steering_wheel
        self.steering_wheel = sw + ds
        rudder_position = self.left_rudder
        self.left_rudder = rudder_position - ds

        print(" new sw=", self.steering_wheel )


class XPlane(XPlaneSetter):

    _acfs = []

    def __init__(self):
        XPlaneSetter.__init__( self )
        self._acfs.append( UserAircraft( self ) )
        self._subscriber = None

        # The frequency of call observing aircraft's motion callback procedure in the X-Plane
        # Частота вызова callback-процедуры наблюдения за движением самолетов в X-Plane
        self._frequency_motion_observing = 2.0

        # The time interval from previous call observing aircraft's motion callback procedure in the X-Plane
        # временнОй интервал с момента предыдущего вызова callback-процедуры наблюдения за движением самолетов.
        self._interval_motion_observing = 0.5

    @asyncio.coroutine
    def run(self):
        # Create connections
        self._pub = yield from asyncio_redis.Connection.create(host="127.0.0.1", port=6379)
        self._connection = yield from asyncio_redis.Connection.create(host='127.0.0.1', port=6379)

        # Create subscriber.
        self._subscriber = yield from self._connection.start_subscribe()

        # Subscribe to channels.
        subscribtion_array = []

        for i in range( 0, len( self._acfs ) ):
            for j in range(0, len( AbstractAircraft.subscribtion ) ):
                channel = 'xtengu.condition.acf_' + str(i) + "." + AbstractAircraft.subscribtion[ j ]
                subscribtion_array.append( channel )

        yield from self._subscriber.subscribe( subscribtion_array )

        self._acfs[0].state = AircraftState.RUN_UP

        # Inside a while loop, wait for incoming events.
        #started = time.time()

        while True:

            # -------------------------------------------------------------------------------------
            #
            #                    Receiving ang handling messages from redis.io
            # -------------------------------------------------------------------------------------
            #                          Прием и обработка сообщений из редиса.
            #
            # -------------------------------------------------------------------------------------

            reply = yield from self._subscriber.next_published()
            # print('Received: ', repr(reply.value), 'on channel', reply.channel)
            # Находим номер канала.
            channel = reply.channel.replace("xtengu.condition.acf_", "" )
            pos = channel.find(".")
            if ( pos > 0 ):
                acf_number = int( channel[0:pos] )
                attr = channel[pos+1:]
                # Смотрим на самолет.
                if acf_number < len( self._acfs ):
                    acf = self._acfs[ acf_number ]
                    if hasattr( acf, attr ):
                        setattr( acf, attr, reply.value )
                    else:
                        print("Attribute not found, acf=", acf_number, ", attr=", attr )

            # -------------------------------------------------------------------------------------
            #
            #       Start of control ( the condistion calculation ) for every each aircraft.
            # -------------------------------------------------------------------------------------
            #           Запуск управления (просчета состояния) для каждого из самолетов.
            #
            # -------------------------------------------------------------------------------------

            for acf in self._acfs:
                acf.control()
                acf._previous_control_time = time.time()

            # -------------------------------------------------------------------------------------
            #
            #                     Transmitting messages to X-Plane (via Redis )
            # -------------------------------------------------------------------------------------
            #                      Передача сообщений в X-Plane (через Редис)
            #
            # -------------------------------------------------------------------------------------

            if self.messages:
                try:
                    while ( self.messages ):
                        msg = self.messages[0]
                        yield from self._pub.publish( msg.channel, str(msg.value) )
                        del self.messages[0]

                except asyncio_redis.Error as e:
                    print('Published failed', repr(e))


            # For the next iteration
            # Для следующей итерации.

            # started = time.time()

        # When finished, close the connection.
        # connection.close()

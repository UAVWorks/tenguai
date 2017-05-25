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
import time, math


class AircraftState( Enum ):

    # The state is unknown.
    # Состояние неизвестно.
    NOTHING = 0

    # Stay on the stand.
    # Стоит на стоянке
    STANDS_AT_THE_PARKING = 1,

    # Debugging one of regulators
    # Отладка одного из регуляторов.
    DEBUG_REGULATOR = 2,

    # Running-up before airborning
    # Разбег (перед отрывом)
    RUN_UP = 3


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

class Regulator:

    def __init__(self, section, redis ):
        self._section = section
        self._publisher = None
        self._last_live = None
        self._redis = redis
        self._desired_value = 0
        self._P = 0
        self._I = 0
        self._D = 0

    @property
    def ping(self):
        return self._last_live

    @ping.setter
    def ping( self, value ):
        ts = int( float(value) )
        timestruct = time.localtime( ts )
        rest = float(value) - ts
        self._last_live = time.mktime( timestruct ) + rest

    @property
    def active(self):
        current_time = time.time();
        delta = current_time - float(self._last_live)
        if delta < 0:
            delta = - delta
        return delta < 1.0
    @active.setter
    def active(self, v):
        value = "0"
        if v:
            value = "1"
        self.publish( "activity", value )

    @property
    def desired_value(self):
        return self._desired_value
    @desired_value.setter
    def desired_value(self, v):
        f = float( v )
        self._desired_value = f
        self.publish( "desired_value" , v )

    @property
    def P(self):
        return self._P
    @P.setter
    def P(self, v ):
        f = float(v)
        self._P = f
        self.publish( "P", v )

    @property
    def I(self):
        return self._I
    @I.setter
    def I(self, v ):
        self._I = float(v)
        self.publish("I", v )

    @property
    def D(self):
        return self._D
    @D.setter
    def D(self, v ):
        self._D = v
        self.publish( "D", v )

    def prefix(self):
        return "tengu.regulators." + self._section.lower() + "."

    def get_subscribtion( self ):
        result = []
        result.append(  self.prefix() + "ping" )
        return result

    def publish(self, channel, value ):
        if ( self._redis ):
            m = Message( self.prefix() + channel, str(value) )
            self._redis.messages.append( m )

    def set_steady_state(self, val ):
        f = float(val)
        self._desired_value = val
        m = Message( self.prefix() + "steady_state", val )
        if self._redis:
            self._redis.messages.append(m)

    def start_debug(self, reactor ):
        """
        Start the debug of this regulator.
        Старт отладки данного регулятора.
        :param reactor:
        :return:
        """
        self.set_steady_state( 0 )
        self._debug_array = []
        self._debug = True
        self._debugger = reactor


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
        self._do_receive = True

        # For the debug purposes
        # Для целей отладки.

        self._debug_array = []
        self._debug_cycle = 0
        self._debug = False

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

        if self._debug:
            self._debug_array.append( gs )
            print("Debug size=", len(self._debug_array ))
            if ( len(self._debug_array) > 200 ):
                self.debug_done()

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

    def relocate(self, latitude, longitude, altitude, heading ):
        """
        Relocate the aircraft.
        Переместить самолет.
        """
        if self._redis and self._control_prefix:
            value = str(latitude) + "," + str(longitude) + "," + str(altitude) + "," + str(heading)
            m = Message(self._control_prefix + "relocate", value)
            self._redis.messages.append( m )

    def control(self):
        pass

    def debug_done(self):
        print("AbstractAicraft::debug_done()")
        pass

    def get_subscribtion( self ):
        return None


class UserAircraft ( AbstractAircraft ):

    # Переходы состояний. Из какого состояния, в какое состояние и процедура отработки.
    _state_changes = {
        AircraftState.NOTHING : {
            AircraftState.RUN_UP : "_sc_nothing_to_runup",
            AircraftState.DEBUG_REGULATOR: "_sc_nothing_to_debug",
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

        self.taxing_speed_regulator = Regulator("TaxingSpeedRegulator", redis )

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

    def nm_to_ms(self, miles):
        """
        transfer from nautical miles to meters per second
        преобразование морских миль в метры в секунду.
        :param miles:
        :return:
        """
        f = float( miles )
        f = f * 0.44704
        return f

    def ms_to_nm(self, miters ):
        """
        Transfer from meter per second to nautical miles.
        Преобразование из метров в секунду в морские мили.
        :param miters:
        :return:
        """
        f = float( miters ) * 1.9438612860586
        return f

    def _sc_nothing_to_runup(self):
        """
        Переход из состояния "ничего" в состояние "разбег".
        :return:
        """

        # Set zero position for control surfaces
        # Установка нулевого положения управляющих поверхностей.

        self.left_elevator = 0
        self.right_elevator = 0

        self.taxing_speed_regulator.set_steady_state( 0 )

        # ground speed goint in meters per second
        # Скорость относительно земли идет - в метрах в секунду.

        self.taxing_speed_regulator.desired_value = self.nm_to_ms(40.0)
        self.taxing_speed_regulator.active = 1

        self.parking_brake = 0
        self.left_brake = 0
        self.right_brake = 0
        #self.throttle = 100

        self.left_flaperon = -10
        self.right_flaperon = -10

        self.taxing_speed_regulator.start_debug( self )

        #self.left_rudder = -7
        #self.right_rudder = -7
        #self.steering_wheel = 7

    def _sc_nothing_to_debug(self):

        print("From NONE to DEBUG.")

        self.parking_brake = 0
        self.left_brake = 0
        self.right_brake = 0
        self.throttle = 0
        self._debug = True

        self.taxing_speed_regulator.desired_value = self.nm_to_ms(40.0)
        self.taxing_speed_regulator.active = 1

        # self.publish("latitude", "56.744726455")
        # self.publish("longitude", "60.826708002")
        # self.publish("altitude", "230.321")
        # self.publish("heading", "254.77089")
        # self.publish("pitch", "20")

    def debug_done(self):

        print("UserAircraft :: debug done. Cycle is ", str( self._debug_cycle ))

        desired = self.taxing_speed_regulator.desired_value
        print("I was desire", str(desired) )

        sum = 0.0
        for val in self._debug_array:
            err = ( val - desired )
            sum += err * err
        sum = math.sqrt(sum)
        print("СКО=", sum)

        # self.publish("latitude", "56.744726455" )
        # self.publish("longitude", "60.826708002" )
        # self.publish("altitude", "230.321")
        # self.publish("heading", "254.77089")

        self.relocate(
            latitude="56.744726455",
            longitude="60.826708002",
            altitude="230.321",
            heading="100"
        )
        #heading = "254.77089"

        self.taxing_speed_regulator.set_steady_state( 0 )
        self.taxing_speed_regulator.active = 0
        self.taxing_speed_regulator.desired_value = 0
        self._debug = False

        self.parking_brake = 100
        self.throttle = 0

        self._debug_array = []
        self._debug_cycle += 1

        # self._sc_nothing_to_debug()

    def control(self):
        AbstractAircraft.control( self )
        delta = -0.014 * self._yaw_rate
        gs = self._ground_speed
        #if gs:
        #    gs = 100 * ( gs * gs )
        #    ds = ds / gs
        #print("ground speed=", str(self._ground_speed), "gs=", str(gs), "ds now=", ds )
        sw = self.steering_wheel
        self.steering_wheel = sw + delta
        rudder_position = self.left_rudder
        self.left_rudder = rudder_position - delta

        #print(" new sw=", self.steering_wheel )

    def get_subscribtion( self ):
        result = []
        acf_array = AbstractAircraft.get_subscribtion( self )
        if acf_array:
            result += acf_array
        result += self.taxing_speed_regulator.get_subscribtion()
        return result


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

        # Subscribe to common aircraft's channels.

        subscribtion_array = []

        for i in range( 0, len( self._acfs ) ):
            for j in range(0, len( AbstractAircraft.subscribtion ) ):
                channel = 'xtengu.condition.acf_' + str(i) + "." + AbstractAircraft.subscribtion[ j ]
                subscribtion_array.append( channel )

        for acf in self._acfs:
            additional_channels = acf.get_subscribtion()
            if additional_channels:
                subscribtion_array += additional_channels

        yield from self._subscriber.subscribe( subscribtion_array )

        # self._acfs[0].state = AircraftState.RUN_UP
        #self._acfs[0].taxing_speed_regulator.P = 1
        #self._acfs[0].taxing_speed_regulator.I = 0
        #self._acfs[0].taxing_speed_regulator.D = 0

        self._acfs[0].state = AircraftState.DEBUG_REGULATOR

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

            found = reply.channel.find( "xtengu.condition.acf_" )
            if found >= 0 :
                # It was a message from some aircraft.
                # Это было сообщение от какого-то самолета.
                channel = reply.channel.replace("xtengu.condition.acf_", "" )
                pos = channel.find(".")

                if ( pos > 0 ):
                    acf_number = int( channel[0:pos] )
                    attr = channel[pos+1:]
                    # Смотрим на самолет.
                    if acf_number < len( self._acfs ):
                        acf = self._acfs[ acf_number ]
                        if acf._do_receive:
                            if hasattr( acf, attr ):
                                setattr( acf, attr, reply.value )
                            else:
                                print("Attribute not found, acf=", acf_number, ", attr=", attr )

            found = reply.channel.find("tengu.regulators.")
            if found >= 0:
                # It was a message from regulator
                # Это было сообщение от регулятора.
                channel = reply.channel.replace("tengu.regulators.", "" )
                sp = channel.split(".")
                user_acf = self._acfs[0]
                prefix = user_acf.taxing_speed_regulator.prefix()
                if  prefix == "tengu.regulators." + sp[0] + ".":
                    setattr( user_acf.taxing_speed_regulator, sp[1], reply.value )

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

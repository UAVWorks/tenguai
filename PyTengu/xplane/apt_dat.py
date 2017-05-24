#!/usr/bin/python3
########################################################################################################################
#                                               Формат apt.dat X-Plane                                                 #
#                                                                                                                      #
# Eugene G. Sysoletin <unclesal@mail.ru>                                                  created 09 feb 2017 at 16:17 #
########################################################################################################################

from mongoengine import *

class Airports(Document):
    """
        Аэропорт.
    """
    uuid = StringField( max_length=48, required= True, unique=True, null=False )
    icao_code = StringField( max_length=32, required=True, unique=True )
    # Имя аэропорта.
    name = StringField(max_length=128)
    # 1=Land Airport, 16=seaplane base, 17=heliport
    apt_type = IntField( default=1, max_value=30, min_value=-1 )
    # Попадание внутрь этой зоны вызывает автоматическое подтягивание
    # аэропорта на клиента.
    ground_zone = MultiPointField( auto_index=True )
    # Elevation of airport in feet above mean sea level (AMSL)
    elevation = DecimalField( precision=2 )
    # Flag for control tower (used only in the X-Plane ATC system)
    # 0=no ATC tower, 1=has ATC tower
    atc_tower = BooleanField( default=False )

    land_runways = []
    pavements = []
    lightings = []
    signs = []
    taxi_locations = []
    # Taxi routing networks
    taxi_networks = []

    def __init__(self, lst ):
        Document.__init__(self)
        # Airport type.
        self.apt_type = lst[0]
        del lst[0]
        # Shifting for first meaning element.
        while not lst[0] and lst :
            del lst[0]
        self.elevation, self.atc_tower, z, self.icao_code = lst[0:4]
        lst = lst[4:]
        self.name = ""
        while lst:
            self.name += lst[0]
            del lst[0]
            if lst:
                self.name += " "

    meta = {
        'collection': 'airports',
        'indexes': [
            'uuid',
            'ground_zone'
        ]
    }


class LandRunways(Document):
    """
    ВПП. Принадлежит аэропорту.
    """
    uuid = StringField( max_length=48, required=True, unique=True, null=False )
    apt = ReferenceField( Airports, null=False, reverse_delete_rule=CASCADE )
    # Ширина ВПП в метрах.
    width = DecimalField( default=20.0, precision=2 )
    # Тип поверхности. 1=асфальт, 2=бетон.
    surface_type = IntField( default=0, max_value=20 )
    # Тип подложки поверхности.
    surface_shoulder_type = IntField( default=0, max_value=10 )
    # 0-плавная, 1.0 - очень грубая
    smoothness = DecimalField( default=0.25, precision=2 )
    # 0 - нет огней, 1 - есть.
    centre_line_lights = IntField( default= 0, max_value=10 )
    # 0=no edge lights, 2=medium intensity edge lights
    edge_lights = IntField( default= 0, max_value=10 )
    # 0=no auto signs, 1=auto-generate signs
    auto_generated_distance_signs = IntField( default=0, max_value=10 )

    land_runways_end = []

    meta = {
        'collection': "land_runways",
        'indexes': [
            'uuid',
            'apt'
        ]
    }

    def __init__(self, lst ):
        Document.__init__(self)
        del lst[0]
        self.width, self.surface_type, self.surface_shoulder_type, \
        self.smoothness, self.centre_line_lights, self.edge_lights,
        self.auto_generated_distance_signs = lst[0:6]
        lst = lst[7:]

        while lst:
            rwe = LandRunwaysEnd( lst )
            rwe.land_runway = self
            self.land_runways_end.append( rwe )
            lst = lst[9:]

class LandRunwaysEnd( Document ):

    uuid = StringField( max_length=48, required=True, unique=True, null=False )
    land_runway = ReferenceField( LandRunways, null=False, reverse_delete_rule=CASCADE)
    # Наименование. Доступны суффиксы L, R, C.
    runway_name = StringField(max_length=5)
    # latitude of runway end (on runway centerline) in decimal degrees)
    latitude = DecimalField(precision=10, max_value=190.0, min_value=-190.0)
    # longitude of runway end (on runway centerline) in decimal degrees
    longitude = DecimalField(precision=10, max_value=190.0, min_value=190.0)
    # Length of displaced threshold in metres(this is included in implied runway
    # length) A displaced threshold will always be inside(between) the two runway ends
    # Two decimal places (metres). Default is 0.00
    displased_length = DecimalField(precision=2, default=0.0)
    # Length of overrun / blast - pad in metres(not included in implied runway
    # length) Two decimal places(metres). Default is 0.00
    overrun_length = DecimalField(precision=2, default=0.0)
    # Code for runway markings(Visual, non - precision, precision) Integer
    # value for Runway Marking Code
    marking = IntField()
    # Code for approach lighting for this runway end
    # Integer value for Approach Lighting Code
    apch_lights = IntField()
    # Flag for runway touchdown zone (TDZ) lighting
    # 0=no TDZ lighting, 1=TDZ lighting
    tdz = IntField(default=0, max_value=10, min_value=-1)
    # Code for Runway End Identifier Lights (REIL)
    # 0 = no REIL,
    # 1 = omni - directional REIL,
    # 2 = unidirectional REIL
    reil = IntField(db_field=0, max_value=10, min_value=-1)

    meta = {
        'collection' : 'land_runways_ends'
    }

    def __init__(self, lst ):
        Document.__init__(self)
        self.runway_name, self.latitude, self.longitude, self.displaced_length, \
        self.overrun_length, self.marking, self.apch_lights, self.tdz, self.reil = lst[0:9]


class Pavement(Document):
    uuid = StringField( max_length=48, required=True, unique=True, null=False )
    apt = ReferenceField( Airports, nul=False, reverse_delete_rule=CASCADE )
    # Code defining the surface type(concrete, asphalt, etc)
    # Integer value for a Surface Type Code
    surface_type = IntField(default=0, max_value=20, min_value=0)
    # Runway smoothness (not used by X-Plane yet)
    smoothness = DecimalField( default=0.25, precision=2 )
    orientation = DecimalField( default=0.0, precision=2 )
    description = StringField( max_length=32, default="" )

    nodes = []

    meta = {
        'collection': 'pavements',
        'indexes': [
            'uuid',
            'apt'
        ]
    }

    def __init__( self, lst ):
        Document.__init__( self )
        self.surface_type, self.smoothness, self.orientation = lst[1:4]
        lst = lst[4:]
        self.description = ""
        while lst:
            self.description += lst[0]
            del lst[0]
            if lst:
                self.description += " "

class Node( Document ):

    uuid = StringField( max_length=48, required=True, unique=True, null=False )
    # 111 Node(plain)
    # 112 Node with Bezier control point
    # 113 Node(close loop), to close boundary
    # 114 Node (close loop) with Bezier control point
    # 115 Node(end) to terminate a line
    # 116 Node(end) with Bezier control point
    type = IntField( default=0, max_value=500 )

    latitude = DecimalField(precision=10, max_value=190.0, min_value=-190.0, default=0.0 )
    longitude = DecimalField(precision=10, max_value=190.0, min_value=190.0, default=0.0 )

    latitude_bezier_cp = DecimalField(precision=10, max_value=190.0, min_value=-190.0, default=0.0)
    longitude_bezier_cp = DecimalField(precision=10, max_value=190.0, min_value=-190.0, default=0.0)

    lighting_code = IntField( default=0, min_value=-10, max_value=500 )
    line_code = IntField( default=0, min_value=-10, max_value=300 )

    pavement = None

    meta = {
        'collection' : 'nodes',
        'indexes' : [
            'uuid'
        ]
    }

    def __init__(self, lst ):
        Document.__init__( self )
        self.type = lst[0]
        self.latitude, self.longitude = lst[1:3]
        lst = lst[3:]
        if int( self.type ) in [ 112, 114, 116 ]:
            self.latitude_bezier_cp, self.longitude_bezier_cp = lst[0:2]
            lst = lst[3:]

        if lst:
            if len(lst) == 1:
                self.lighting_code = lst[0]
            elif len(lst) == 2:
                self.line_code, self.lighting_code = lst[0:2]

class Lighting( Document ):

    uuid = StringField(max_length=48, required=True, unique=True, null=False)
    apt = ReferenceField(Airports, nul=False, reverse_delete_rule=CASCADE)

    latitude = DecimalField(precision=10, max_value=190.0, min_value=-190.0, default=0.0)
    longitude = DecimalField(precision=10, max_value=190.0, min_value=190.0, default=0.0)

    lighting_object_code = IntField( default=0, min_value=-1, max_value=200 )
    orientation = DecimalField( precision=3, min_value=-190.0, max_value=190.0, default=0.0 )
    # Visual glideslope angle in degrees
    glidescope_angle = DecimalField( precision=3, min_value=-1, max_value=190, default=0.0 )
    # Associated runway number(required for VASI / PAPI, etc)
    associated_rwy = StringField( max_length=16 )
    # Description of lighting object(not used by X - Plane
    description = StringField( max_length=32 )

    meta = {
        'collection' : 'lightings',
        'indexes' : [
            'uuid',
            'apt'
        ]
    }

    def __init__(self, lst ):
        Document.__init__(self)
        self.latitude, self.longitude, self.lighting_object_code, self.orientation, \
        self.glidescope_angle, self.associated_rwy = lst[1:7]
        lst = lst[7:]
        self.description = ""
        while lst:
            self.description += lst[0]
            del lst[0]
            if ( lst ):
                self.description += " "

class Sign(Document):

    uuid = StringField(max_length=48, required=True, unique=True, null=False)
    apt = ReferenceField(Airports, nul=False, reverse_delete_rule=CASCADE)

    latitude = DecimalField(precision=10, max_value=190.0, min_value=-190.0, default=0.0)
    longitude = DecimalField(precision=10, max_value=190.0, min_value=190.0, default=0.0)

    # Orientation of sign in true degrees(heading of someone looking at sign’s front)
    orientation = DecimalField(precision=3, min_value=-190.0, max_value=190.0, default=0.0)
    # Code for sign size
    size_code = IntField( max_value=20, default=0 )
    # Text to be rendered on sign front and/or back
    text = StringField( max_length=32, default="" )

    meta = {
        'collection' : 'signs',
        'indexes' : [
            'uuid', 'apt'
        ]
    }

    def __init__( self, lst ):
        Document.__init__( self )
        self.latitude, self.longitude, self.orientation, reserved1, self.size_code = lst[1:6]
        lst = lst[6:]
        self.text = ""
        while lst:
            self.text += lst[0]
            del lst[0]
            if lst:
                self.text += " "


class TaxiLocation( Document ):

    uuid = StringField(max_length=48, required=True, unique=True, null=False)
    apt = ReferenceField(Airports, nul=False, reverse_delete_rule=CASCADE)

    latitude = DecimalField(precision=10, max_value=190.0, min_value=-190.0, default=0.0)
    longitude = DecimalField(precision=10, max_value=190.0, min_value=190.0, default=0.0)

    # Heading (true) of airplane positioned at this location
    heading = DecimalField(precision=3, min_value=-190.0, max_value=190.0, default=0.0)
    # Type of location “gate”, “hangar”, “misc” or “tie-down”
    location_type = StringField(max_length=16 )
    # Airplane types to that can use this location
    # Pipe - separated list(“ | ”).Can include “heavy”, ”jets”,
    # “turboprops”, “props” and “helos” ( or just “all” for all types)
    airplane_type = StringField( max_length=16 )

    name = StringField( max_length=32 )


    def __init__(self, lst ):
        Document.__init__(self)
        self.latitude, self.longitude, self.heading, self.location_type, self.airplane_type = lst[1:6]
        lst = lst[6:]
        self.name = ""
        while lst:
            self.name += lst[0]
            del lst[0]
            if lst :
                self.name += " "

class TaxiRoutingNetwork( Document ):
    uuid = StringField(max_length=48, required=True, unique=True, null=False)
    apt = ReferenceField(Airports, nul=False, reverse_delete_rule=CASCADE)

    nodes = []
    edges = []

    meta = {
        'collection' : 'taxi_networks',
        'indexes' : [
            'uuid', 'apt'
        ]
    }

    def __init__(self, lst ):
        Document.__init__( self )

class TaxiRoutingNode( Document ):

    uuid = StringField(max_length=48, required=True, unique=True, null=False)
    network = ReferenceField( TaxiRoutingNetwork, nul=False, reverse_delete_rule=CASCADE )

    latitude = DecimalField(precision=10, max_value=190.0, min_value=-190.0, default=0.0)
    longitude = DecimalField(precision=10, max_value=190.0, min_value=190.0, default=0.0)

    # Usage of node in network(begin or end a taxi path, or both)
    both = StringField(max_length=16)
    # Node identifier (user-defined)
    node_identifier = IntField( default=0 )
    # Node name.Not currently used.
    name = StringField( max_length=64, default="" )

    meta = {
        'collection' : 'taxi_routing_nodes',
        'indexes' : [
            'uuid', 'network'
        ]
    }

    def __init__(self, lst ):
        Document.__init__(self)
        self.latitude, self.longitude, self.both, self.node_identifier = lst[1:5]
        lst = lst[5:]
        self.name = ""
        while lst:
            self.name += lst[0]
            del lst[0]
            if lst:
                self.name += " "


class TaxiRoutingEdge( Document ):

    uuid = StringField(max_length=48, required=True, unique=True, null=False)
    network = ReferenceField(TaxiRoutingNetwork, nul=False, reverse_delete_rule=CASCADE)

    start_node_identifier = IntField( default=0 )
    end_node_identifier = IntField( default=0 )

    # Edge can be used in both directions
    # “twoway” or “oneway”

    twoway = StringField( max_length=16, default="" )

    # Node is on a regular taxiway.If on “runway” a clearance is needed from ATC
    # “taxiway” or “runway”

    taxiway = StringField( max_length=16, default="" )

    # Taxiway identifier.Used to build ATC taxi clearances(eg. “... taxi via A, T, Q”)
    # String. Taxiway or runway identifier (eg. “A” or “16L/34R”)
    name = StringField( max_length=32, default="" )

    active_zones = []

    meta = {
        'collection' : 'taxi_routing_edges',
        'indexes' : [ 'uuid', 'network' ]
    }

    def __init__(self, lst ):
        Document.__init__(self)
        self.start_node_identifier, self.end_node_identifier, self.twoway, self.taxiway = lst[1:5 ]
        lst = lst[5:]
        self.name = ""
        while lst:
            self.name += lst[0]
            del lst[0]
            if lst:
                self.name += " "

class TaxiRoutingEdgeActiveZone( Document ):


    uuid = StringField(max_length=48, required=True, unique=True, null=False)
    edge = ReferenceField( TaxiRoutingEdge, nul=False, reverse_delete_rule=CASCADE)

    # Active zone classification
    # “arrival” or “departure” or “ils”

    classification = StringField( max_length=16, default="" )

    # Runway(s) to which active zone refers
    # Comma-separated list up to 4 runway identifies
    rwys = StringField( max_length=128, default="" )

    meta = {
        'collection' : 'taxi_routing_enge_active_zones',
        'indexes' : [ 'uuid', 'edge' ]
    }


class XP10_Importer:
    """
    Импортер из формата apt.dat X-Plane
    """
    # Ключ - это код X-Plane.
    # 1 элемент в списке - это класс, который соответствует данному ключу.
    # Последнее - функция, которую вызываем после добавления элемента.

    ocodes = {
        'I' : [ None ],
        '1000' : [ None ],
        '1' : [ Airports, "ac_airport" ],
        # Viewpoint
        '14' : [ None ],
        '16' : [ Airports, "ac_airport" ],
        '17' : [ Airports, "ac_airport" ],
        '20' : [ Sign, "ac_sign"],
        '21' : [ Lighting, "ac_lighting"],
        '100' : [ LandRunways, "ac_land_runway" ],
        '110' : [ Pavement, "ac_pavement" ],
        '111' : [ Node, "ac_node" ],
        '112' : [ Node, "ac_node" ],
        '113' : [ Node, "ac_node" ],
        '114' : [ Node, "ac_node" ],
        '115' : [ Node, "ac_node" ],
        '116' : [ Node, "ac_node" ],
        # Linear feature
        '120' : [ None ],
        # Airport boundary
        #'130' : [ None ]
        '1200': [ TaxiRoutingNetwork, "ac_taxi_routing_network" ],
        '1201' : [ TaxiRoutingNode, "ac_taxi_routing_node" ],
        '1202' : [ TaxiRoutingEdge, "ac_taxi_routing_edge" ],
        '1204' : [ TaxiRoutingEdgeActiveZone, "ac_taxi_routing_edge_active_zone" ],
        '1300' : [ TaxiLocation, "ac_taxi_location" ]

    }

    def __init__(self, fname):
        """
        Конструктор импортера.
        :param fname:
        """
        file = open( fname, "r" )
        if not file:
            print("XP_Importer: can't open file", fname )
        self.content = file.readlines()
        file.close()
        self.airport = None

    def do_import(self):

        if self.airport:
            print("XP10_Importer: do_import, already have an airport.")
            return

        for line in self.content:
            d = line.split(' ')
            d = [ x.replace("\n", "") for x in d if x != "" ]
            if not d or len(d) < 2 :
                continue

            # Get collection element for class and element of this airport's item.

            oocode_elem = XP10_Importer.ocodes.get( d[0] )
            if not oocode_elem:
                print("Element not found:", d[0] )
                continue

            # Get the class for this airport's item.

            oclass = oocode_elem[ 0 ]

            if not oclass:
                #print("Object Code is not realized:", d[0] )
                pass
            else:
                # Create object for this class
                oobject = oclass( d )
                # Call after-create function for this object.
                if len(oocode_elem) > 1 :
                    fname = oocode_elem[1]
                    #if hasattr( self, fname ):
                    fun = getattr( self, fname )
                    fun( oobject )
                    #else:
                    #    print("Attribute not found ", fname )


        # end lines loop, do_import procedure


    def ac_airport(self, apt ):
        """
        After-creating airport function
        :return:
        """
        self.airport = apt

    def ac_land_runway(self, rwy ):
        """
        after-create land runway function
        :param rwy:
        :return:
        """
        rwy.apt = self.airport
        self.airport.land_runways.append( rwy )

    def ac_pavement(self, pvm ):
        """
        after-create pavement function
        :param pvm:
        :return:
        """
        pvm.apt = self.airport
        self.airport.pavements.append( pvm )

    def ac_node(self, node ):
        last_pavement = self.airport.pavements[ len(self.airport.pavements) - 1 ]
        node.pavement = last_pavement
        last_pavement.nodes.append( node )

    def ac_lighting(self, light ):
        light.apt = self.airport
        self.airport.lightings.append( light )

    def ac_sign(self, sign ):
        sign.apt = self.airport
        self.airport.signs.append( sign )

    def ac_taxi_location(self, taxi_location ):
        taxi_location.apt = self.airport
        self.airport.taxi_locations.append( taxi_location )

    def ac_taxi_routing_network(self, network ):
        network.apt = self.airport
        self.airport.taxi_networks.append( network )

    def ac_taxi_routing_node(self, node ):
        network = self.airport.taxi_networks[ len( self.airport.taxi_networks ) - 1 ]
        node.network = network
        network.nodes.append( network )

    def ac_taxi_routing_edge(self, edge ):
        network = self.airport.taxi_networks[len(self.airport.taxi_networks) - 1]
        edge.network = network
        network.edges.append( edge )

    def ac_taxi_routing_edge_active_zone(self, zone ):
        network = self.airport.taxi_networks[len(self.airport.taxi_networks) - 1]
        edge = network.edges[ len( network.edges ) - 1 ]
        zone.edge = edge
        edge.active_zones.append( edge )


# connect("tengu")
#
# fname = "/opt/X-Plane 10/Custom Scenery/USSS_Koltsovo_2012/Earth nav data/apt.dat"
# importer = XP10_Importer( fname )
# importer.do_import()
#
# f = open(fname, "r")
# for line in f:
#     ls = line.split(" ")
#     print(ls)
# f.close()
# content = [x.strip() for x in content]

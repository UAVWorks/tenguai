TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = common        \
    loredis             \
    xtengu              \
    onboard         
    
onboard.depends = common
xtengu.depends = loredis



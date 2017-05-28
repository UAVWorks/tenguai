TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = loredis           \
        core                \
        onboard             \
        guis
    
core.depends = loredis
onboard.depends = core
guis.depends = core



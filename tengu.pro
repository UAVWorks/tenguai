TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = tengu         \
    simulators          
    
simulators.depends = tengu



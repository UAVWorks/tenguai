TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = loredis         \
        core              \
	onboard
    
core.depends = loredis



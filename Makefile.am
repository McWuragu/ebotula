SHELL=/bin/sh
SUBDIRS=src tools etc doc po

if BUILD_TESTSUIT
SUBDIRS += testsuit
endif

DIST_SUBDIRS = src tools etc doc po testsuit

EXTRA_DIST= config.rpath ebotula.spec.in README.de BUGS
dist-hook: ebotula.spec
	cp ebotula.spec $(distdir)

ACLOCAL_AMFLAGS = -I m4

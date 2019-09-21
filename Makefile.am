SHELL=/bin/sh
SUBDIRS=src tools etc doc po intl testsuit

EXTRA_DIST= config.rpath ebotula.spec.in README.de BUGS
dist-hook: ebotula.spec
	cp ebotula.spec $(distdir)

ACLOCAL_AMFLAGS = -I m4

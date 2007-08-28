SHELL=/bin/sh
SUBDIRS=src etc doc po

EXTRA_DIST= m4/ChangeLog ebotula.spec.in README.de BUGS
dist-hook: ebotula.spec
	cp ebotula.spec $(distdir)

ACLOCAL_AMFLAGS = -I m4

levdir = $(datadir)/jetpack/levels

AM_CPPFLAGS = \
	-DBLIT \
	-DSCOREPATH='"$(sharedstatedir)/jetpack.scores"' \
	-DLEVELPATH='"$(levdir)/"'

AM_CFLAGS = $(X_CFLAGS)

bin_PROGRAMS = jetpack

noinst_HEADERS = \
	bitmap.h \
	copyright.h \
	defs.h \
	message.h

jetpack_SOURCES = \
	bitmap.c \
	bonus.c \
	collision.c \
	demo.c \
	draw.c \
	erase.c \
	events.c \
	gameover.c \
	initx.c \
	main.c \
	maze.c \
	message.c \
	normal.c \
	quitx.c \
	scores.c \
	setinmaze.c \
	setup.c \
	special.c \
	time.c \
	update.c \
	windowx.c

jetpack_LDFLAGS = $(X_LIBS)

jetpack_LDADD = $(X_PRE_LIBS) -lX11 $(X_EXTRA_LIBS)

dist_lev_DATA = \
	levels/000

dist_man1_MANS = \
	jetpack.man

dist_doc_DATA = README

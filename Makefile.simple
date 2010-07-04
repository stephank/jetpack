# Program name
EXEC = jetpack

# Master destination directory
DESTDIR = /usrd/s/m/meb2

# Various subdirectories
LIBDIR = $(DESTDIR)/lib/jetpack
BINDIR = $(DESTDIR)/bin
MANPATH = $(DESTDIR)/man

# Files for high scores and predifined levels
SCOREFILE = $(LIBDIR)/jetpack.scores
LEVELFILE = $(LIBDIR)/jetpack.lev

# -O4 for optimization, -DBLIT for blitting
CFLAGS = -O4 -DBLIT -DSCOREPATH=\"$(SCOREFILE)\" -DLEVELPATH=\"$(LEVELFILE)\"

# Your favorite X library
LIBS = -lX11

OBJS =	bitmap.o bonus.o collision.o demo.o draw.o erase.o events.o\
		gameover.o initx.o main.o maze.o message.o normal.o quitx.o\
		scores.o setinmaze.o setup.o special.o time.o update.o windowx.o

jetpack: $(OBJS)
	cc -o $(EXEC) $(OBJS) $(LIBS) $(CFLAGS)

# do whatever you like for installation here. I have it set all the data
# files to be readable and writable only by the user, (except the man
# page) and the program itself to be setuid
install: $(EXEC)
	strip $(EXEC)
	cp $(EXEC) $(BINDIR)
	chmod 4755 $(BINDIR)/$(EXEC)
	cp levels/000 $(LEVELFILE)000
	chmod 0600 $(LEVELFILE)000
	cp jetpack.man $(MANDIR)/jetpack.n
	chmod 0644 $(MANDIR)/jetpack.n

# dependencies (leave these alone)
$(OBJS) : defs.h copyright.h
bitmap.o : bitmap.h
message.c : message.h

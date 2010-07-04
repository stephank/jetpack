/*	scores.c : routines that implement and maintain the high scores.
*/

#include "copyright.h"
#include "defs.h"

/*	If the scorefile is not externally defined at compile time, the
	default scorefile is jetpack.scores . However, this means scorefiles
	will be created in the user's cwd whenever he/she runs the game.
*/
#ifndef SCOREPATH
#define SCOREPATH "jetpack.scores"
#endif

Pixmap	scorepixmap;

#define	MAXHIGHLENGTH	46
#define MAXNAMELENGTH	21

int		numhighs;
int		highscore[MAXHIGHSCORES];
int		highlevel[MAXHIGHSCORES];
char	highstring[MAXHIGHSCORES][MAXHIGHLENGTH];
char	dtext[MAXNAMELENGTH], dialogtime[10];
int		scorefd;
FILE	*scorefile;
Window	dialogwindow,dialogstringwindow,dialogbuttonwindow;
int		dialogwidth, dialogheight;

/*	open_scores tries to open the score file, and if it can't it tries to
	create one.
*/
open_scores()
{
	numhighs = 0;
	scorefd = open(SCOREPATH, O_RDWR);
	if(scorefd == -1) {
		scorefd = open(SCOREPATH, O_RDWR | O_CREAT, 0444);
		if(scorefd == -1) {
			scorefd = open("jetpack.scores", O_RDWR);
			if(scorefd == -1) {
				scorefd = open("jetpack.scores", O_RDWR | O_CREAT, 0444);
				if(scorefd == -1) {
					fprintf(stderr,"Jetpack : Couldn't open scorefile.\n");
					exit(1);
				} else {
					flock(scorefd,LOCK_EX);
					scorefile = fdopen(scorefd,"r+");
					write_scores();
					flock(scorefd,LOCK_UN);
					return;
				}
			} else {
				flock(scorefd,LOCK_EX);
				scorefile = fdopen(scorefd,"r+");
				read_scores();
				flock(scorefd,LOCK_UN);
				return;
			}
		} else {
			flock(scorefd,LOCK_EX);
			scorefile = fdopen(scorefd,"r+");
			write_scores();
			flock(scorefd,LOCK_UN);
			return;
		}
	} else {
		flock(scorefd,LOCK_EX);
		scorefile = fdopen(scorefd,"r+");
		read_scores();
		flock(scorefd,LOCK_UN);
		return;
	}
}

/*	close_scores closes the score file.
*/
close_scores()
{
	fclose(scorefile);
	close(scorefd);
}

/*	read_scores reads the high score data from the score file. 
*/
read_scores()
{
	register int	i;

	rewind(scorefile);
	fflush(scorefile);
	fread(&numhighs, sizeof(int), 1, scorefile);
	for(i=0; i<numhighs; i++) {
		fread(highstring[i], sizeof(char), MAXHIGHLENGTH - 2, scorefile);
		fread(highlevel+i, sizeof(int), 1, scorefile);
		fread(highscore+i, sizeof(int), 1, scorefile);
		highstring[i][MAXHIGHLENGTH-1] = '\0';
	}
	fflush(scorefile);
}

/*	write_scores writes the high score data into the score file.
*/
write_scores()
{
	register int	i;

	rewind(scorefile);
	fflush(scorefile);
	fwrite(&numhighs, sizeof(int), 1, scorefile);
	for(i=0; i<numhighs; i++) {
		fwrite(highstring[i], sizeof(char), MAXHIGHLENGTH - 2, scorefile);
		fwrite(highlevel+i, sizeof(int), 1, scorefile);
		fwrite(highscore+i, sizeof(int), 1, scorefile);
	}
	fflush(scorefile);
}

/*	read_new_scores re-reads in the score file info just in case it has
	been changed.
*/
read_new_scores()
{
	flock(scorefd,LOCK_EX);
	read_scores();
	flock(scorefd,LOCK_UN);
	XFillRectangle(display, scorepixmap, ctable[CBLACK].smallgc,
					0, 0, SCOREWIDTH, SCOREHEIGHT);
	draw_scores(-1);
}

/*	check_score checks the score global variable against the high score
	table, and if it makes it, gets the player's name and saves the new
	high scores in the file.
*/
check_score()
{
	register int	i, spot;
	char			buf[MAXNAMELENGTH];

	spot = 0;
	flock(scorefd,LOCK_EX);
	read_scores();
	flock(scorefd,LOCK_UN);
	for(i=0; i<numhighs; i++) {
		if(score > highscore[i]) {
			get_high_name(buf);
			spot = 1;
			break;
		}
	}
	if(spot == 0) {
		if(numhighs < MAXHIGHSCORES) get_high_name(buf);
		else return;
	}
	flock(scorefd,LOCK_EX);
	read_scores();
	for(i=0; i<numhighs; i++) {
		if(score > highscore[i]) {
			shift_highs(i);
			highscore[i] = score;
			highlevel[i] = level;
			sprintf(highstring[i], "%-20s     %-3d       %-9d", buf,
					level, score);
			write_scores();
			flock(scorefd,LOCK_UN);
			return;
		}
	}
	/*	Didn't beat anyone else, but if the scoreboard isn't full, let it
		in.
	*/
	if(numhighs < MAXHIGHSCORES) {
		highscore[numhighs] = score;
		highlevel[numhighs] = level;
		sprintf(highstring[numhighs], "%-20s     %-3d       %-9d", buf,
				level, score);
		numhighs++;
		write_scores();
		flock(scorefd,LOCK_UN);
		return;
	}
	/* Sorry, you got bumped off while you entered your name */
}

#define	HIGHTITLE	"Mad Mark's Top Fliers:"
#define	HIGHTOP		"Name:                    Level:    Score:   "

/*	draw_scores draws the high score table on the scorepixmap. which
	allows only 5 of the names to be drawn at a time, which keeps things
	from lagging.
*/
draw_scores(which)
int	which;
{
	register int	i, j, length, width, y, first, last;

	length = strlen(HIGHTITLE);
	width = XTextWidth(bigfontinfo,HIGHTITLE,length);
	XDrawImageString(display, scorepixmap,
						ctable[random()%(NCOLORS-1)+1].biggc,
						SCOREWIDTH / 2 - width / 2, 3 + bigfontinfo->ascent,
						HIGHTITLE, length);
	y = bigfontinfo->ascent + bigfontinfo->descent + 3;
	length = strlen(HIGHTOP);
	width = XTextWidth(fontinfo,HIGHTOP,length);
	XDrawString(display, scorepixmap, ctable[CWHITE].smallgc,
				SCOREWIDTH / 2 - width / 2, y + 3 + fontinfo->ascent,
				HIGHTOP, length);
	y += fontinfo->ascent * 2 + fontinfo->descent + 4;
	if(which == -1) {
		first = 0;
		last = MAXHIGHSCORES;
	} else {
		first = which * 5;
		last = which * 5 + 5;
	}
	i = y + first * (fontinfo->ascent + fontinfo->descent);
	for(j=first; ((j<numhighs) && (j<last)); j++) {
		length = strlen(highstring[j]);
		width = XTextWidth(fontinfo,highstring[j],length);
		XDrawImageString(display, scorepixmap,
							ctable[random()%(NCOLORS-1)+1].smallgc,
							SCOREWIDTH / 2 - width / 2, i, highstring[j],
							length);
		i += fontinfo->ascent + fontinfo->descent;
	}
}

/*	shift_highs pushes all the scores from place i down one spot.
*/
shift_highs(i)
int	i;
{
	int	j;

	for(j=MAXHIGHSCORES-1; j>i; j--) {
		highscore[j] = highscore[j-1];
		highlevel[j] = highlevel[j-1];
		strncpy(highstring[j],highstring[j-1],44);
	}
	if(numhighs < MAXHIGHSCORES) numhighs++;
}

/*	get_high_name brings up a dialog that prompts the player to enter
	his/her name. The name is stored in buf.
*/
get_high_name(buf)
char	*buf;
{
	XEvent					event;
	register int			j, finished, timer;
	/*
	register unsigned long	utimer, usecond;
	*/

	XMapRaised(display, dialogwindow);
	XMapRaised(display, dialogstringwindow);
	XMapRaised(display, dialogbuttonwindow);
	XFlush(display);

	dtext[0] = '\0';
	finished = 0;
	while(!finished) {
			XNextEvent(display, &event);
			switch(event.type) {
				case ConfigureNotify:
				case Expose:
				case MapNotify:
					display_dialog(event.xany.window);
					break;
				case ButtonPress:
					if(event.xbutton.window == dialogbuttonwindow) {
						XFillRectangle(display, dialogbuttonwindow,
										ctable[CBLUE].smallgc, 0, 0,
										50, 30);
						display_dialog(dialogbuttonwindow);
						finished = 1;
					}
					break;
				case KeyPress:
					finished = dialog_key(&event);
					break;
			}
	}

	/*	check to see if there are non-blank characters in the name. If
		there aren't any, the user is shy, so return "Anonymous"
	*/
	for(j=0, finished=0; dtext[j] != '\0'; j++) {
		if(dtext[j] != ' ') finished = 1;
	}
	if(!finished) strncpy(dtext,"Anonymous",20);

	strncpy(buf, dtext, 20);

	/*	unmap the dialog
	*/
	XUnmapWindow(display, dialogstringwindow);
	XUnmapWindow(display, dialogbuttonwindow);
	XUnmapWindow(display, dialogwindow);
	XFlush(display);
}

/*	dialog_key processes keypresses from the dialog
*/
int	dialog_key(event)
XKeyEvent	*event;
{
	int				length, l;
	char			buf[MAXNAMELENGTH];
	KeySym			keysym;
	XComposeStatus	composestatus;

	length = XLookupString(event, buf, MAXNAMELENGTH, &keysym,
							&composestatus);
	buf[1] = '\0';
	l = strlen(dtext);
	/*	check for printable character
	*/
	if((keysym >= ' ') && (keysym <= '~') && (length > 0)) {
		if((l + strlen(buf)) < MAXNAMELENGTH) {
			strcat(dtext, buf);
			display_dialog(dialogstringwindow);
		}
	} else {
		switch(keysym) {
			/*	delete key
			*/
			case XK_BackSpace:
			case XK_Delete:
				if(l >= 1) {
					XClearWindow(display, dialogstringwindow);
					l--;
					dtext[l] = '\0';
					display_dialog(dialogstringwindow);
					XFlush(display);
				}
				break;

			/* return is as good as hitting ok
			*/
			case '\n':
			case XK_Return:
			case XK_KP_Enter:
				XFillRectangle(display, dialogbuttonwindow,
								ctable[CBLUE].smallgc, 0, 0, 50, 30);
				display_dialog(dialogbuttonwindow);
				return(1);
			default:
				break;
		}
	}
	return(0);
}

/*	display_dialog draws the part of the dialog specified by w. Also
	refreshes anything that needs to be.
*/
display_dialog(w)
Window	w;
{
	if(w == dialogwindow) {
		XDrawString(display, dialogwindow, ctable[CGREEN].biggc,
					10, 5 + bigfontinfo->ascent, "You made a high score!",
					strlen("You made a high score!"));
		XDrawString(display, dialogwindow, ctable[CBLUE].biggc,
					10, 10 + bigfontinfo->descent + 2 *
					bigfontinfo->ascent, "Enter your name:",
					strlen("Enter your name:"));
	}
	if(w == dialogstringwindow)
		XDrawString(display, dialogstringwindow,
					ctable[CRED].biggc, 10,
					5 + bigfontinfo->ascent, dtext, strlen(dtext));
	if(w == dialogbuttonwindow)
		XDrawString(display, dialogbuttonwindow,
					ctable[CWHITE].biggc, 10,
					5 + bigfontinfo->ascent, "OK", strlen("OK"));
	if(w == gamewindow) 

#ifdef	BLIT
		XCopyArea(display, drawpixmap, gamewindow, ctable[CBLACK].smallgc,
					0, 0, WINDOWWIDTH, WINDOWHEIGHT, 0, 0);
#else
		refresh();
#endif

	if(w == infowindow) refresh_info();
	XFlush(display);
}

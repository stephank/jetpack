/*	draw.c : routines that draw objects in the window
*/

#include "copyright.h"
#include "defs.h"

Pixmap	drawpixmap;

char	fuelstring[20] = "Fuel: ";
char	scorestring[30] = "Score: ";
char	levelstring[40] = "Mazes Completed: ";
char	bonusstring[30] = "Bonus: ";

int	oldscore = -1, oldlevel = -1;

int	titley = 130;

/*	draw draws all the objects in the window.
*/
void	draw()
{
#ifdef	BLIT
	XCopyArea(display, drawpixmap, gamewindow, ctable[CBLACK].smallgc,
				0, 0, WINDOWWIDTH, WINDOWHEIGHT, 0, 0);
    XFillRectangle(display, drawpixmap, ctable[CBLACK].smallgc, 0, 0,
					WINDOWWIDTH, WINDOWHEIGHT);
#else
	erase_draw_zones();
	init_zones();
#endif

	draw_info();

	draw_player();
	draw_maze();
	draw_door();
	draw_sweepers();
	draw_treasures();
	draw_fireballs();
	draw_guards();
	draw_messages();
}

/*	draw_maze draws the lines of the maze that appear in the window
*/
void	draw_maze()
{
	register int	i, x1, y1, x2, y2;

	for(i=0; i<mazelcount; i++) {
		x1 = mazelines[i].x1 - plx + WINDOWWIDTH / 2;
		y1 = mazelines[i].y1 - ply + WINDOWHEIGHT / 2;
		x2 = mazelines[i].x2 - plx + WINDOWWIDTH / 2;
		y2 = mazelines[i].y2 - ply + WINDOWHEIGHT / 2;
		if(((x1 >= -EDGE) && (y1 >= -EDGE) && (x1 <= WINDOWWIDTH + EDGE) &&
			(y1 <= WINDOWHEIGHT + EDGE)) || ((x2 >= -EDGE) && (y2 >= -EDGE)
			&& (x2 <= WINDOWWIDTH + EDGE) && (y2 <= WINDOWHEIGHT + EDGE))) {

#ifdef	BLIT
				XDrawLine(display, drawpixmap, ctable[CWHITE].smallgc,
							x1, y1, x2, y2);
#else
				set_zone(x1, y1, x2, y2, ZLINE, i);
#endif
		}
	}
}

/*	draw_player draws the player in the center of the window
*/
void	draw_player()
{
	if(dead) return;
	if(exploded > -1) {

#ifdef	BLIT
		XCopyArea(display, explodepix[exploded], drawpixmap,
					ctable[CWHITE].smallgc, 0, 0, EXPLODEWIDTH,
					EXPLODEHEIGHT, WINDOWWIDTH / 2 - EXPLODEWIDTH / 2,
					WINDOWHEIGHT / 2 - EXPLODEHEIGHT / 2);
#else
		set_zone(WINDOWWIDTH / 2 - EXPLODEWIDTH / 2, WINDOWHEIGHT / 2 -
					EXPLODEHEIGHT / 2, EXPLODEWIDTH, EXPLODEHEIGHT,
					ZEXPLODE, 0);
#endif

	} else {

#ifdef	BLIT
		XCopyArea(display, playerpix[playerphase], drawpixmap,
					ctable[CWHITE].smallgc, 0, 0, PLAYERWIDTH,
					PLAYERHEIGHT, WINDOWWIDTH / 2 - PLAYERWIDTH / 2,
					WINDOWHEIGHT / 2 - PLAYERHEIGHT / 2);
#else
		set_zone(WINDOWWIDTH / 2 - PLAYERWIDTH / 2, WINDOWHEIGHT / 2 -
					PLAYERHEIGHT / 2, PLAYERWIDTH, PLAYERHEIGHT,
					ZPLAYER, 0);
#endif

		if(burn)

#ifdef	BLIT
			XCopyArea(display, burnpix[playerphase / FACEFRONT],
							drawpixmap, ctable[CWHITE].smallgc, 0, 0,
							PLAYERWIDTH, PLAYERHEIGHT, WINDOWWIDTH / 2 -
							PLAYERWIDTH / 2, WINDOWHEIGHT / 2 -
							PLAYERHEIGHT / 2);
#else
		set_zone(WINDOWWIDTH / 2 - PLAYERWIDTH / 2, WINDOWHEIGHT / 2 -
					PLAYERHEIGHT / 2, PLAYERWIDTH, PLAYERHEIGHT,
					ZBURN, 0);
#endif

	}
}

/*	draw_fireballs draws all the fireballs in the window
*/
void	draw_fireballs()
{
	register int	i, fx, fy;

	for(i=0; i<numfire; i++) {
		fx = transx(firex[i], FIREWIDTH);
		fy = transy(firey[i], FIREHEIGHT);
		if((fx >= -FIREWIDTH - EDGE) && (fy >= -FIREHEIGHT - EDGE) &&
			(fx <= WINDOWWIDTH + EDGE) && (fy <= WINDOWHEIGHT + EDGE)) {

#ifdef	BLIT
			XCopyArea(display, firepix[firephase[i]], drawpixmap,
						ctable[CWHITE].smallgc, 0, 0, FIREWIDTH,
						FIREHEIGHT, fx, fy);
#else
			set_zone(fx, fy, FIREWIDTH, FIREHEIGHT, ZFIRE, i);
#endif

		}
	}
}

/*	draw_guards draws all the guards in the window
*/
void	draw_guards()
{
	register int	i, gx, gy;

	for(i=0; i<numguard; i++) {
		gx = transx(guardx[i], GUARDWIDTH);
		gy = transy(guardy[i], GUARDHEIGHT);
		if((gx >= -GUARDWIDTH - EDGE) && (gy >= -GUARDHEIGHT - EDGE) &&
			(gx <= WINDOWWIDTH + EDGE) && (gy <= WINDOWHEIGHT + EDGE)) {

#ifdef	BLIT
			XCopyArea(display, guardpix[guarddir[i]][guardphase[i]],
						drawpixmap, ctable[CWHITE].smallgc, 0, 0,
						GUARDWIDTH, GUARDHEIGHT, gx, gy);
#else
			set_zone(gx, gy, GUARDWIDTH, GUARDHEIGHT, ZGUARD, i);
#endif

		}
	}
}

/*	draw_sweepers draws all the sweepers in the window
*/
void	draw_sweepers()
{
	register int	i, sx, sy;

	for(i=0; i<numsweep; i++) {
		sx = transx(sweepx[i], SWEEPERWIDTH);
		sy = transy(sweepy[i], SWEEPERHEIGHT);
		if((sx >= -SWEEPERWIDTH - EDGE) && (sy >= -SWEEPERHEIGHT - EDGE) &&
			(sx <= WINDOWWIDTH + EDGE) && (sy <= WINDOWHEIGHT + EDGE)) {

#ifdef	BLIT
			XCopyArea(display, sweeperpix[sweepphase[i]], drawpixmap,
						ctable[CWHITE].smallgc, 0, 0, SWEEPERWIDTH,
						SWEEPERHEIGHT, sx, sy);
#else
			set_zone(sx, sy, SWEEPERWIDTH, SWEEPERHEIGHT, ZSWEEP, i);
#endif

		}
	}
}

/*	draw_treasures draws the key and fuel pods in the window
*/
void	draw_treasures()
{
	register int	i, tx, ty, width;

	for(i=0; i<numfuel; i++) {
		if(fuelalive[i]) {
			tx = transx(fuelx[i], FUELWIDTH);
			ty = transy(fuely[i], FUELHEIGHT);
			if((tx >= -FUELWIDTH - EDGE) && (ty >= -FUELHEIGHT - EDGE) &&
				(tx <= WINDOWWIDTH + EDGE) && (ty <= WINDOWHEIGHT + EDGE)) {

#ifdef	BLIT
				XCopyArea(display, fuelpix, drawpixmap,
							ctable[CWHITE].smallgc, 0, 0, FUELWIDTH,
							FUELHEIGHT, tx, ty);
#else
				set_zone(tx, ty, FUELWIDTH, FUELHEIGHT, ZFUEL, i);
#endif

			}
		} else if(fueltimer[i]) {
			tx = transx(fuelx[i], FUELWIDTH);
			ty = transy(fuely[i], FUELHEIGHT);
			width = XTextWidth(fontinfo,"100",3) / 2;

#ifdef	BLIT
			XDrawString(display, drawpixmap,
						ctable[CGREEN].smallgc, tx + FUELWIDTH / 2
						- width, ty + FUELHEIGHT / 2,
						"100", 3);
#else
			set_zone(tx + FUELWIDTH / 2 - width, ty + FUELHEIGHT / 2 -
						fontinfo->ascent, width * 2, fontinfo->ascent +
						fontinfo->descent, ZFUEL, i);
#endif

			fueltimer[i]--;
		}
	}
	if(keyalive) {
		tx = transx(keyx, KEYWIDTH);
		ty = transy(keyy, KEYHEIGHT);
		if((tx >= -KEYWIDTH - EDGE) && (ty >= -KEYHEIGHT - EDGE) &&
			(tx <= WINDOWWIDTH + EDGE) && (ty <= WINDOWHEIGHT + EDGE)) {

#ifdef	BLIT
			XCopyArea(display, keypix, drawpixmap,
						ctable[CWHITE].smallgc, 0, 0, KEYWIDTH,
						KEYHEIGHT, tx, ty);
#else
			set_zone(tx, ty, KEYWIDTH, KEYHEIGHT, ZKEY, 0);
#endif

		}
	} else if(keytimer) {
		tx = transx(keyx, KEYWIDTH);
		ty = transy(keyy, KEYHEIGHT);
		width = XTextWidth(fontinfo,"500",3) / 2;

#ifdef	BLIT
		XDrawString(display, drawpixmap, ctable[CGREEN].smallgc,
					tx + KEYWIDTH / 2 - width, ty + KEYHEIGHT / 2,
					"500", 3);
#else
		set_zone(tx + KEYWIDTH / 2 - width, ty + KEYHEIGHT / 2 -
					fontinfo->ascent, width * 2, fontinfo->ascent +
					fontinfo->descent, ZKEY, 0);
#endif

		keytimer--;
	}
}

/*	draw_door draws the door in the window
*/
void	draw_door()
{
	register int	dx, dy, phase;

	dx = transx(doorx, DOORWIDTH);
	dy = transy(doory, DOORHEIGHT);
	if((dx >= -DOORWIDTH - EDGE) && (dy >= -DOORHEIGHT - EDGE) &&
		(dx <= WINDOWWIDTH + EDGE) && (dy <= WINDOWHEIGHT + EDGE)) {
		phase = indoor;
		if(indoor && (exploded > -1)) phase = 0;

#ifdef	BLIT
		XCopyArea(display, doorpix[phase], drawpixmap,
					ctable[CWHITE].smallgc, 0, 0, DOORWIDTH,
					DOORHEIGHT, dx, dy - (DOORHEIGHT - PLAYERHEIGHT) / 2);
#else
		set_zone(dx, dy - (DOORHEIGHT - PLAYERHEIGHT) / 2, DOORWIDTH,
					DOORHEIGHT, ZDOOR, 0);
#endif

	}
}

/*	transx translates an x coordinate against the player's x coordinate to
	fit it properly in the window.
*/
int	transx(int x, int off)
{
	return(x - plx + WINDOWWIDTH / 2 - off / 2);
}

/*	transy translates an y coordinate against the player's y coordinate to
	fit it properly in the window.
*/
int	transy(int y, int off)
{
	return(y - ply + WINDOWHEIGHT / 2 - off / 2);
}

/*	refresh_info redraws the info window
*/
void	refresh_info()
{
	register int	i;

	XClearWindow(display, infowindow);
	if(!keyalive) XCopyArea(display, keypix, infowindow,
							ctable[CWHITE].smallgc, 0, 0, KEYWIDTH,
							KEYHEIGHT, WINDOWWIDTH / 2 - 10 - KEYWIDTH, 5);
	for(i=0; i<men; i++) {
		XCopyArea(display, extramanpix, infowindow, ctable[CWHITE].smallgc,
					0, 0, EXTRAMANWIDTH, EXTRAMANHEIGHT, WINDOWWIDTH - (i +
					1) * (EXTRAMANWIDTH + 3), 5);
	}
	if(gameover) XDrawString(display,infowindow, ctable[CRED].biggc,
								100,30,"DEMO",strlen("DEMO"));
	oldlevel = -1;
	oldscore = -1;
	draw_info();
}

/*	draw_messages draws any active messages in the window
*/
void	draw_messages()
{
	register int	tmpcolor;

	if(messagetime != 0) {

#ifdef	BLIT
		if(messagecolor == CBLACK)
			tmpcolor = random() % (NCOLORS-1) + 1;
		else tmpcolor = messagecolor;
		XDrawString(display, drawpixmap,
					ctable[tmpcolor].smallgc, messagex, messagey,
					messagestring, messagelength);
#else
		set_zone(messagex, messagey - fontinfo->ascent, messagewidth,
					fontinfo->ascent + fontinfo->descent, ZSMESS, 0);
#endif

		messagetime--;
	}
	if(bigmessagetime != 0) {

#ifdef	BLIT
		if(bigmessagecolor == CBLACK)
			tmpcolor = random() % (NCOLORS-1) + 1;
		else tmpcolor = bigmessagecolor;
		XDrawString(display, drawpixmap,
					ctable[tmpcolor].biggc,
					bigmessagex, bigmessagey,
					bigmessagestring, bigmessagelength);
#else
		set_zone(bigmessagex, bigmessagey - bigfontinfo->ascent,
					bigmessagewidth, bigfontinfo->ascent +
					bigfontinfo->descent, ZBMESS, 0);
#endif

		bigmessagetime--;
	}
}

/*	draw_info draws the stuff in the info window that needs to be updated
	frequently
*/
void	draw_info()
{
	int	tmp;

	sprintf(fuelstring, "Fuel: %04d", playerfuel);
	if(playerfuel < 100) tmp = CRED;
	else if(playerfuel < 500) tmp = CYELLOW;
	else tmp = CGREEN;
	XDrawImageString(display, infowindow, ctable[tmp].smallgc,
						10, 10, fuelstring, strlen(fuelstring));
	if(level != oldlevel) {
		sprintf(levelstring, "Level: %03d", level);
		XDrawImageString(display, infowindow, ctable[CWHITE].smallgc,
							10, 20 + fontinfo->ascent + fontinfo->descent,
							levelstring, strlen(levelstring));
		oldlevel = level;
	}
	sprintf(bonusstring, "Bonus: %06d", bonus);
	XDrawImageString(display, infowindow, ctable[CWHITE].smallgc,
						WINDOWWIDTH / 2, 10, bonusstring,
						strlen(bonusstring));
	if(score != oldscore) {
		sprintf(scorestring, "Score: %09d", score);
		XDrawImageString(display, infowindow, ctable[CWHITE].smallgc,
							WINDOWWIDTH / 2, 20 + fontinfo->ascent +
							fontinfo->descent, scorestring,
							strlen(scorestring));
		oldscore = score;
	}
}

/*  place_string draws a centered string on the title screen
*/
void	place_string(char *s, int y, int color, XFontStruct *info)
{
    register int    length, width;

    length = strlen(s);
    width = XTextWidth(info, s, length);

#ifdef  BLIT
    if(info == bigfontinfo) {
        XDrawString(display, drawpixmap, ctable[color].biggc,
                    WINDOWWIDTH / 2 - width / 2, y, s, length);
    } else {
        XDrawString(display, drawpixmap, ctable[color].smallgc,
                    WINDOWWIDTH / 2 - width / 2, y, s, length);
    }
#else
    if(info == bigfontinfo) {
        XDrawString(display, gamewindow, ctable[color].biggc,
                    WINDOWWIDTH / 2 - width / 2, y, s, length);
    } else {
        XDrawString(display, gamewindow, ctable[color].smallgc,
                    WINDOWWIDTH / 2 - width / 2, y, s, length);
    }
#endif

}

/*  title_draw updates and draws the title screen
*/
void	title_draw()
{
    register int    i, j;

    update_fireballs();
    update_sweepers();
    update_guards();

#ifdef  BLIT
    XFillRectangle(display, drawpixmap, ctable[CBLACK].smallgc, 0,
                    titley, WINDOWWIDTH, WINDOWHEIGHT - titley);
    XCopyArea(display, scorepixmap, drawpixmap, ctable[CWHITE].smallgc,
                0, 0, SCOREWIDTH, SCOREHEIGHT, SCOREX, SCOREY);
#else
    XCopyArea(display, scorepixmap, gamewindow, ctable[CBLACK].smallgc,
                0, 0, SCOREWIDTH, SCOREHEIGHT, SCOREX, SCOREY);
    erase_draw_zones();
    init_zones();
#endif

    draw_maze();
    draw_guards();
    draw_sweepers();
    draw_fireballs();
    /*  make the player image do random things
    */
    if(random() % 30 == 0) burn = 1 - burn;
    if(random() % 30 == 0) playerdir = random() % 3 - 1;
    if(!burn && (playerdir != STOP)) {
        playerphase += playerdir;
        if(playerphase > FACERIGHT - 1) playerphase = FACEFRONT + 1;
        if(playerphase < FACELEFT + 1) playerphase = FACEFRONT - 1;
        if((playerdir == GOLEFT) && (playerphase < FACEFRONT))
			playerphase = FACEFRONT;
        if((playerdir == GORIGHT) && (playerphase > FACEFRONT))
			playerphase = FACEFRONT;
    } else playerphase = (playerdir+1) * FACEFRONT;

#ifdef  BLIT
    XCopyArea(display, playerpix[playerphase], drawpixmap,
                ctable[CWHITE].smallgc, 0, 0, PLAYERWIDTH,
                PLAYERHEIGHT, WINDOWWIDTH / 2 - PLAYERWIDTH / 2, titley);
    if(burn) XCopyArea(display, burnpix[playerphase / FACEFRONT],
                        drawpixmap, ctable[CWHITE].smallgc, 0, 0,
                        PLAYERWIDTH, PLAYERHEIGHT, WINDOWWIDTH / 2 -
                        PLAYERWIDTH / 2, titley);
#else
    XFillRectangle(display, gamewindow, ctable[CBLACK].smallgc,
                    WINDOWWIDTH / 2 - PLAYERWIDTH / 2, titley, PLAYERWIDTH,
                    PLAYERHEIGHT);
    XCopyArea(display, playerpix[playerphase], gamewindow,
                ctable[CWHITE].smallgc, 0, 0, PLAYERWIDTH,
                PLAYERHEIGHT, WINDOWWIDTH / 2 - PLAYERWIDTH / 2, titley);
    if(burn) XCopyArea(display, burnpix[playerphase / FACEFRONT],
                        gamewindow, ctable[CWHITE].smallgc, 0, 0,
                        PLAYERWIDTH, PLAYERHEIGHT, WINDOWWIDTH / 2 -
                        PLAYERWIDTH / 2, titley);
#endif
}

/*  title_refresh draws the title screen stuff that doesn't have to be
    continuously updated
*/
void	title_refresh()
{
    char            scstring[30];
    register int    y, width;

    y = 5 + bigfontinfo->ascent;
    place_string("Jetpack", y, CYELLOW, bigfontinfo);
    y += bigfontinfo->descent + 5 + fontinfo->ascent;
    place_string("By Mark Bradley", y, CBLUE, fontinfo);
    y += fontinfo->descent + 5 + fontinfo->ascent;
    place_string("Controls:", y, CWHITE, fontinfo);
    y += fontinfo->descent + fontinfo->ascent;
    place_string(
        "  keypad 4, a, left button : move left                       ",
        y, CGREEN, fontinfo);
    place_string(
        "                                           K : commit suicide",
        y, CRED, fontinfo);
    y += fontinfo->descent + fontinfo->ascent;
    place_string(
        " keypad 6, d, right button : move right                      ",
        y, CGREEN, fontinfo);
    place_string(
        "                                           p : pause game    ",
        y, CBLUE, fontinfo);
    y += fontinfo->descent + fontinfo->ascent;
    place_string(
        "               keypad 5, s : stop                            ",
        y, CRED, fontinfo);
    place_string(
        "                                           Q : quit game     ",
        y, CGREY, fontinfo);
    y += fontinfo->descent + fontinfo->ascent;
    place_string(
        "             center button : fire jetpack                    ",
        y, CYELLOW, fontinfo);
    y += fontinfo->descent + 10 + fontinfo->ascent;
    place_string("Extra Player awarded every 10000 points", y, CGREY,
                    fontinfo);

#ifdef  BLIT
    XCopyArea(display, fuelpix, drawpixmap, ctable[CWHITE].smallgc,
                0, 0, FUELWIDTH, FUELHEIGHT, WINDOWWIDTH / 4 - FUELWIDTH /
               2, 20 - FUELHEIGHT / 2);
    XCopyArea(display, keypix, drawpixmap, ctable[CWHITE].smallgc,
                0, 0, KEYWIDTH, KEYHEIGHT, 3 * (WINDOWWIDTH / 4) -
                KEYWIDTH / 2, 20 - KEYHEIGHT / 2);
    width = XTextWidth(fontinfo, "100", 3);
    XDrawString(display, drawpixmap, ctable[CGREEN].smallgc,
                WINDOWWIDTH / 4 - width / 2, 20 + FUELHEIGHT / 2 + 5 +
                fontinfo->ascent, "100", 3);
    width = XTextWidth(fontinfo, "500", 3);
    XDrawString(display, drawpixmap, ctable[CGREEN].smallgc,
                3 * (WINDOWWIDTH / 4) - width / 2, 20 + KEYHEIGHT / 2 +
                10 + fontinfo->ascent, "500", 3);
#else
    XCopyArea(display, fuelpix, gamewindow, ctable[CWHITE].smallgc,
                0, 0, FUELWIDTH, FUELHEIGHT, WINDOWWIDTH / 4 - FUELWIDTH /
               2, 20 - FUELHEIGHT / 2);
    XCopyArea(display, keypix, gamewindow, ctable[CWHITE].smallgc,
                0, 0, KEYWIDTH, KEYHEIGHT, 3 * (WINDOWWIDTH / 4) -
                KEYWIDTH / 2, 20 - KEYHEIGHT / 2);
    width = XTextWidth(fontinfo, "100", 3);
    XDrawString(display, gamewindow, ctable[CGREEN].smallgc,
                WINDOWWIDTH / 4 - width / 2, 20 + FUELHEIGHT / 2 + 5 +
                fontinfo->ascent, "100", 3);
    width = XTextWidth(fontinfo, "500", 3);
    XDrawString(display, gamewindow, ctable[CGREEN].smallgc,
                3 * (WINDOWWIDTH / 4) - width / 2, 20 + KEYHEIGHT / 2 +
                10 + fontinfo->ascent, "500", 3);
#endif

    sprintf(scstring, "Last Score: %09d", lastscore);
    XDrawString(display, infowindow, ctable[CGREY].biggc,
                WINDOWWIDTH / 2 -
                XTextWidth(bigfontinfo, scstring, strlen(scstring)) / 2,
                INFOHEIGHT / 2, scstring, strlen(scstring));
}

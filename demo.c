/*	demo.c : routines that implement the title screen and demonstration.
*/

#include "copyright.h"
#include "defs.h"

#define	DEMODELAY	50

int	demostage, demodelay;

/*	demo is the pseudo-event loop that runs the demo. It is identical to
	the main loop, except demo_event_filter only checks for input to leave
	the demo, and demo_player does the actual player action. The level
	itself is special level 0.
*/
demo()
{
	register unsigned long	timer;

	reset_game();
	reset_level();
	level = 0;
	demostage = 0;
	if(special_setup() == 0) return;
	XClearWindow(display, gamewindow);
	XClearWindow(display, infowindow);
	draw();
	refresh_info();
	while(!leveldone) {
            begin_timer();
            while(XPending(display) && !demo_event_filter());
			demo_player();
			update();
            collision_check();
            message_check();
			bonus_check();
            draw();
            timer = get_timer();
            if(timer < USECDELAY) usleep(USECDELAY - timer);
	}
	XClearWindow(display, gamewindow);
	XClearWindow(display, infowindow);
}

/*	title_screen is the animated title screen. There is a "maze" that
	surrounds the high score list that contain moving enemies. There is
	also a player suspended in the center going through random motions.
*/
title_screen()
{
	register unsigned long	timer, btimer;
	register int			shifttimer, shiftwhich;

	setup_title_maze();

#ifdef	BLIT
	XFillRectangle(display, drawpixmap, ctable[CBLACK].smallgc, 0, 0,
					WINDOWWIDTH, WINDOWHEIGHT);
#else
	init_zones();
#endif

	XClearWindow(display, gamewindow);
	XClearWindow(display, infowindow);
	title_refresh();
	/*	read in the scorefile in case it has changed
		and draw all the scores for the first time
	*/
	read_new_scores();
	shifttimer = 5; shiftwhich = 0;
	begin_timer();
	while(gameover) {
		btimer = get_timer();
		while(XPending(display)) title_event_filter();

#ifdef	BLIT
		XCopyArea(display, drawpixmap, gamewindow,
			ctable[CBLACK].smallgc, 0, 0, WINDOWWIDTH, WINDOWHEIGHT,
			0, 0);
#endif

		/* 	Only draw 5 scores at a time so there won't be lag
		*/
		shifttimer--;
		if(shifttimer == 0) {
			shifttimer = 5;
			draw_scores(shiftwhich);
			shiftwhich++;
			if(shiftwhich == MAXHIGHSCORES / 5) shiftwhich = 0;
		}
		title_draw();
		timer = get_timer();
		/*	after 15 seconds, switch to demo
		*/
		if(timer > 15000000) break;
		if(timer-btimer < USECDELAY) usleep(USECDELAY-(timer-btimer));
	}
	stop_timer();
	XClearWindow(display, gamewindow);
	XClearWindow(display, infowindow);
}

/*	demo_walk is a procedure that makes the demo player walk to a
	position.
*/
demo_walk(x)
int	x;
{
	if(plx > x + 5) playerdir = GOLEFT;
	else if(plx < x - 5) playerdir = GORIGHT;
	else {
		playerdir = STOP;
		demostage++;
	}
}

/*	demo_fly is a procedure that makes the demo player fly to a position.
*/
demo_fly(x, y)
int	x, y;
{
	int	maybe = 0;

	if(plx > x + 5) playerdir = GOLEFT;
	else if(plx < x - 5) playerdir = GORIGHT;
	else {
		playerdir = STOP;
		maybe = 1;
	}
	if(ply > y + 5) burn = 1;
	else if(ply < y - 5) burn = 0;
	else {
		burn = 0;
		if(maybe) demostage++;
	}
}

/*	demo_delay makes the demo player wait for a while.
*/
demo_delay()
{
	demodelay--;
	if(!demodelay) demostage++;
}

/*	demo_player uses the above macro procedures in a crude switch
	statement to make the demo player run through the whole routine.
	A much better implementation would be to have all the data in a static
	array, but since it's so simple, I'll stick to this for now.
*/
demo_player()
{
	switch(demostage) {
		case 0:
			demo_walk(300);
			demodelay = DEMODELAY;
			break;
		case 1:
			demo_delay();
			break;
		case 2:
			demo_fly(240, 260);
			break;
		case 3:
			demo_fly(400, 350);
			demodelay = DEMODELAY;
			break;
		case 4:
			demo_delay();
			break;
		case 5:
			demo_walk(660);
			demodelay = DEMODELAY;
			break;
		case 6:
			demo_delay();
			break;
		case 7:
			demo_walk(1040);
			demodelay = DEMODELAY;
			break;
		case 8:
			demo_delay();
			break;
		case 9:
			demo_fly(1380, 300);
			break;
		case 10:
			demo_fly(1380, 660);
			break;
		case 11:
			demo_walk(1140);
			demodelay = DEMODELAY;
			break;
		case 12:
			demo_delay();
			break;
		case 13:
			demo_walk(900);
			demodelay = DEMODELAY;
			break;
		case 14:
			demo_delay();
			break;
		case 15:
			demo_fly(keyx, keyy);
			break;
		case 16:
			demo_walk(900);
			break;
		default:
			leveldone = 1;
			break;
	}
}

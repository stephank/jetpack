/*	events.c : routines that do event handling
*/

#include "copyright.h"
#include "defs.h"

/*	event_filter handles game events
*/
int	event_filter()
{
	XEvent	event;

	XNextEvent(display, &event);
	switch(event.type) {
		case ConfigureNotify:
		case Expose:
		case MapNotify:
			refresh_info();

#ifdef	BLIT
			XCopyArea(display, drawpixmap, gamewindow,
						ctable[CBLACK].smallgc, 0, 0,
						WINDOWWIDTH, WINDOWHEIGHT, 0, 0);
#else
			refresh();
#endif

			return(1);
			break;
		case ButtonPress:
			if(!waiting) {
				if(event.xbutton.button == Button2) burn = 1;
				if(event.xbutton.button == Button1) {
					if(event.xbutton.state & Button3Mask) {
						playerdir = STOP;
					} else {
						playerdir = GOLEFT;
					}
				}
				if(event.xbutton.button == Button3) {
					if(event.xbutton.state & Button1Mask) {
						playerdir = STOP;
					} else {
						playerdir = GORIGHT;
					}
				}
			}
			return(1);
			break;
		case ButtonRelease:
			if((event.xbutton.state & Button2Mask) && 
				(event.xbutton.button == Button2))
				burn = 0;
			if(event.xbutton.button == Button3) {
				if(event.xbutton.state & Button1Mask) {
					playerdir = GOLEFT;
				} else {
					playerdir = STOP;
				}
			}
			if(event.xbutton.button == Button1) {
				if(event.xbutton.state & Button3Mask) {
					playerdir = GORIGHT;
				} else {
					playerdir = STOP;
				}
			}
			return(1);
			break;
		case KeyPress:
			key_hit(&event);
			return(1);
			break;
	}
	return(0);
}

/*	key_hit handles game key events
*/
key_hit(event)
XKeyEvent	*event;
{
	char			buf;
	KeySym			key;
	XComposeStatus	i;
	
	XLookupString(event, &buf, 1, &key, &i);
	switch(key) {
		case XK_KP_4:
		case 'a':
		case 'A':
			playerdir = GOLEFT;
			break;
		case XK_KP_6:
		case 'd':
		case 'D':
			playerdir = GORIGHT;
			break;
		case XK_KP_5:
		case ' ':
		case 's':
		case 'S':
			playerdir = STOP;
			break;
		case 'K':
			if(exploded != -1) break;
			dumb_message(MSUICIDE);
			exploded = 0;
			explodetimer = EXPLODETIME;
			indoor = 1;
			if(!keyalive) {
				keyalive = 1;
				keyx = plx;
				keyy = ply;
			}
			break;
		case 'Q':
			if(dead) break;
			dead = 1;
			deadtimer = 50;
			messagetime = 0;
			bigmessagetime = 0;
			bigmessage("Game Over", CBLACK, 100);
			dumb_message(MQUIT);
			break;
		case 'p':
			if(paused) {
				paused = 0;
				bigmessagetime = 0;
				begin_timer();
			} else {
				bigmessage("PAUSED", CBLUE, 50);
				draw();

#ifdef	BLIT
				XCopyArea(display, drawpixmap, gamewindow,
							ctable[CBLACK].smallgc, 0, 0,
							WINDOWWIDTH, WINDOWHEIGHT, 0, 0);
#else
				refresh();
#endif

				paused = 1;
				stop_timer();
			}
			break;
	}
}

/*	demo_event_filter handles events for the demo
*/
int	demo_event_filter()
{
	XEvent	event;

	XNextEvent(display, &event);
	switch(event.type) {
		case ConfigureNotify:
		case Expose:
		case MapNotify:
			refresh_info();
			return(1);
			break;
		case ButtonPress:
			leveldone = 1;
			gameover = 0;
			break;
		case KeyPress:
			title_key_hit(&event);
			return(1);
			break;
	}
	return(0);
}

/*	title_event_filter handles events for the title screen
*/
title_event_filter()
{
	XEvent	event;

	XNextEvent(display, &event);
	switch(event.type) {
		case ConfigureNotify:
		case Expose:
		case MapNotify:
			title_refresh();
			break;
		case ButtonPress:
			gameover = 0;
			break;
		case KeyPress:
			title_key_hit(&event);
			break;
	}
}

/*	title_key_hit handles key events for the demo and title screen
*/
title_key_hit(event)
XKeyEvent	*event;
{
	char			buf;
	KeySym			key;
	XComposeStatus	i;
	int				length;
	
	length = XLookupString(event, &buf, 1, &key, &i);
	switch(key) {
		case 'Q':
		case 'q':
			quit_game();
			break;
		default:
			if(length > 0) {
				gameover = 0;
				leveldone = 1;
			}
	}
}

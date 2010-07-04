/*	main.c : main loop of jetpack
*/

#include "copyright.h"
#include "defs.h"

Window	gamewindow, infowindow;

int	men, score, level;

int	leveldone, paused, gameover, lastscore;

/*	main is the main loop of the progam. (surprise!) Because of the
	functional abstraction, it should be pretty readable.
*/
main()
{
	int						seed;
	char					lev[20];
	register unsigned long	timer;

	/*	randomize the random number generator by seeding it with the time
		in seconds
	*/
	seed = (int) time(NULL);
	srandom(seed);

	
	/*	set things up
	*/
	open_scores();
	init_X();
	set_up_windows();
	set_up_bitmaps();
	lastscore = 0;
	gameover = 1;

#ifndef	BLIT
	init_zones();
#endif

	/*	This outer loop is executed forever. The program is exited by a
		call to quit_game.
	*/
	while(1) {

		/*	This loop cycles between the title screen and the
			demonstration. When the user hits a key or button, gameover
			is set to 1.
		*/
		while(gameover) {
			title_screen();
			if(gameover) demo();
		}

		reset_game();

		/*	This loop is the outer "game" loop. Each level is started here.
		*/
		while(!gameover) {
			reset_level();

			/*	Some levels are special preset levels
			*/
			if(!special_setup()) normal_level();

			/* Display the level number
			*/
			sprintf(lev, "Level: %d", level);
			bigmessage(lev, CGREEN, 75);

			/* initial draw
			*/
			XClearWindow(display, gamewindow);
			draw();
			XClearWindow(display, infowindow);
			refresh_info();

			/*	central event loop. When the level is over, or the player
				loses his/her last life, leveldone is set to 1.
			*/
			while(!leveldone) {
				begin_timer();
				while(paused) {
					while(XPending(display) && !event_filter());
					usleep(USECDELAY);
				}
				while(XPending(display) && !event_filter());
				update();
				collision_check();
				message_check();
				bonus_check();
				draw();

				/*	When nothing was in the maze except me, I set
					USECDELAY to a nice-looking speed. Jetpack doesn't
					use a lot of cpu, but there would be slowdown with
					more objects. So to keep the delay constant, I used
					begin_timer and get_timer to time the calculations
					and drawing, subtracted that from USECDELAY, and
					voila! I had a constant delay. The only times that
					it runs over are usually unnoticeable, and if it
					runs over a lot, it means the machine is really
					busy, and you shouldn't be playing anyway.
				*/
				timer = get_timer();
				if(timer < USECDELAY) usleep(USECDELAY - timer);
			}
		}
	}
}

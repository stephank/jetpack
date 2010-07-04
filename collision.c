/*	collision.c : routines that check player collisions
*/

#include "copyright.h"
#include "defs.h"

/*	collision_check checks for all collisions
*/
collision_check()
{
	register int	i;
	register long	dx, dy;

	/*	if in the door, player can't be hurt
	*/
	if(!indoor) {
		for(i=0; i<numfire; i++) {
			dx = (plx - firex[i]) * (plx - firex[i]);
			dy = (ply - firey[i]) * (ply - firey[i]);
			if(dx + dy <= (PLAYERWIDTH / 2 + FIREWIDTH / 2) * (PLAYERWIDTH
				/ 2 + FIREWIDTH / 2)) {
				if(exploded == -1) {
					dumb_message(MKILL);
					exploded = 0;
					explodetimer = EXPLODETIME;
					indoor = 1;
					if(!keyalive) {
						keyalive = 1;
						keyx = plx;
						keyy = ply;
						refresh_info();
					}
				}
			} else if(dx + dy <= (PLAYERWIDTH / 2 + FIREWIDTH / 2) *
						(PLAYERWIDTH / 2 + FIREWIDTH / 2) * 2) {
				if((plx / mazescale == firex[i] / mazescale) &&
					(ply / mazescale == firey[i] / mazescale))
						dumb_message(MCLOSE);
			}
		}
		for(i=0; i<numsweep; i++) {
			dx = (plx - sweepx[i]) * (plx - sweepx[i]);
			dy = (ply - sweepy[i]) * (ply - sweepy[i]);
			if(dx + dy <= (PLAYERWIDTH / 2 + SWEEPERWIDTH / 2) * (PLAYERWIDTH
				/ 2 + SWEEPERWIDTH / 2)) {
				if(exploded == -1) {
					dumb_message(MKILL);
					exploded = 0;
					explodetimer = EXPLODETIME;
					indoor = 1;
					if(!keyalive) {
						keyalive = 1;
						keyx = plx;
						keyy = ply;
						refresh_info();
					}
				}
			} else if(dx + dy <= (PLAYERWIDTH / 2 + SWEEPERWIDTH / 2) *
						(PLAYERWIDTH / 2 + SWEEPERWIDTH / 2) * 2) {
				if((plx / mazescale == sweepx[i] / mazescale) &&
					(ply / mazescale == sweepy[i] / mazescale))
						dumb_message(MCLOSE);
			}
		}
		for(i=0; i<numguard; i++) {
			dx = (plx - guardx[i]) * (plx - guardx[i]);
			dy = (ply - guardy[i]) * (ply - guardy[i]);
			if(dx + dy <= (PLAYERWIDTH / 2 + GUARDWIDTH / 2) *
				(PLAYERWIDTH / 2 + GUARDWIDTH / 2)) {
				if(exploded == -1) {
					dumb_message(MKILL);
					exploded = 0;
					explodetimer = EXPLODETIME;
					indoor = 1;
					if(!keyalive) {
						keyalive = 1;
						keyx = plx;
						keyy = ply;
						refresh_info();
					}
				}
			} else if(dx + dy <= (PLAYERWIDTH / 2 + GUARDWIDTH / 2) *
						(PLAYERWIDTH / 2 + GUARDWIDTH / 2) * 2) {
				if((plx / mazescale == guardx[i] / mazescale) &&
					(ply / mazescale == guardy[i] / mazescale))
						dumb_message(MCLOSE);
			}
		}
	}
	for(i=0; i<numfuel; i++) {
		if(fuelalive[i]) {
			dx = (plx - fuelx[i]) * (plx - fuelx[i]);
			dy = (ply - fuely[i]) * (ply - fuely[i]);
			if(dx + dy <= (PLAYERWIDTH / 2 + FUELWIDTH / 2) * (PLAYERWIDTH
				/ 2 + FUELWIDTH / 2)) {
				dumb_message(MFUEL);
				playerfuel += FUELINCREMENT;
				if(playerfuel > FUELCAPACITY) playerfuel = FUELCAPACITY;
				fuelalive[i] = 0;
				fueltimer[i] = 25;
				score += 100;
			}
		}
	}

	/*	Grab the key
	*/
	if(keyalive && (exploded == -1)) {
		dx = (plx - keyx) * (plx - keyx);
		dy = (ply - keyy) * (ply - keyy);
		if(dx + dy <= (PLAYERWIDTH / 2 + KEYWIDTH / 2) * (PLAYERWIDTH / 2
			+ KEYWIDTH / 2)) {
			dumb_message(MKEY);
			score += 500;
			keyalive = 0;
			keytimer = 25;
			refresh_info();
		}
	}

	/*	Exit?
	*/
	if(!keyalive && indoor && !waiting && !leveldone) {
		plx = doorx;
		ply = doory;
		indoor = 1;
		dumb_message(MEXIT);
		waiting = bonus / BONUSINCREMENT + 50;
		playerphase = FACEFRONT;
		playerdir = STOP;
	}
}

/*	message_checks if the player has hit a message zone and if so, puts up
	the appropriate message
*/
message_check()
{
	register int	i;

	for(i=0; i<numlmessage; i++) {
		if(lmessage[i].unused) {
			if((plx >= lmessage[i].x1) && (ply >= lmessage[i].y1) &&
				(plx <= lmessage[i].x2) && (ply <= lmessage[i].y2)) {
					lmessage[i].unused = 0;
					if(lmessage[i].bigactive) {
						bigmessage(lmessage[i].bigtext, lmessage[i].bigcolor,
									lmessage[i].bigtime);
					}
					if(lmessage[i].active) {
						message(lmessage[i].text, lmessage[i].color,
									lmessage[i].time);
					}
			}
		}
	}
}

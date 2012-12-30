/*	update.c : routines that update all the objects for each frame
*/

#include "copyright.h"
#include "defs.h"

int	plx, ply, burn, playerphase, playerdir, playervert, playerfuel,
	walk, waiting, exploded, explodetimer;

int	numfire;
int	firex[MAXFIREBALLS], firey[MAXFIREBALLS], firedx[MAXFIREBALLS],
	firedy[MAXFIREBALLS], firephase[MAXFIREBALLS];

int	numfuel;
int	fuelx[MAXFUELPODS], fuely[MAXFUELPODS], fuelalive[MAXFUELPODS],
	fueltimer[MAXFUELPODS];

int	numsweep;
int	sweepx[MAXSWEEPERS], sweepy[MAXSWEEPERS], sweepphase[MAXSWEEPERS],
	sweepdir[MAXSWEEPERS], sweepfloor[MAXSWEEPERS], sweepspeed[MAXSWEEPERS],
	sweeprot[MAXSWEEPERS];

int	numguard;
int	guardx[MAXGUARDS], guardy[MAXGUARDS], guarddir[MAXGUARDS],
	guardspeed[MAXGUARDS], guardphase[MAXGUARDS], guardtime[MAXGUARDS];

int	keyx, keyy, keyalive, keytimer;
int	doorx, doory, indoor;

/*	update does all the object updating
*/
void	update()
{
	update_player();
	update_fireballs();
	update_sweepers();
	update_guards();
}

/*	update_player updates the player
*/
void	update_player()
{
	register int	flags;

	if(dead) {
		deadtimer--;
		if(!deadtimer) game_over();
		return;
	}
	if(waiting) {
		waiting--;
		score_bonus();
		if(waiting == 0) {
			leveldone = 1;
			level++;
			oldscore = -1;
			oldlevel = -1;
		}
		return;
	}
	if(exploded > -1) {
		explodetimer--;
		if(explodetimer == 0) {
			explodetimer = EXPLODETIME;
			exploded++;
			if(exploded >= EXPLODEFRAMES) {
				death();
			}
		}
		return;
	}
	nx = plx + PLAYERSPEED * playerdir;
	ny = ply + playervert;
	if(playerfuel <= 0) burn = 0;
	playervert += GRAVITY + THRUST * burn;
	playerfuel = playerfuel - BURNCOST * burn;
	if(playervert > DOWNSPEED) playervert = DOWNSPEED;
	if(playervert < UPSPEED) playervert = UPSPEED;
	flags = set_in_maze(PLAYERWIDTH / 2);
	if(flags & WALLDOWN) {
		walk = 1;
		if(playervert > 0) playervert = 0;
	} else {
		walk = 0;
	}
	if(flags & WALLUP) {
		if(playervert < 0) playervert = 0;
	}
	plx = nx;
	ply = ny;
	if(!burn && walk && (playerdir != 0)) {
		playerphase += playerdir;
		if(playerphase > FACERIGHT - 1) playerphase = FACEFRONT + 1;
		if(playerphase < FACELEFT + 1) playerphase = FACEFRONT - 1;
		if((playerdir == 1) && (playerphase < FACEFRONT))
			playerphase = FACEFRONT;
		if((playerdir == -1) && (playerphase > FACEFRONT))
			playerphase = FACEFRONT;
	} else playerphase = (playerdir + 1) * FACEFRONT;
	if((plx > doorx - 5) && (plx < doorx + 5) && (ply < doory + 5) &&
		(ply > doory - 5) && walk) indoor = 1;
	else indoor = 0;
}

/*	update_fireballs updates all the fireballs
*/
void	update_fireballs()
{
	register int	i, flags;

	for(i=0; i<numfire; i++) {
		nx = firex[i] + firedx[i];
		ny = firey[i] + firedy[i];
		flags = set_in_maze(FIREWIDTH / 2);
		if(flags & WALLRIGHT) if(firedx[i] > 0) firedx[i] = -firedx[i];
		if(flags & WALLLEFT) if(firedx[i] < 0) firedx[i] = -firedx[i];
		if(flags & WALLDOWN) if(firedy[i] > 0) firedy[i] = -firedy[i];
		if(flags & WALLUP) if(firedy[i] < 0) firedy[i] = -firedy[i];
		firex[i] = nx;
		firey[i] = ny;
		firephase[i] = random() % FIREPHASES;
	}
}

/*	update_guards updates all the guards
*/
void	update_guards()
{
	register int	i, dir, x, y, dx, dy, sp;

	for(i=0; i<numguard; i++) {
		guardtime[i]++;
		if(guardtime[i] == 10) {
			guardtime[i] = 0;
			guardphase[i] = random() % GUARDPHASES;
		}
		dx = guardx[i] % mazescale;
		dy = guardy[i] % mazescale;
		sp = guardspeed[i] / 2 + 1;
		if((dx > mazescale / 2 - sp) && (dx < mazescale / 2 + sp) &&
			(dy > mazescale / 2 - sp) && (dy < mazescale / 2 + sp)) {
				x = guardx[i] / mazescale;
				y = guardy[i] / mazescale;
				guardx[i] = x * mazescale + mazescale / 2;
				guardy[i] = y * mazescale + mazescale / 2;
				do {
					dir = random() % 4;
				} while((walldir[dir] & maze[x][y]) ||
						((walldir[dir] == oppdir[guarddir[i]]) &&
						!(maze[x][y] == ((WALLUP|WALLDOWN|WALLLEFT|WALLRIGHT)
						& ~(walldir[dir])))));
				guarddir[i] = dir;
		}
		guardx[i] += guardspeed[i] * xdir[guarddir[i]];
		guardy[i] += guardspeed[i] * ydir[guarddir[i]];
	}
}

/*	update_sweepers updates all the sweepers
*/
void	update_sweepers()
{
	register int	i, flags;

	for(i=0; i<numsweep; i++) {
		sweepphase[i] += sweeprot[i];
		if(sweepphase[i] == SWEEPERPHASES) sweepphase[i] = 0;
		if(sweepphase[i] < 0) sweepphase[i] = SWEEPERPHASES - 1;
		nx = sweepx[i];
		ny = sweepy[i];
		if(!(maze[nx / mazescale][ny / mazescale] & walldir[sweepfloor[i]])) {
			if(xdir[sweepfloor[i]] != 0) {
				nx = (nx / mazescale + xdir[sweepfloor[i]]) * mazescale +
						mazescale / 2 - xdir[sweepfloor[i]] * mazescale / 2;
			}
			if(ydir[sweepfloor[i]] != 0) {
			ny = (ny / mazescale + ydir[sweepfloor[i]]) * mazescale +
					mazescale / 2 - ydir[sweepfloor[i]] * mazescale / 2;
			}
			if(sweeprot[i] == 1) increment_sweepdir(i);
			else decrement_sweepdir(i);
		}
		nx += xdir[sweepdir[i]] * sweepspeed[i];
		ny += ydir[sweepdir[i]] * sweepspeed[i];
		flags = set_in_maze(SWEEPERWIDTH / 2);
		if(flags & WALLRIGHT) {
			if(sweepdir[i] == 1) {
				if(sweepfloor[i] == 2) decrement_sweepdir(i);
				else increment_sweepdir(i);
			}
		}
		if(flags & WALLLEFT) {
			if(sweepdir[i] == 3) {
				if(sweepfloor[i] == 0) decrement_sweepdir(i);
				else increment_sweepdir(i);
			}
		}
		if(flags & WALLUP) {
			if(sweepdir[i] == 0) {
				if(sweepfloor[i] == 1) decrement_sweepdir(i);
				else increment_sweepdir(i);
			}
		}
		if(flags & WALLDOWN) {
			if(sweepdir[i] == 2) {
				if(sweepfloor[i] == 3) decrement_sweepdir(i);
				else increment_sweepdir(i);
			}
		}
		sweepx[i] = nx;
		sweepy[i] = ny;
	}
}

/*	decrement_sweepdir does a circular decrement on sweeper i's direction
*/
void	decrement_sweepdir(int i)
{	
	sweepdir[i]--;
	if(sweepdir[i] < 0) sweepdir[i] = 3;
	sweepfloor[i]--;
	if(sweepfloor[i] < 0) sweepfloor[i] = 3;
}

/*	increment_sweepdir does a circular increment on sweeper i's direction
*/
void	increment_sweepdir(int i)
{	
	sweepdir[i]++;
	if(sweepdir[i] > 3) sweepdir[i] = 0;
	sweepfloor[i]++;
	if(sweepfloor[i] > 3) sweepfloor[i] = 0;
}

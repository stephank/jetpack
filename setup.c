/*	setup.c : routines used to setup level stuff.
*/

#include "copyright.h"
#include "defs.h"

/*	make_random_level takes its huge list of arguments and creates a
	level.
*/
void	make_random_level(int mx, int my, int msc, int md, int mt, int f,
		int fmin, int fmax, int s, int smin, int smax, int g, int gmin,
		int gmax, int p)
{
	float	r, t;

	mazescale = msc;
	r = ((float) md) / 100.0;
	t = ((float) mt) / 100.0;
	generate_maze(mx, my, r, t);
	random_player();
	random_fireballs(f, fmin, fmax);
	random_fuel(p);
	random_sweepers(s, smin, smax);
	random_guards(g, gmin, gmax);
	random_key();
	if(r > 0.75) r = 0.75;
	bonus = (((int) (mazewidth * mazeheight * mazescale * (1 - r)) -
				p * 100 + s * 50 + g * 50 + f * 100) / 100) * 10;
	if(bonus < 500) bonus = 500;
	initbonus = bonus;
}

/*	reset_level just gives values to certain global variables that need to
	be initialized for each level.
*/
void	reset_level()
{
	leveldone = 0;
	waiting = 0;
	messagetime = 0;
	bigmessagetime = 0;
	numlmessage = 0;
	numfire = 0;
	numfuel = 0;
	numsweep = 0;
	numguard = 0;
	bonustimer = 20;
	if(playerfuel < FUELCAPACITY / 2) playerfuel = FUELCAPACITY / 2;

#ifndef	BLIT
	init_zones();
#endif

}

/*	reset_game just gives values to certain global variables that need to
	be initialized for each game.
*/
void	reset_game()
{
	dead = 0;
    paused = 0;
    score = 0;
    men = 3;
    playerfuel = FUELCAPACITY;
    exploded = -1;
    level = 1;
	keyalive = 1;
	extramaninc = 1;
}

/*	place_player puts the player and the door at maze location (x,y).
*/
void	place_player(int x, int y)
{
	ply = (y + 1) * mazescale - 11;
	plx = x * mazescale + mazescale / 2;
	doorx = plx;
	doory = ply;
	exploded = -1;
	walk = 1;
	playervert = 0;
	indoor = 1;
	burn = 0;
	playerdir = STOP;
	playerphase = FACEFRONT;
}

/*	random_player puts the player and the door in a random maze location
	with a floor.
*/
void	random_player()
{
	register int	x, y;

	do {
		x = random() % mazewidth + 1;
		y = random() % mazeheight + 1;
	} while(!(maze[x][y] & WALLDOWN));
	ply = (y + 1) * mazescale - 11;
	plx = x * mazescale + mazescale / 2;
	doorx = plx;
	doory = ply;
	exploded = -1;
	walk = 1;
	playervert = 0;
	indoor = 1;
	burn = 0;
	playerdir = STOP;
	playerphase = FACEFRONT;
}

/*	place_fireball puts a fireball at world position (x,y) with speed
	[dx,dy]. The coordinates are modified to so the fireball won't sit on
	a wall.
*/
void	place_fireball(int x, int y, int dx, int dy)
{
	if(numfire == MAXFIREBALLS) return;
	nx = x;
	ny = y;
	set_in_maze(FIREWIDTH / 2);
	firex[numfire] = nx;
	firey[numfire] = ny;
	firedx[numfire] = dx;
	firedy[numfire] = dy;
	numfire++;
}

/*	random_fireballs puts num fireballs in the maze with speeds ranging
	from min to max. The coordinates of each are generated so they won't
	sit on a wall or are too close to the player.
*/
void	random_fireballs(int num, int min, int max)
{
	register int	i;

	if(num > MAXFIREBALLS) numfire = MAXFIREBALLS;
	else numfire = num;
	for(i=0; i<numfire; i++) {
		do {
			nx = random() % (mazewidth * mazescale) + mazescale;
			ny = random() % (mazeheight * mazescale) + mazescale;
			set_in_maze(FIREWIDTH / 2);
		} while((plx - nx) * (plx - nx) + (ply - ny) * (ply - ny) < 40000);
		firex[i] = nx;
		firey[i] = ny;
		do {
			firedx[i] = random() % 21 - 10;
			firedy[i] = random() % 21 - 10;
		} while(((firedx[i] ==  0) && (firedy[i] == 0)) || (abs(firedx[i]) +
				abs(firedy[i]) < min) || (abs(firedx[i]) + abs(firedy[i])
				> max));
		firephase[i] = random() % FIREPHASES;
	}
}

/*	place_guard puts a guard at maze location (x,y) with direction dir and
	speed speed
*/
void	place_guard(int x, int y, int dir, int speed)
{
	if(numguard == MAXGUARDS) return;
	guardx[numguard] = x * mazescale + mazescale / 2;
	guardy[numguard] = y * mazescale + mazescale / 2;
	guarddir[numguard] = dir;
	guardspeed[numguard] = speed;
	guardphase[numguard] = random() % GUARDPHASES;
	guardtime[numguard] = 0;
	numguard++;
}

/*	random_guards puts num guards in the maze with speeds ranging from min
	to max. Guards are nor placed on the same square as the player.
*/
void	random_guards(int num, int min, int max)
{
	register int	i, x, y, m;

	if(num > MAXGUARDS) numguard = MAXGUARDS;
	else numguard = num;
	for(i=0; i<numguard; i++) {
		do {
			x = random() % mazewidth + 1;
			y = random() % mazeheight + 1;
		} while((x == plx / mazescale) && (y == ply / mazescale));
		guardx[i] = x * mazescale + mazescale / 2;
		guardy[i] = y * mazescale + mazescale / 2;
		if(max > 9) m = 9;
		else m = max;
		guardspeed[i] = random() % (m - min + 1) + min;
		guardphase[i] = random() % GUARDPHASES;
		guardtime[i] = 0;
		do {
			guarddir[i] = random() % 4;
		} while(maze[x][y] & walldir[guarddir[i]]);
	}
}

/*	place_sweeper puts a sweeper at maze location (x,y) with a floor
	direction of floor, rotation of rot, and speed speed
*/
void	place_sweeper(int x, int y, int floor, int rot, int speed)
{
	if(numsweep == MAXSWEEPERS) return;
	sweepx[numsweep] = x * mazescale + mazescale / 2 + xdir[floor] *
						(mazescale / 2 - 11);
	sweepy[numsweep] = y * mazescale + mazescale / 2 + ydir[floor] *
						(mazescale / 2 - 11);
	sweepfloor[numsweep] = floor;
	if(rot == 1) {
		if(floor == 0) sweepdir[numsweep] = 3;
		else sweepdir[numsweep] = floor - 1;
	} else {
		if(floor == 3) sweepdir[numsweep] = 0;
		else sweepdir[numsweep] = floor + 1;
	}
	sweeprot[numsweep] = rot;
	sweepphase[numsweep] = random() % SWEEPERPHASES;
	sweepspeed[numsweep] = speed;
	numsweep++;
}

/*	random sweepers puts num sweepers in the maze with speeds from min to
	max. Sweepers will not be placed in the same square as the player.
*/
void	random_sweepers(int num, int min, int max)
{
	register int	i, x, y, dir, m;

	if(num > MAXSWEEPERS) numsweep = MAXSWEEPERS;
	else numsweep = num;
	for(i=0; i<numsweep; i++) {
		do {
			do {
				x = random() % mazewidth + 1;
				y = random() % mazeheight + 1;
			} while(!maze[x][y]);
			do {
				dir = random() % 4;
			} while(!(maze[x][y] & walldir[dir]));
			sweepx[i] = x * mazescale + mazescale / 2 + xdir[dir] *
						(mazescale / 2 - 11);
			sweepy[i] = y * mazescale + mazescale / 2 + ydir[dir] *
						(mazescale / 2 - 11);
			sweepfloor[i] = dir;
			if(random() & 01) {
				if(dir == 0) sweepdir[i] = 3;
				else sweepdir[i] = dir - 1;
				sweeprot[i] = 1;
			} else {
				if(dir == 3) sweepdir[i] = 0;
				else sweepdir[i] = dir + 1;
				sweeprot[i] = -1;
			}
		} while((plx / mazescale == sweepx[i]) &&
					(ply / mazescale == sweepy[i]));
		sweepphase[i] = random() % SWEEPERPHASES;
		if(max > 9) m = 9;
		else m = max;
		sweepspeed[i] = random() % (m - min + 1) + min;
	}
}

/*	place_fuel puts a fuel pod at world location (x,y). The coordinates
	are modified so the pod won't sit on a wall.
*/
void	place_fuel(int x, int y)
{
	if(numfuel == MAXFUELPODS) return;
	nx = x;
	ny = y;
	set_in_maze(FUELWIDTH / 2);
	fuelx[numfuel] = nx;
	fuely[numfuel] = ny;
	fuelalive[numfuel] = 1;
	numfuel++;
}

/*	random_fuel places num fuel pods in the maze. The coordinates are
	generated so they don't sit on walls or too close together.
*/
void	random_fuel(int num)
{
	register int	i, j, ok, radius;

	if(num > MAXFUELPODS) numfuel = MAXFUELPODS;
	else numfuel = num;
	if(numfuel == 0) return;
	radius = (mazewidth * mazeheight * mazescale * mazescale) / (6 * numfuel);
	for(i=0; i<numfuel; i++) {
		do {
			nx = random() % (mazewidth * mazescale) + mazescale;
			ny = random() % (mazeheight * mazescale) + mazescale;
		set_in_maze(FUELWIDTH / 2);
		ok = 1;
		for(j=0; j<i; j++) {
			if((fuelx[j] - nx) * (fuelx[j] - nx) +
				(fuely[j] - ny) * (fuely[j] - ny) < radius) ok = 0;
		}
		} while(!ok);
		fuelx[i] = nx;
		fuely[i] = ny;
		fuelalive[i] = 1;
	}
}

/*	place_key puts the key at world coordinates (x,y). The coordinates are
	modified so the key will not sit on a wall.
*/
void	place_key(int x, int y)
{
	nx = x;
	ny = y;
	set_in_maze(KEYWIDTH / 2);
	keyx = nx;
	keyy = ny;
	keyalive = 1;
}

/*	random_key puts the key in a random location. The coordinates are
	generated so the key won't be on a wall or close to the player.
*/
void	random_key()
{
	register int	radius,dist;

	if(mazewidth > mazeheight) {
		if(plx > (mazewidth * mazescale) / 2) dist = plx;
		else dist = mazewidth * mazescale - plx;
	} else {
		if(ply > (mazeheight * mazescale) / 2) dist = ply;
		else dist = mazeheight * mazescale - ply;
	}
	radius = (dist * dist * 9) / 16;
	do {
		nx = random() % (mazewidth * mazescale) + mazescale;
		ny = random() % (mazeheight * mazescale) + mazescale;
	} while((plx - nx) * (plx - nx) + (ply - ny) * (ply - ny) < radius);
	set_in_maze(KEYWIDTH / 2);
	keyx = nx;
	keyy = ny;
	keyalive = 1;
}

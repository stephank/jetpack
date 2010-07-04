/*	special.c : routines that read in the predefined levels from files.
*/

#include "copyright.h"
#include "defs.h"

#ifndef	LEVELPATH
#define	LEVELPATH	"jetpack.lev"
#endif

/*	special_setup checks to see if the current level has a level file, and
	if so, to read in the level from the level file
*/
int	special_setup()
{
	register int	i, j;
	char			filename[MAXPATHLEN];
	FILE			*fp;
	int				gx, gy, gd, gs, fx, fy, fdx, fdy, sx, sy, sf, sd, ss,
					px, py, kx, ky, length, ng, nf, ns, np, playerx,
					playery;

	sprintf(filename, "%s%03d", LEVELPATH, level);
	fp = fopen(filename, "r");
	if(fp == NULL) return(0);
	rewind(fp);
	fflush(fp);
	mazelcount = 0;
	fscanf(fp, " %d %d %d ", &mazewidth, &mazeheight, &mazescale);
	for(i=0; i<mazeheight+2; i++) {
		for(j=0; j<mazewidth+2; j++) {
			fscanf(fp, "%d", &(maze[j][i]));
		}
	}
	convert_maze(mazewidth, mazeheight);
	fscanf(fp, " %d %d %d ", &playerx, &playery, &bonus);
	place_player(playerx, playery);
	fscanf(fp, " %d ", &ng);
	for(i=0; i<ng; i++) {
		fscanf(fp, " %d %d %d %d ", &gx, &gy, &gd, &gs);
		place_guard(gx, gy, gd, gs);
	}
	fscanf(fp, " %d ", &nf);
	for(i=0; i<nf; i++) {
		fscanf(fp, " %d %d %d %d ", &fx, &fy, &fdx, &fdy);
		place_fireball(fx, fy, fdx, fdy);
	}
	fscanf(fp, " %d ", &ns);
	for(i=0; i<ns; i++) {
		fscanf(fp, " %d %d %d %d %d ", &sx, &sy, &sf, &sd, &ss);
		place_sweeper(sx, sy, sf, sd, ss);
	}
	fscanf(fp, " %d ", &np);
	for(i=0; i<np; i++) {
		fscanf(fp, " %d %d ", &px, &py);
		place_fuel(px, py);
	}
	fscanf(fp, " %d %d ", &kx, &ky);
	place_key(kx, ky);
	fscanf(fp, " %d ", &numlmessage);
	for(i=0; i<numlmessage; i++) {
		fscanf(fp, " %d %d %d %d %d %d %d %d ", &(lmessage[i].x1),
				&(lmessage[i].y1), &(lmessage[i].x2), &(lmessage[i].y2),
				&(lmessage[i].color), &(lmessage[i].bigcolor),
				&(lmessage[i].time), &(lmessage[i].bigtime));
		fscanf(fp, " %d ", &length);
		for(j=0; j<length; j++) {
			fscanf(fp, "%c", lmessage[i].bigtext+j);
		}
		lmessage[i].bigtext[length] = '\0';
		lmessage[i].bigactive = (length != 0);
		fscanf(fp, " %d ", &length);
		for(j=0; j<length; j++) {
			fscanf(fp, "%c", lmessage[i].text+j);
		}
		lmessage[i].text[length] = '\0';
		lmessage[i].active = (length != 0);
		lmessage[i].unused = 1;
	}
	initbonus = bonus;
	return(1);
}

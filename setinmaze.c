/*	setinmaze.c : routine used to keep objects off the maze walls
*/

#include "copyright.h"
#include "defs.h"

int	nx, ny;

/*	set_in_maze : This ugly procedure checks the global variables nx and
	ny against the maze and adjusts them if they sit on a wall. The reason
	there are double checks for WALLUP and WALLDOWN is to avoid side
	effects like being stopped by a vertical wall on the other side of the
	ceiling you're touching with your head.
*/
int	set_in_maze(off)
int	off;
{
	int flags = 0;

	if(((ny % mazescale) >= mazescale - (off + 1)) &&
		(maze[nx/mazescale][ny/mazescale] & WALLDOWN)) {

			ny = mazescale - (off + 1) + ((ny/mazescale) * mazescale);
			flags |= WALLDOWN;
		
	} else if(((ny % mazescale) <= (off + 1)) &&
		(maze[nx/mazescale][ny/mazescale] & WALLUP)) {
		
			ny = (off + 1) + ((ny/mazescale) * mazescale);
			flags |= WALLUP;
		
	}
	if(((nx % mazescale) >= mazescale - (off + 1)) &&
		((maze[nx/mazescale][ny/mazescale] & WALLRIGHT) ||
		(((ny % mazescale) > mazescale - off) &&
		(maze[nx/mazescale][ny/mazescale+1] & WALLRIGHT)) ||
		(((ny % mazescale) < off) &&
		(maze[nx/mazescale][ny/mazescale-1] & WALLRIGHT)))) {
			
			nx = mazescale - (off + 1) + ((nx/mazescale) * mazescale);
			flags |= WALLRIGHT;
		
	} else if(((nx % mazescale) <= (off + 1)) &&
		((maze[nx/mazescale][ny/mazescale] & WALLLEFT) ||
		(((ny % mazescale) > mazescale - off) &&
		(maze[nx/mazescale][ny/mazescale+1] & WALLLEFT)) ||
		(((ny % mazescale) < off) &&
		(maze[nx/mazescale][ny/mazescale-1] & WALLLEFT)))) {
			
			nx = (off + 1) + ((nx/mazescale) * mazescale);
			flags |= WALLLEFT;
		
	}
	if(((ny % mazescale) >= mazescale - (off + 1)) &&
		((maze[nx/mazescale][ny/mazescale] & WALLDOWN) ||
		(((nx % mazescale) > mazescale - off) &&
		(maze[nx/mazescale+1][ny/mazescale] & WALLDOWN)) ||
		(((nx % mazescale) < off) &&
		(maze[nx/mazescale-1][ny/mazescale] & WALLDOWN)))) {
		
			ny = mazescale - (off + 1) + ((ny/mazescale) * mazescale);
			flags |= WALLDOWN;
		
	} else if(((ny % mazescale) <= (off + 1)) &&
		((maze[nx/mazescale][ny/mazescale] & WALLUP) ||
		(((nx % mazescale) > mazescale - off) &&
		(maze[nx/mazescale+1][ny/mazescale] & WALLUP)) ||
		(((nx % mazescale) < off) &&
		(maze[nx/mazescale-1][ny/mazescale] & WALLUP)))) {
		
			ny = (off + 1) + ((ny/mazescale) * mazescale);
			flags |= WALLUP;
		
	}
	return(flags);
}

/*	maze.c : routines that generate the mazes
*/

#include "copyright.h"
#include "defs.h"

/*	These arrays are used to implement direction.
*/
char	walldir[4] = { WALLUP, WALLRIGHT, WALLDOWN, WALLLEFT };
char	oppdir[4] = { WALLDOWN, WALLLEFT, WALLUP, WALLRIGHT };
int		xdir[4] = { 0, 1, 0, -1 };
int		ydir[4] = { -1, 0, 1, 0 };

int			maze[MAXMAZEWIDTH][MAXMAZEHEIGHT];
struct line	mazelines[MAXLINES];
int			mazescale, mazelcount, mazewidth, mazeheight;

/*	generate maze uses a digging algorithm to generate the maze.
*/
void	generate_maze(int width, int height, float ratio, float twistratio)
{
	register int	x, y, d, cells, lines, twist, dug;
	char			celldone[MAXMAZEWIDTH][MAXMAZEHEIGHT];

	mazelcount = 0;
	if(width > MAXMAZEWIDTH - 1) width = MAXMAZEWIDTH - 1;
	if(height > MAXMAZEHEIGHT - 1) height = MAXMAZEHEIGHT - 1;
	mazewidth = width;
	mazeheight = height;
	lines = (int) ((mazewidth-1)*(mazeheight-1) * ratio);
	twist = (int) (twistratio * width * height);

	/*	Make the maze full of walls.
	*/
	for(x=0; x < MAXMAZEWIDTH; x++)
		for(y=0; y < MAXMAZEHEIGHT; y++) {
			maze[x][y] = (WALLUP | WALLDOWN | WALLLEFT | WALLRIGHT);
			celldone[x][y] = 0;
		}

	/*	Alter the edges of the maze so the algorithm uses them as
		boundaries.
	*/
	for(x=0; x < width + 1; x++) {
		celldone[x][0] = 1;
		celldone[x][height+1] = 1;
		maze[x][0] = 0;
		maze[x][height+1] = WALLUP;
	}
	for(y=0; y < height + 1; y++) {
		celldone[0][y] = 1;
		celldone[width+1][y] = 1;
		maze[0][y] = 0;
		maze[width+1][y] = WALLLEFT;
	}
	maze[width+1][height+1] = 0;

	/*	Pick a random cell to start and clear it.
	*/
	x = random() % width + 1;
	y = random() % height + 1;
	cells = width * height - 1;
	celldone[x][y] = 1;
	dug = 0;
	while(cells > 0) {

		/*	If we aren't in a dug cell or the cell we are in is completely
			surrounded by dug cells or we've dug the maximum length of a
			passage, go to a new cell.
		*/
		while((!celldone[x][y]) ||
			(celldone[x][y-1] && celldone[x-1][y] && celldone[x][y+1] &&
			celldone[x+1][y]) || (dug > twist)) {
			x = random() % width + 1;
			y = random() % height + 1;
			dug = 0;
		}

		/*	Pick a direction that has a wall
		*/
		do {
			d = random() % 4;
		} while(celldone[x+xdir[d]][y+ydir[d]]);

		/* Dig!
		*/
		cells--;
		maze[x][y] &= ~(walldir[d]);
		x += xdir[d];
		y += ydir[d];
		celldone[x][y] = 1;
		maze[x][y] &= ~(oppdir[d]);
		dug++;
	}

	/*	Remove random lines according to the ratio passed.
	*/
	while(lines > 0) {
		do {
			do {
				x = random() % width + 1;
				y = random() % height + 1;
			} while(!maze[x][y]);
			do {
				d = random() % 4;
			} while(!(maze[x][y] & walldir[d]));
		} while((x+xdir[d] <= 0) || (x+xdir[d] >= mazewidth + 1) ||
				(y+ydir[d] <= 0) || (y+ydir[d] >= mazeheight + 1));
		maze[x][y] &= ~(walldir[d]);
		maze[x+xdir[d]][y+ydir[d]] &= ~(oppdir[d]);
		lines--;
	}

	/*	Create the lines used for drawing the maze.
	*/
	convert_maze(width, height);
}

/*	add_line adds a world-coordinates line to the maze drawing
*/
void	add_line(int x1, int y1, int x2, int y2)
{
	mazelines[mazelcount].x1 = x1;
	mazelines[mazelcount].y1 = y1;
	mazelines[mazelcount].x2 = x2;
	mazelines[mazelcount].y2 = y2;
	mazelcount++;
}

/*	convert_maze creates a world drawing of the maze from its array
	structure.
*/
void	convert_maze(int width, int height)
{
	int x, y;

	for(x=1; x<width+2; x++)
		for(y=1; y <height+2; y++) {
			if(maze[x][y] & WALLUP) add_line(x*mazescale, y*mazescale,
												(x+1)*mazescale, y*mazescale);
			if(maze[x][y] & WALLLEFT) add_line(x*mazescale, y*mazescale,
												x*mazescale, (y+1)*mazescale);
		}
}

/*  setup_title_maze creates the trivial maze that the title screen uses,
    and creates the monsters that cavort in it.
*/
void	setup_title_maze()
{
    mazewidth = 4;
    mazeheight = 3;
    mazescale = 100;
    mazelcount = 0;
    maze[0][0] = 0; maze[1][0] = 4; maze[2][0] = 4; maze[3][0] = 4;
    maze[4][0] = 4; maze[5][0] = 0; maze[0][1] = 2; maze[1][1] = 9;
    maze[2][1] = 1; maze[3][1] = 1; maze[4][1] = 3; maze[5][1] = 8;
    maze[0][2] = 2; maze[1][2] = 8; maze[2][2] = 0; maze[3][2] = 0;
    maze[4][2] = 2; maze[5][2] = 8; maze[0][3] = 2; maze[1][3] = 12;
    maze[2][3] = 4; maze[3][3] = 4; maze[4][3] = 6; maze[5][3] = 8;
    maze[0][4] = 0; maze[1][4] = 1; maze[2][4] = 1; maze[3][4] = 1;
    maze[4][4] = 1; maze[5][4] = 0;
    convert_maze(4, 3);
    plx = 300; ply = 170;
    playerphase = FACEFRONT; playerdir = STOP; burn = 0;
    random_fireballs(random() % 3 + 2, 3, 7);
    random_guards(random() % 3 + 1, 3, 7);
    numsweep = 0;
    place_sweeper(random() % 4 + 1, 0, 2, 1, random() % 5 + 1);
    place_sweeper(random() % 4 + 1, 4, 0, 1, random() % 5 + 1);
    place_sweeper(0,random() % 3 + 1, 1, 1, random() % 5 + 1);
    place_sweeper(5,random() % 3 + 1, 3, 1, random() % 5 + 1);
}

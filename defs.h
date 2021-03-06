/*	defs.h : externally defined things for jetpack
*/

#include "copyright.h"

/*	here are all the include files used by the different source files.
	Since every source file includes this one, it makes dependencies a lot
	more efficient.
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<sys/param.h>
#include	<sys/file.h>
#include	<sys/types.h>
#include	<sys/time.h>
#include	<X11/Xlib.h>
#include	<X11/Xutil.h>
#include	<X11/keysym.h>
#include	<X11/keysymdef.h>

/*	Windowing stuff
*/

/*	Dimensions of the window and the information window
*/
#define WINDOWWIDTH		500
#define WINDOWHEIGHT	500
#define INFOHEIGHT		50

/*	Error factor used in non-blitting version for overlap testing.
*/
#ifdef	BLIT
#define	EDGE	0
#else
#define	EDGE	10
#endif

/*	Definitions for erase zone types.
*/
#define	ZLINE		0
#define	ZPLAYER		1
#define	ZBURN		2
#define	ZEXPLODE	3
#define	ZFIRE		4
#define	ZGUARD		5
#define	ZSWEEP		6
#define	ZFUEL		7
#define	ZKEY		8
#define	ZDOOR		9
#define	ZSMESS		10
#define	ZBMESS		11

/*	Dimensions and position of high score table pixmap within window
*/
#define SCOREWIDTH	400
#define SCOREHEIGHT	300
#define SCOREX		50
#define SCOREY		180

extern Display			*display;
extern Colormap			colormap;
extern Window			rootwindow, gamewindow, infowindow, dialogwindow,
						dialogstringwindow, dialogbuttonwindow;
extern int				screen, depth;
extern unsigned long	blackpixel, whitepixel;
extern XFontStruct		*fontinfo, *bigfontinfo;
extern Pixmap			drawpixmap, scorepixmap;
extern int				dialogwidth, dialogheight;

/*	colors
*/

#define	NCOLORS	8

#define	CBLACK	0
#define	CRED	1
#define	CORANGE	2
#define	CYELLOW	3
#define	CBLUE	4
#define	CGREY	5
#define	CGREEN	6
#define	CWHITE	7

/*	two GC's in each colormap entry are needed -- one for the small
	fixed font, and one for the large Courier font.
*/
struct colors {
	char	*name;
	GC		smallgc;
	GC		biggc;
	int		pixelvalue;
};

extern struct colors	ctable[8];

/*	maze stuff
*/

#define	MAXMAZEWIDTH	100
#define	MAXMAZEHEIGHT	100
#define	MAXLINES MAXMAZEWIDTH*MAXMAZEHEIGHT*2+MAXMAZEWIDTH+MAXMAZEHEIGHT

#define	WALLUP		0x0001
#define	WALLRIGHT	0x0002
#define	WALLDOWN	0x0004
#define	WALLLEFT	0x0008

struct line {
	int x1, y1, x2, y2;
};

extern int			maze[MAXMAZEWIDTH][MAXMAZEHEIGHT];
extern struct line	mazelines[MAXLINES];
extern int			mazescale, mazewidth, mazeheight, mazelcount;
extern char			walldir[4], oppdir[4];
extern int			xdir[4], ydir[4];

/*	message stuff
*/

#define	MAXMESSAGELENGTH	100
#define	MAXBIGMESSAGELENGTH	30
#define	MAXLEVELMESSAGES	20

/*	These constants are used to call the dumb_message function when
	something worth commenting on happens
*/
#define	MFUEL		0
#define	MKILL		1
#define	MKEY		2
#define	MCLOSE		3
#define	MSUICIDE	4
#define	MEXIT		5
#define MQUIT		6

/*	Structure used to store the messages triggered in certain areas of a
	level
*/
struct levelmessage {
	char	text[MAXMESSAGELENGTH];
	char	bigtext[MAXBIGMESSAGELENGTH];
	int		x1, y1, x2, y2;
	int		color, bigcolor, time, bigtime;
	int		active, bigactive, unused;
};

extern int					numlmessage;
extern struct levelmessage	lmessage[MAXLEVELMESSAGES];
extern char					messagestring[MAXMESSAGELENGTH],
							bigmessagestring[MAXBIGMESSAGELENGTH];
extern int					messagex, messagey, bigmessagex, bigmessagey,
							messagecolor, bigmessagecolor,
							messagelength, bigmessagelength,
							messagewidth, bigmessagewidth,
							messagetime, bigmessagetime;

/*	player stuff
*/

#define	PLAYERWIDTH		20
#define	PLAYERHEIGHT	20
#define	PLAYERPARTS		2
#define	BURNWIDTH		20
#define	BURNHEIGHT		20
#define	BURNPARTS		2
#define	BURNDIRECTIONS	3

#define	FACELEFT	0
#define	FACEFRONT	7
#define	FACERIGHT	14

#define PLAYERPHASES	FACERIGHT+1

#define	GOLEFT	-1
#define	STOP	0
#define	GORIGHT	1

extern Pixmap	playerpix[PLAYERPHASES], burnpix[BURNDIRECTIONS];
extern int		plx, ply, burn, playerphase, playerdir, playervert,
				playerfuel, walk, waiting, dead, deadtimer;

/*	pixmaps for exploding player
*/

#define	EXPLODEWIDTH	30
#define	EXPLODEHEIGHT	30
#define	EXPLODEFRAMES	10
#define	EXPLODETIME		2

extern Pixmap	explodepix[EXPLODEFRAMES];
extern int		exploded, explodetimer;

/*	wall sweepers
*/

#define	SWEEPERWIDTH	20
#define	SWEEPERHEIGHT	20
#define	MAXSWEEPERS		40
#define	SWEEPERPARTS	3
#define	SWEEPERPHASES	4
#define	MAXSWEEPSPEED	10

extern Pixmap	sweeperpix[SWEEPERPHASES];
extern int		numsweep;
extern int		sweepx[MAXSWEEPERS], sweepy[MAXSWEEPERS],
				sweepdir[MAXSWEEPERS], sweepfloor[MAXSWEEPERS],
				sweepphase[MAXSWEEPERS], sweepspeed[MAXSWEEPERS],
				sweeprot[MAXSWEEPERS];

/*	fireballs
*/

#define	FIREWIDTH		20
#define	FIREHEIGHT		20
#define	MAXFIREBALLS	40
#define	FIREPARTS		2
#define	FIREPHASES		5

extern Pixmap	firepix[FIREPHASES];
extern int		numfire;
extern int		firex[MAXFIREBALLS], firey[MAXFIREBALLS],
				firedx[MAXFIREBALLS], firedy[MAXFIREBALLS],
				firephase[MAXFIREBALLS];

/*	guards
*/

#define	GUARDWIDTH		21
#define	GUARDHEIGHT		21
#define	MAXGUARDS		40
#define	GUARDPARTS		2
#define	GUARDLIGHTS		3
#define GUARDFLAMEPARTS	2
#define GUARDDIRECTIONS	4
#define	GUARDPHASES		8
#define MAXGUARDSPEED	9

extern Pixmap	guardpix[GUARDDIRECTIONS][GUARDPHASES];
extern int		numguard;
extern int		guardx[MAXGUARDS], guardy[MAXGUARDS], guarddir[MAXGUARDS],
				guardspeed[MAXGUARDS], guardphase[MAXGUARDS],
				guardtime[MAXGUARDS];

/*	fuel pods
*/

#define	FUELWIDTH		20
#define	FUELHEIGHT		20
#define	MAXFUELPODS		50
#define	FUELPARTS		4
#define	FUELINCREMENT	100

extern Pixmap	fuelpix;
extern int		numfuel;
extern int		fuelx[MAXFUELPODS], fuely[MAXFUELPODS],
				fuelalive[MAXFUELPODS], fueltimer[MAXFUELPODS];

/*	key
*/

#define	KEYWIDTH	10
#define	KEYHEIGHT	10
#define	KEYPARTS	1

extern Pixmap	keypix;
extern int		keyx, keyy, keyalive, keytimer;

/*	door
*/

#define	DOORWIDTH	30
#define	DOORHEIGHT	30
#define	DOORPARTS	1
#define	DOORFRAMES	2

extern Pixmap	doorpix[DOORFRAMES];
extern int		doorx, doory;
extern int		indoor;

/*	pixmap of extra man for info window
*/

#define	EXTRAMANWIDTH	12
#define	EXTRAMANHEIGHT	20

extern Pixmap	extramanpix;

/*	dummy coordinates for checking collisions with the maze
*/

extern int	nx, ny;

/*	world data
*/

#define	UPSPEED			-10
#define	DOWNSPEED		10
#define THRUST			-2
#define GRAVITY			1
#define FUELCAPACITY	1000
#define BURNCOST		1
#define PLAYERSPEED		5

/*	game data
*/

#define	MAXHIGHSCORES	25
#define	BONUSINCREMENT	10
#define	BONUSTIME		20
#define EXTRAMANSCORE	10000
#define USECDELAY		45000

extern int	men, score, bonus, bonustimer, initbonus, level,
			oldscore, oldlevel, extramaninc;

/*	game state
*/

extern int	paused, leveldone, gameover, lastscore;

/*	function prototypes
*/
void	set_up_bitmaps();
void	bonus_check();
void	decrement_bonus();
void	extra_man_check();
void	score_bonus();
void	collision_check();
void	message_check();
void	demo();
void	title_screen();
void	demo_walk(int x);
void	demo_fly(int x, int y);
void	demo_delay();
void	demo_player();
void	draw();
void	draw_maze();
void	draw_player();
void	draw_fireballs();
void	draw_guards();
void	draw_sweepers();
void	draw_treasures();
void	draw_door();
int	transx(int x, int off);
int	transy(int y, int off);
void	refresh_info();
void	draw_messages();
void	draw_info();
void	place_string(char *s, int y, int color, XFontStruct *info);
void	title_draw();
void	title_refresh();
void	init_zones();
void	set_zone(int x, int y, int w, int h, int type, int num);
int	zone_intersect(int i, int j);
void	erase_draw_zones();
void	erase_recur(int i);
void	draw_recur(int i);
void	refresh();
int	event_filter();
void	key_hit(XKeyEvent *event);
int	demo_event_filter();
void	title_event_filter();
void	title_key_hit(XKeyEvent *event);
void	death();
void	game_over();
void	init_X();
void	set_up_contexts();
void	set_up_colors();
int	main(int argc, char **argv);
void	generate_maze(int width, int height, float ratio, float twistratio);
void	add_line(int x1, int y1, int x2, int y2);
void	convert_maze(int width, int height);
void	setup_title_maze();
void	message(char *s, int color, int time);
void	bigmessage(char *s, int color, int time);
void	dumb_message(int kind);
void	normal_level();
void	quit_game();
void	open_scores();
void	close_scores();
void	read_scores();
void	write_scores();
void	read_new_scores();
void	check_score();
void	draw_scores(int which);
void	shift_highs(int i);
void	get_high_name(char *buf);
int	dialog_key(XKeyEvent *event);
void	display_dialog(Window w);
int	set_in_maze(int off);
void	make_random_level(int mx, int my, int msc, int md, int mt, int f,
		int fmin, int fmax, int s, int smin, int smax, int g, int gmin,
		int gmax, int p);
void	reset_level();
void	reset_game();
void	place_player(int x, int y);
void	random_player();
void	place_fireball(int x, int y, int dx, int dy);
void	random_fireballs(int num, int min, int max);
void	place_guard(int x, int y, int dir, int speed);
void	random_guards(int num, int min, int max);
void	place_sweeper(int x, int y, int floor, int rot, int speed);
void	random_sweepers(int num, int min, int max);
void	place_fuel(int x, int y);
void	random_fuel(int num);
void	place_key(int x, int y);
void	random_key();
int	special_setup();
void	begin_timer();
void	stop_timer();
unsigned long	get_timer();
void	update();
void	update_player();
void	update_fireballs();
void	update_guards();
void	update_sweepers();
void	decrement_sweepdir(int i);
void	increment_sweepdir(int i);
void	set_up_windows();

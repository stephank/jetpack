/*	gameover.c : routines used when player dies or game is over
*/

#include "copyright.h"
#include "defs.h"

int	dead, deadtimer;

/*	death is called when a player is killed.
*/
void	death()
{
    men--;
    if(men < 0) {
		dead = 1;
		deadtimer = 50;
		bigmessage("Game Over", CBLACK, 100);
		return;
	}
    if(!keyalive) {
        keyx = plx;
        keyy = ply;
        keyalive = 1;
    }
    plx = doorx;
    ply = doory;
    playerfuel = 1000;
    burn = 0;
    walk = 1;
    playerdir = STOP;
    playervert = 0;
    playerphase = FACEFRONT;
    exploded = -1;
    XClearWindow(display, gamewindow);
    XClearWindow(display, infowindow);
    refresh_info();
}

/*	game_over is called when a player loses his/her last life. The high
	score table is checked, and some cleanup is done.
*/
void	game_over()
{
	lastscore = score;
	check_score();
    XClearWindow(display, gamewindow);
    XClearWindow(display, infowindow);
	gameover = 1;
	leveldone = 1;
}

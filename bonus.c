/*	bonus.c : bonus calculating routines for jetpack
*/

#include "copyright.h"
#include "defs.h"

int	bonus, bonustimer, initbonus, extramaninc;

/*	bonus_check decrements the bonus and checks to see if the player has
	earned an extra player
*/
void	bonus_check()
{
	decrement_bonus();
	extra_man_check();
}

/*	decrement_bonus decrements the bonus
*/
void	decrement_bonus()
{
	bonustimer--;
	if(!bonustimer) {
		bonus -= BONUSINCREMENT;
		if(bonus < 0) bonus = 0;
		bonustimer = BONUSTIME;
	}
}

/*	extra_man_check awards an extra player if he/she has earned it
*/
void	extra_man_check()
{
	if(score >= EXTRAMANSCORE * extramaninc) {
		men++;
		extramaninc++;
		if(!waiting) bigmessage("BONUS!", CBLACK, 50);
		message("Extra player awarded!", CBLACK, 100);
		refresh_info();
	}
}

/*	score_bonus counts down the bonus at the end of a level
*/
void	score_bonus()
{
	char buf[14];

	sprintf(buf, "Bonus: %06d", bonus);
	bigmessage(buf, CYELLOW, 1);
	bonus -= BONUSINCREMENT;
	if(bonus < 0) bonus = 0;
	else score += BONUSINCREMENT;
}

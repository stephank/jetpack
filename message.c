/*	message.c : routines that implement a message display system
*/

#include "copyright.h"
#include "defs.h"
#include "message.h"

struct levelmessage	lmessage[MAXLEVELMESSAGES];
int					numlmessage;

char	messagestring[MAXMESSAGELENGTH],
		bigmessagestring[MAXBIGMESSAGELENGTH];
int		messagex, messagey, bigmessagex, bigmessagey,
		messagetime = 0, bigmessagetime = 0,
		messagecolor, bigmessagecolor,
		messagelength = 0, bigmessagelength = 0,
		messagewidth, bigmessagewidth;

/*	message puts a message s on the screen in color for time
*/
message(s, color, time)
char	*s;
int		color, time;
{
	messagecolor = color;
	messagetime = time;
	strncpy(messagestring, s, MAXMESSAGELENGTH);
	messagelength = strlen(messagestring);
	messagewidth = XTextWidth(fontinfo, messagestring, messagelength);
	messagey = WINDOWHEIGHT / 5 + 50;
	messagex = WINDOWWIDTH / 2 - messagewidth / 2;
}

/*	message puts a big message s on the screen in color for time
*/
bigmessage(s, color, time)
char	*s;
int		color, time;
{
	bigmessagecolor = color;
	bigmessagetime = time;
	strncpy(bigmessagestring, s, MAXBIGMESSAGELENGTH);
	bigmessagelength = strlen(bigmessagestring);
	bigmessagewidth = XTextWidth(bigfontinfo, bigmessagestring,
									bigmessagelength);
	bigmessagey = WINDOWHEIGHT / 5;
	bigmessagex = WINDOWWIDTH / 2 - bigmessagewidth / 2;
}

/*	dumb_message puts up a message to the player when something happens.
	Called dumb because the procedure is ugly and the messages are mostly
	snide remarks.
*/
dumb_message(kind)
int	kind;
{
	register int	opinion, num;

	if(gameover) return;
	if((messagetime != 0) && (kind != MKILL) && (kind != MSUICIDE) &&
		(kind != MEXIT)) return;
	if(exploded > -1) return;
	if((dumbmessagechance[kind] > 1) &&
		(random() % (dumbmessagechance[kind]) == 0)) return;
	opinion = -1;
	switch(kind) {
		case MKILL:
		case MCLOSE:
		case MSUICIDE:
		case MQUIT:
			opinion = 0;
			break;
		case MFUEL:
			if(playerfuel > FUELCAPACITY - FUELINCREMENT) opinion = 0;
			if(playerfuel < 300) opinion = 1;
			break;
		case MKEY:
			if(playerfuel < 400) {
				opinion = 2;
				break;
			}
			if(bonus < initbonus / 3) {
				opinion = 0;
				break;
			}
			if(bonus > (initbonus * 3) / 4) {
				opinion = 1;
				break;
			}
			opinion = 3;
			break;
		case MEXIT:
			if(bonus == 0) {
				opinion = 0;
				break;
			}
			if(bonus >  (2 * initbonus) / 3) {
				opinion = 1;
				break;
			}
			if(bonus < initbonus / 3) {
				opinion = 2;
				break;
			}
			opinion = 3;
			break;
		default:
			break;
	}
	if(opinion == -1) return;
	num = random() % numdumbmessages[kind][opinion];
	message(dumbmessage[kind][opinion][num], random() % (NCOLORS - 1) + 1,
	dumbmessagetime[kind]);
}

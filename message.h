/*	message.h : silly messages used in jetpack.
*/

#include "copyright.h"

#define	MAXDUMBMESSAGES		10
#define	DUMBMESSAGETYPES	7
#define	DUMBMESSAGEOPINIONS	4

static int	numdumbmessages[DUMBMESSAGETYPES][DUMBMESSAGEOPINIONS] = {
	{ 8, 9 },
	{ 9 },
	{ 10, 9, 8, 7 },
	{ 10 },
	{ 8 },
	{ 10, 9, 9, 10 },
	{ 10 },
};

static int	dumbmessagetime[DUMBMESSAGETYPES] = {
	50, 20, 50, 20, 50, 100, 100,
};

static int	dumbmessagechance[DUMBMESSAGETYPES] = {
	3, 2, 1, 2, 1, 1, 1,
};

static char dumbmessage[DUMBMESSAGETYPES][DUMBMESSAGEOPINIONS][MAXDUMBMESSAGES][MAXMESSAGELENGTH] = {

/*	MFUEL
*/
{
	{
		"Wasteful.",
		"It'd people like you who caused the oil crisis!",
		"Did you really need that?",
		"I think you have plenty.",
		"Your tank only holds 1000.",
		"When you're running out of fuel later, don't come crying to me.",
		"Greedy, aren't you?",
		"If you use all the fuel up now, you'll run out later.",
	},
	{
		"Lucky break.",
		"You should be more careful!",
		"Do you like living on the edge?",
		"Whew!",
		"Don't let yourself run out!",
		"What a relief!",
		"I hope there's more around here.",
		"Try to pick up fuel more often.",
		"Running on empty is a good way to lose.",
	},
},

/*	MKILL
*/
{
	{
		"Ouch!",
		"Aaarrrgghhh!",
		"BOOM!",
		"Noooooo!",
		"Another one bites the dust.",
		"OH NOOOOOO!!!",
		"Dead meat.",
		"End of the line.",
		"KA-BOOOMMM!!",
	},
},

/*	MKEY
*/
{
	{
		"Took you long enough.",
		"Finally!",
		"I hope you don't take this long to get back.",
		"It's about time!",
		"Hurry up and get back!",
		"The clock's ticking...",
		"C'mon, slowpoke!",
		"tick...tick...tick...",
		"Your bonus is running out!",
		"Get the lead out!",
	},
	{
		"That was quick!",
		"Piece of cake!",
		"What a stroke of luck!",
		"How did you find it so quickly?",
		"Wow, that was fast!",
		"You didn't peek when I placed the key, did you?",
		"That was easy!",
		"No problem!",
		"Perfect!",
	},
	{
		"Your chances don't look good.",
		"Did you pass any fuel on the way?",
		"Can you make it?",
		"You really need some fuel.",
		"Getting back is going to be tough.",
		"I hope you didn't use all the fuel pods getting here.",
		"Don't waste so much fuel!",
		"You have to find some fuel pods!",
	},
	{
		"Now head back to the door!",
		"Got it!",
		"Alright!",
		"Yes!",
		"Go! Go! Go!",
		"Good job!",
		"Yeah!",
	},
},

/*	MCLOSE
*/
{
	{
		"Yow! Careful!",
		"Not so close!",
		"Watch out!",
		"Are you trying to get yourself killed?",
		"Are you crazy!",
		"I can't look!",
		"You're out of your mind!",
		"Look out!",
		"Yikes!",
		"Whoa!",
	},
},

/*	MSUICIDE
*/
{
	{
		"Don't you know suicide is illegal?",
		"Such a senseless waste of human life.",
		"AAAARRRRGGGHHHH.....",
		"Way to confront your problems!",
		"Remember kids, suicide isn't cool.",
		"That's no way to win.",
		"BOOOOMMMM!!!",
		"Surely there's an alternative?",
	},
},

/*	MEXIT
*/
{
	{
		"Pathetic.",
		"You need diapers.",
		"Slow and steady loses the bonus.",
		"Speedy Gonzalez you're not.",
		"Zero bonus. Is that you Qarl?",
		"Wake me after the next level.",
		"Stop it, you're boring me to death.",
		"What are you doing, drawing maps?",
		"Zzzzzzzzz....",
		"Wretched.",
	},
	{
		"What speed!",
		"I'm really impressed!",
		"Terrific!",
		"That was amazing!",
		"Incredible!",
		"Wow! I'll have to make the next maze harder!",
		"You've got this down pat!",
		"Fabulous!",
		"Excellent!",
	},
	{
		"You can do better than that!",
		"That's not too good.",
		"You need to go a little faster.",
		"I've seen better times.",
		"Pick up the pace, Ace.",
		"If you want a big bonus, you have to go faster.",
		"You're taking too long.",
		"Too slow.",
		"Hurry it up!",
	},
	{
		"Good job!",
		"Congratulations!",
		"Way to go!",
		"Good level!",
		"Good job!",
		"Alright!",
		"Great!",
		"Keep it up!",
		"Lookin' good!",
		"Yeah!",
	},
},

/*	MQUIT
*/
{
	{
		"Quitter!",
		"Don't stop now!",
		"Well, play again soon!",
		"Just one more game, please?",
		"Play again, I dare ya!",
		"Don't give up!",
		"It's been real.",
		"Oh, come on!",
		"Don't you want to finish the game?",
		"Goodbye!",
	},
},
};

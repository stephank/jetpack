/*	normal.c : routine that sets up the levels that aren't predefined
*/

#include "copyright.h"
#include "defs.h"

/* normal_level is a very ugly switch statement that sets up the various
	types of "normal" levels
*/
void	normal_level()
{
	register int	mx, my, f, p, s, n, md, mt, msc, fmin, fmax, smin, smax,
					g, gmin, gmax, kind, obj;

	/*	there are 5 kinds of "normal" levels: Open levels, Big levels,
		Twisty levels, and Death levels
	*/
	kind = level % 8;
	switch(kind) {
		/*	Twisty level: medium large maze, all walls, normal fuel and
			enemies, many intersections.
		*/
		case 0:
			message("Twisty Level", CGREEN, 75);
			msc = random() % 50 + 100;
			mx = random() % 15 + 5;
			my = random() % 15 + 5;
			md = 0;
			mt = 0;
			obj = (mx * my * msc) / 500;
			if(obj > 30) obj = 30;
			do {
				f = random() % 30;
				s = random() % 30;
				g = random() % 30;
			} while((f + s + g > obj) || (f + s + g < obj / 2));
			p = random() % (obj / 4) + obj / 4;
			smin = level / 3 + 1;
			smax = level / 3 + 3;
			if(smin > 5) smin = 5;
			fmin = level / 3 - 1;
			fmax = level / 3 + 3;
			if(fmin > 5) fmin = 5;
			gmin = level / 3 + 1;
			gmax = level / 3 + 3;
			if(gmin > 5) gmin = 5;
			make_random_level(mx, my, msc, md, mt, f, fmin, fmax, s, smin, smax,
								g, gmin, gmax, p);
			break;

		/*	Open level: medium size maze, very few walls, normal level of
			fuel and enemies
		*/
		case 2:
			message("Open Level", CORANGE, 75);
			msc = random() % 75 + 75;
			mx = random() % 20 + 5;
			my = random() % 20 + 5;
			md = random() % 30 + 71;
			mt = 100;
			obj = (mx * my * msc) / 500;
			if(obj > 30) obj = 30;
			do {
				f = random() % 30;
				s = random() % 30;
				g = random() % 30;
			} while((f + s + g > obj) || (f + s + g < obj / 2));
			p = random() % (obj / 4) + obj / 4;
			smin = level / 3 + 1;
			smax = level / 3 + 3;
			if(smin > 5) smin = 5;
			fmin = level / 3 - 1;
			fmax = level / 3 + 3;
			if(fmin > 5) fmin = 5;
			gmin = level / 3 + 1;
			gmax = level / 3 + 3;
			if(gmin > 5) gmin = 5;
			make_random_level(mx, my, msc, md, mt, f, fmin, fmax, s, smin, smax,
								g, gmin, gmax, p);
			break;
		/*	Big level: large maze, very few walls missing if any, normal
			level of fuel and enemies
		*/
		case 6:
			message("Big Level", CYELLOW, 75);
			msc = random() % 25 + 125;
			mx = random() % 10 + 15;
			my = random() % 10 + 15;
			if(random() % 3 == 0) md = random() % 5;
			else md = 0;
			mt = random() % 101;
			obj = (mx * my * msc) / 500;
			p = obj / 2;
			if(obj > 30) obj = 30;
			do {
				f = random() % 30;
				s = random() % 30;
				g = random() % 30;
			} while((f + s + g > obj) || (f + s + g < obj / 2));
			smin = level / 3 + 1;
			smax = level / 3 + 3;
			if(smin > 5) smin = 5;
			fmin = level / 3 - 1;
			fmax = level / 3 + 3;
			if(fmin > 5) fmin = 5;
			gmin = level / 3 + 1;
			gmax = level / 3 + 3;
			if(gmin > 5) gmin = 5;
			make_random_level(mx, my, msc, md, mt, f, fmin, fmax, s, smin, smax,
								g, gmin, gmax, p);
			break;
		/*	Death level: small maze, few walls missing, little fuel, two
			to three times normal level of enemies
		*/
		case 4:
			message("Death Level", CRED, 75);
			msc = random() % 20 + 100;
			mx = random() % 5 + 5;
			my = random() % 5 + 5;
			md = random() % 10;
			mt = random() % 101;
			obj = (mx * my * msc) / 500;
			do {
				f = random() % 30;
				s = random() % 30;
				g = random() % 30;
			} while((f + s + g > obj * 3) || (f + s + g < obj * 2));
			p = random() % (obj / 4) + obj / 3;
			smin = level / 3 + 1;
			smax = level / 3 + 3;
			if(smin > 5) smin = 5;
			fmin = level / 3 - 1;
			fmax = level / 3 + 3;
			if(fmin > 5) fmin = 5;
			gmin = level / 3 + 1;
			gmax = level / 3 + 3;
			if(gmin > 5) gmin = 5;
			make_random_level(mx, my, msc, md, mt, f, fmin, fmax, s, smin, smax,
								g, gmin, gmax, p);
			break;
		/*	Ordinary level: normal level of fuel and enemies, maze
			varies.
		*/
		default:
			msc = random() % 75 + 75;
			mx = random() % 15 + 5;
			my = random() % 15 + 5;
			n = random() % 10;
			if(n == 0) {
				md = random() % 90 + 11;
			} else if(n < 4) {
				md = random() % 10 + 1;
			} else md = 0;
			mt = random() % 101;
			obj = (mx * my * msc) / 500;
			if(obj > 30) obj = 30;
			do {
				f = random() % 30;
				s = random() % 30;
				g = random() % 30;
			} while((f + s + g > obj) || (f + s + g < obj / 2));
			p = random() % (obj / 4) + obj / 3;
			smin = level / 3 + 1;
			smax = level / 3 + 3;
			if(smin > 5) smin = 5;
			fmin = level / 3 - 1;
			fmax = level / 3 + 3;
			if(fmin > 5) fmin = 5;
			gmin = level / 3 + 1;
			gmax = level / 3 + 3;
			if(gmin > 5) gmin = 5;
			make_random_level(mx, my, msc, md, mt, f, fmin, fmax, s, smin, smax,
								g, gmin, gmax, p);
			break;
	}
}

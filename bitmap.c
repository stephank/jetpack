/*	bitmap.c : function to create bitmaps used in jetpack
*/

#include "copyright.h"
#include "defs.h"
#include "bitmap.h"

/*	all the pixmaps of various objects.
*/
Pixmap	playerpix[PLAYERPHASES], burnpix[BURNDIRECTIONS];
Pixmap	firepix[FIREPHASES], sweeperpix[SWEEPERPHASES],
		guardpix[GUARDDIRECTIONS][GUARDPHASES];
Pixmap	explodepix[EXPLODEFRAMES];
Pixmap	doorpix[DOORFRAMES];
Pixmap	fuelpix, keypix;
Pixmap	extramanpix;

/*	These arrays hold the color names for each part of each bitmap.
	They make the pixmap creation code more modifiable.
*/
int	playercolor[PLAYERPARTS] = { CBLUE, CGREY };
int	burncolor[BURNPARTS] = { CRED, CYELLOW };
int	firecolor[FIREPARTS] = { CRED, CYELLOW };
int	sweepcolor[SWEEPERPARTS] = { CGREEN, CGREY, CWHITE };
int	guardcolor[GUARDPARTS] = { CGREY, CYELLOW };
int	guardflamecolor[GUARDFLAMEPARTS] = { CRED, CYELLOW };
int	guardlightcolor = CRED;
int	explodecolor = CRED;
int	skelcolor = CWHITE;
int	fuelcolor[FUELPARTS] = { CORANGE, CRED, CGREY, CWHITE };
int	keycolor[KEYPARTS] = { CYELLOW };
int	doorcolor[DOORFRAMES][DOORPARTS] = {{ CWHITE }, { CGREY }};
int extramancolor = CBLUE;

set_up_bitmaps()
{
	/*	first-stage pixmaps -- each different colored part must be created
		from a mono bitmap, and then laid atop each other to make the
		final multi-color pixmap.
	*/
	Pixmap playerbit[PLAYERPHASES][PLAYERPARTS];
	Pixmap burnbit[BURNDIRECTIONS][BURNPARTS];
	Pixmap firebit[FIREPHASES][FIREPARTS];
	Pixmap sweeperbit[SWEEPERPHASES][SWEEPERPARTS];
	Pixmap guardbit[GUARDPARTS];
	Pixmap guardlightbit[GUARDLIGHTS];
	Pixmap guardflamebit[GUARDDIRECTIONS][GUARDFLAMEPARTS];
	Pixmap explodebit[EXPLODEFRAMES];
	Pixmap skelbit[EXPLODEFRAMES];
	Pixmap doorbit[DOORFRAMES][DOORPARTS];
	Pixmap fuelbit[FUELPARTS];
	Pixmap keybit[KEYPARTS];
	Pixmap extramanbit;

	int	i, j, k, g[GUARDLIGHTS];

	/*	Each of the following sections essentially does this:
			create and erase the main pixmap
			create all the secondary pixmaps from bitmap data
			copy the secondary pixmaps onto the main pixmap with the
				proper color
	*/

	/*	Player pixmaps
	*/
	for(i=0; i<PLAYERPHASES; i++) {
		playerpix[i] = XCreatePixmap(display, gamewindow, PLAYERWIDTH,
										PLAYERHEIGHT, depth);
		XFillRectangle(display, playerpix[i], ctable[CBLACK].smallgc,
						0, 0, PLAYERWIDTH, PLAYERHEIGHT);
		for(j=0; j<PLAYERPARTS; j++) {
			playerbit[i][j] = XCreateBitmapFromData(display, gamewindow,
													player_bits[i][j],
													PLAYERWIDTH,
													PLAYERHEIGHT);
			XCopyPlane(display, playerbit[i][j], playerpix[i],
						ctable[playercolor[j]].smallgc, 0, 0,
						PLAYERWIDTH, PLAYERHEIGHT, 0, 0, 1);
		}
	}

	/*	Pixmaps for the jetpack flame
	*/
	for(i=0; i<BURNDIRECTIONS; i++) {
		burnpix[i] = XCreatePixmap(display, gamewindow, BURNWIDTH,
									BURNHEIGHT, depth);
		XFillRectangle(display, burnpix[i], ctable[CBLACK].smallgc,
						0, 0, BURNWIDTH, BURNHEIGHT);
		for(j=0; j<BURNPARTS; j++) {
			burnbit[i][j] = XCreateBitmapFromData(display, gamewindow,
													burn_bits[i][j],
													BURNWIDTH,
													BURNHEIGHT);
			XCopyPlane(display, burnbit[i][j], burnpix[i],
						ctable[burncolor[j]].smallgc, 0, 0,
						BURNWIDTH, BURNHEIGHT, 0, 0, 1);
		}
	}

	/* Wall sweeper pixmaps (very pretty)
	*/
	for(i=0; i<SWEEPERPHASES; i++) {
		sweeperpix[i] = XCreatePixmap(display, gamewindow, SWEEPERWIDTH,
										SWEEPERHEIGHT, depth);
		XFillRectangle(display, sweeperpix[i], ctable[CBLACK].smallgc,
						0, 0, SWEEPERWIDTH, SWEEPERHEIGHT);
		for(j=0; j<SWEEPERPARTS; j++) {
			sweeperbit[i][j] = XCreateBitmapFromData(display, gamewindow,
														sweeper_bits[i][j],
														SWEEPERWIDTH, 
														SWEEPERHEIGHT);
			XCopyPlane(display, sweeperbit[i][j], sweeperpix[i],
						ctable[sweepcolor[j]].smallgc, 0, 0,
						SWEEPERWIDTH, SWEEPERHEIGHT, 0, 0, 1);
		}
	}

	/*	Fireball pixmaps
	*/
	for(i=0; i<FIREPHASES; i++) {
		firepix[i] = XCreatePixmap(display, gamewindow, FIREWIDTH,
									FIREHEIGHT, depth);
		XFillRectangle(display, firepix[i], ctable[CBLACK].smallgc,
						0, 0, FIREWIDTH, FIREHEIGHT);
		for(j=0; j<FIREPARTS; j++) {
			firebit[i][j] = XCreateBitmapFromData(display, gamewindow,
													fire_bits[i][j],
													FIREWIDTH,
													FIREHEIGHT);
			XCopyPlane(display, firebit[i][j], firepix[i],
						ctable[firecolor[j]].smallgc, 0, 0,
						FIREWIDTH, FIREHEIGHT, 0, 0, 1);
		}
	}

	/*	Guard pixmaps. These are a little odd because of the random
		blinking light sequences. There are three sets of lights on the
		guard, and each can be on or off. This makes 8 different frames,
		but to cut down on bitmap data, I only made the three light
		bitmaps rather than 8.
	*/
	for(i=0; i<GUARDPARTS; i++) {
		guardbit[i] = XCreateBitmapFromData(display, gamewindow,
											guard_bits[i], GUARDWIDTH,
											GUARDHEIGHT);
		for(j=0; j<GUARDDIRECTIONS; j++) {
			guardflamebit[j][i] = XCreateBitmapFromData(display, gamewindow,
													guard_flame_bits[j][i],
													GUARDWIDTH,
													GUARDHEIGHT);
		}
	}
	for(i=0; i<GUARDLIGHTS; i++) {
		guardlightbit[i] = XCreateBitmapFromData(display, gamewindow,
											guard_light_bits[i],
											GUARDWIDTH, GUARDHEIGHT);
	}
	for(i=0; i<GUARDDIRECTIONS; i++) {
		for(j=0; j<GUARDPHASES; j++) {
			guardpix[i][j] = XCreatePixmap(display, gamewindow, GUARDWIDTH,
											GUARDHEIGHT, depth);
			XFillRectangle(display, guardpix[i][j],
							ctable[CBLACK].smallgc, 0, 0,
							GUARDWIDTH, GUARDHEIGHT);
			for(k=0; k<GUARDPARTS; k++) {
				XCopyPlane(display, guardbit[k], guardpix[i][j],
							ctable[guardcolor[k]].smallgc,
							0, 0, GUARDWIDTH, GUARDHEIGHT, 0, 0, 1);
			}
			for(k=0; k<GUARDFLAMEPARTS; k++) {
				XCopyPlane(display, guardflamebit[i][k], guardpix[i][j],
							ctable[guardflamecolor[k]].smallgc,
							0, 0, GUARDWIDTH, GUARDHEIGHT, 0, 0, 1);
			}
			/*	This initialization will need to be expanded if I add more
				lights.
			*/
			g[2] = j / 4;
			g[1] = (j % 4) / 2;
			g[0] = (j % 4) % 2;
			for(k=0; k<GUARDLIGHTS; k++) {
				if(g[k]) XCopyPlane(display, guardlightbit[k], guardpix[i][j],
									ctable[guardlightcolor].smallgc,
									0, 0, GUARDWIDTH, GUARDHEIGHT, 0, 0, 1);
			}
		}
	}

	/*	Player explosion pixmaps
	*/
	for(i=0; i<EXPLODEFRAMES; i++) {
		explodepix[i] = XCreatePixmap(display, gamewindow, EXPLODEWIDTH,
										EXPLODEHEIGHT, depth);
		XFillRectangle(display, explodepix[i], ctable[CBLACK].smallgc,
						0, 0, EXPLODEWIDTH, EXPLODEHEIGHT);
		explodebit[i] = XCreateBitmapFromData(display, gamewindow,
												explode_bits[i],
												EXPLODEWIDTH,
												EXPLODEHEIGHT);
		skelbit[i] = XCreateBitmapFromData(display, gamewindow,
												skel_bits[i], SKELWIDTH,
												SKELHEIGHT);
		XCopyPlane(display, explodebit[i], explodepix[i],
					ctable[explodecolor].smallgc, 0, 0, EXPLODEWIDTH,
					EXPLODEHEIGHT, 0, 0, 1);
		XCopyPlane(display, skelbit[i], explodepix[i],
					ctable[skelcolor].smallgc, 0, 0, SKELWIDTH,
					SKELHEIGHT, (EXPLODEWIDTH - SKELWIDTH) / 2,
					(EXPLODEWIDTH - SKELWIDTH) / 2, 1);
	}

	/*	Fuel pod pixmap
	*/
	fuelpix = XCreatePixmap(display, gamewindow, FUELWIDTH, FUELHEIGHT,
							depth);
	XFillRectangle(display, fuelpix, ctable[CBLACK].smallgc, 0, 0,
					FUELWIDTH, FUELHEIGHT);
	for(i=0; i<FUELPARTS; i++) {
			fuelbit[i] = XCreateBitmapFromData(display, gamewindow,
												fuel_bits[i],
												FUELWIDTH, FUELHEIGHT);
			XCopyPlane(display, fuelbit[i], fuelpix,
						ctable[fuelcolor[i]].smallgc, 0, 0,
						FUELWIDTH, FUELHEIGHT, 0, 0, 1);
	}

	/*	Key pixmap
	*/
	keypix = XCreatePixmap(display, gamewindow, KEYWIDTH, KEYHEIGHT,
							depth);
	XFillRectangle(display, keypix, ctable[CBLACK].smallgc, 0, 0,
					KEYWIDTH, KEYHEIGHT);
	for(i=0; i<KEYPARTS; i++) {
			keybit[i] = XCreateBitmapFromData(display, gamewindow,
												key_bits[i], KEYWIDTH,
												KEYHEIGHT);
			XCopyPlane(display, keybit[i], keypix,
						ctable[keycolor[i]].smallgc, 0, 0,
						KEYWIDTH, KEYHEIGHT, 0, 0, 1);
	}

	/*	Door pixmaps. The only difference in frames is the color, but I
		wanted it this way in case I want to animate the door in the
		future.
	*/
	for(i=0; i<DOORFRAMES; i++) {
		doorpix[i] = XCreatePixmap(display, gamewindow, DOORWIDTH,
									DOORHEIGHT, depth);
		XFillRectangle(display, doorpix[i], ctable[CBLACK].smallgc,
						0, 0, DOORWIDTH, DOORHEIGHT);
		for(j=0; j<DOORPARTS; j++) {
			doorbit[i][j] = XCreateBitmapFromData(display, gamewindow,
													door_bits[i][j], 
													DOORWIDTH, DOORHEIGHT);
			XCopyPlane(display, doorbit[i][j], doorpix[i],
						ctable[doorcolor[i][j]].smallgc, 0, 0, 
						DOORWIDTH, DOORHEIGHT, 0, 0, 1);
		}
	}

	/*	Extra player pixmap
	*/
	extramanpix = XCreatePixmap(display, gamewindow, EXTRAMANWIDTH,
								EXTRAMANHEIGHT, depth);
	XFillRectangle(display, extramanpix, ctable[CBLACK].smallgc,
					0, 0, EXTRAMANWIDTH, EXTRAMANHEIGHT);
	extramanbit = XCreateBitmapFromData(display, gamewindow, extraman_bits,
										EXTRAMANWIDTH, EXTRAMANHEIGHT);
	XCopyPlane(display, extramanbit, extramanpix,
				ctable[extramancolor].smallgc,
				0, 0, EXTRAMANWIDTH, EXTRAMANHEIGHT, 0, 0, 1);

	/*	free all the temporary pixmaps
	*/
	for(i=0; i<PLAYERPHASES; i++) {
		for(j=0; j<PLAYERPARTS; j++) {
			XFreePixmap(display, playerbit[i][j]);
		}
	}
	for(i=0; i<BURNDIRECTIONS; i++) {
		for(j=0; j<BURNPARTS; j++) {
			XFreePixmap(display, burnbit[i][j]);
		}
	}
	for(i=0; i<FIREPHASES; i++) {
		for(j=0; j<FIREPARTS; j++) {
			XFreePixmap(display, firebit[i][j]);
		}
	}
	for(i=0; i<SWEEPERPHASES; i++) {
		for(j=0; j<SWEEPERPARTS; j++) {
			XFreePixmap(display, sweeperbit[i][j]);
		}
	}
	for(i=0; i<GUARDPARTS; i++) {
		XFreePixmap(display, guardbit[i]);
	}
	for(i=0; i<GUARDLIGHTS; i++) {
		XFreePixmap(display, guardlightbit[i]);
	}
	for(i=0; i<GUARDDIRECTIONS; i++) {
		for(j=0; j<GUARDFLAMEPARTS; j++) {
			XFreePixmap(display, guardflamebit[i][j]);
		}
	}
	for(i=0; i<EXPLODEFRAMES; i++) {
		XFreePixmap(display, explodebit[i]);
		XFreePixmap(display, skelbit[i]);
	}
	for(i=0; i<DOORFRAMES; i++) {
		for(j=0; j<DOORPARTS; j++) {
			XFreePixmap(display, doorbit[i][j]);
		}
	}
	for(i=0; i<FUELPARTS; i++) {
		XFreePixmap(display, fuelbit[i]);
	}
	for(i=0; i<KEYPARTS; i++) {
		XFreePixmap(display, keybit[i]);
	}
	XFreePixmap(display, extramanbit);
}

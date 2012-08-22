//#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
#include <ncursesw/curses.h>
#include "macros.h"

//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <locale.h>
//#include <time.h>

// View area
static WINDOW *life = NULL;
// Info window
static WINDOW *info = NULL;
// Marks window
static WINDOW *marks = NULL;


/* A helper function for creating new windows */
WINDOW *createwin(int height, int width, int begy, int begx);

int tik=0;
int cho=0;
int fskip=1, fskipc=0;

int wlines=200, wcols=200; 
int tout=200, dtout=50, touti=15000;

int vwposy=1, vwposx=1;
static int vlines=0, vcols=0;

int pause=1, age=0;

int lcell='#', dcell='.'; 

int *cells, *cells_buf;

void init_curses();

void init_world();
void finish_game();

void clear_cworld();

void info_status();

bool kbd();

void evolv();
void u_pop();

void draw_vmap();
void draw_mmap();
void draw_popmap();



main()
{
	
	int k;
	init_world();
	
	init_curses();

	draw_vmap();

//	fskipc=0;

	do
	{ //Keyboard loop
		if(pause==0)
		{	

			evolv();

			//k=fskipc/fskip;

			if ((fskipc%fskip)==0)	
			{
				//fskipc=0;
				info_status();
				draw_vmap();
			}

			fskipc++;
			age++;

		}


		cho=getch();
	}
	while(kbd()); 


	finish_game();

	return(0);
}




void init_curses()
{

	initscr();
	noecho();
	timeout(touti);
	//raw();
	
	
	/* Refresh standart screen */
	refresh();

	// Creating windows
	life = createwin(MLMAX, MCMAX, MLMIN, MCMIN);
	info = createwin(PLINES, PCOLS, 0, COLS-PCOLS);

	box(info, 0, 0); // default box around info-window

	box(life, 0, 0); // default box around life-window

	
	/* Move cursor to info-window at 1,1 position and 
	 * write header. */
	mvwprintw(info, 1, 1, "INFO");

		/* Move cursor back to view area */
	wmove(life, 2,2);

	// refresh windows
	wrefresh(info);
	wrefresh(life);

	info_status(0);

}

void finish_game()
{
	free (cells);
	free (cells_buf);

	delwin(life);
	delwin(info);
	delwin(marks);
	endwin();

}


void clear_cworld()
{

	memset (cells, 0, SIZEWI);


}


WINDOW *createwin(int height, int width, int begy, int begx)
{
    /* Hold the window */
    WINDOW *local_win = NULL;

    /* Create the window */
    local_win = newwin(height, width, begy, begx);
    if(local_win == NULL) /* Check that it succeeded */
        exit(EXIT_FAILURE);

    wmove(local_win, 1, 1); /* Looks better if the cursor is not on
                               top of lines */

    wrefresh(local_win); /* And refresh */

    return local_win;
}

void evolv()
{

	int i,j,pop;
	
	// copy data to buffer
	memcpy (cells_buf, cells, SIZEWI);


	for (i=1; i<=wlines; i++)
		for (j=1; j<=wcols; j++)
		{
			pop=0;

			// NW 
			if (CBDATA((i-1),(j-1))==1)
				pop++;
			// N
			if (CBDATA((i-1),j)==1)
				pop++;
			// NE
			if (CBDATA((i-1),j+1)==1)
				pop++;
			// E
			if (CBDATA(i,j+1)==1)
				pop++;
			// SE
			if (CBDATA((i+1),j+1)==1)
				pop++;
			// S
			if (CBDATA((i+1),j)==1)
				pop++;
			// SW
			if (CBDATA((i+1),j-1)==1)
				pop++;
			// W
			if (CBDATA(i,j-1)==1)
				pop++;


			switch (pop)
			{
			case 0: case 1: case 4: case 5:
			case 6: case 7: case 8:
				CDATA(i,j)=0;
				break;
			case 3:
				CDATA(i,j)=1;
				break;
			}
		}

	


}

void draw_vmap()
{

	int i,j,x,y,k,t;

	getyx(life, y, x);

	box(life, 0, 0); // default box around marks-window

	for(i=VLMIN;i<=VLMAX;i++)
		for (j=VCMIN;j<=VCMAX;j++)
			mvwprintw(life, i, j, "%c", (char) (VCDATA(i,j)==0) ? dcell : lcell );

	k=10-(vwposx-1)%10;

	for (j=k; j<MCMAX-3; j=j+10)
		mvwprintw(life, MCMIN, j, "%i", vwposx+j-1);

	for (j=k; j<MCMAX-3; j=j+10)
		mvwprintw(life, MLMAX-1, j, "%i", vwposx+j-1);

	k=10-(vwposy-1)%10;

	
	/* print mark as 1 digit of decades */
	for (i=k; i<MLMAX-1; i+=10)
		mvwprintw(life, i, MCMAX-1, "%i", ((vwposy+i-1)%100)/10);

	/* print mark as 1 digit of decades */
	for (i=k; i<MLMAX-1; i+=10)
		mvwprintw(life, i, MCMIN, "%i", ((vwposy+i-1)%100)/10);

	wmove(life, y, x);

	wrefresh(life);



}

void info_status()
{
	int y,x,i,j;
	getyx(life, y, x);

	

	mvwprintw(info, 2, 1, " Columns: %i", COLS);
	mvwprintw(info, 3, 1, " Lines: %i", LINES);

	//mvwprintw(info, 5, 1, "                   ");

	i=5;

	mvwprintw(info, i++, 1, " Key pressed: %.3c", cho);
	mvwprintw(info, i++, 1, " tik: %i", tik++);
	mvwprintw(info, i++, 1, " age: %i", age);
	mvwprintw(info, i++, 1, " pause: %i", pause);
	mvwprintw(info, i++, 1, " timeout: %i", tout);

	mvwprintw(info, i++, 1, " Key pressed: %i  ", cho);
	mvwprintw(info, i++, 1, " vwposy: %.3i", vwposy);
	mvwprintw(info, i++, 1, " vwposx: %.3i", vwposx);
	mvwprintw(info, i++, 1, " w size: %i x %i", wlines, wcols);
	mvwprintw(info, i++, 1, " fskip: %i  ", fskip);

	wmove(life, y, x);
	wrefresh(info);
	wrefresh(life);
	//refresh();

}


bool kbd()
{
	int y, x;
	int map;
	getyx(life, y, x);

	//timeout(touti);

	switch(cho)
	{
	case -1:
		cho='0';
		return true;
		break;

	case ' ':	/* Activate / Deactivate */

		VCDATA(y,x)=( (VCDATA(y,x) == 0) ? 1 : 0);
		break;

	case 65:	//move up 65=KEY_UP	

		y--;
		y=( (y<VLMIN) ? VLMAX : y);
		break;

	case 66:	//move down 66=KEY_DOWN

		y++;
		y=( (y>VLMAX) ? y-VLMAX : y);
		break;

	case 68:	//move left	

		x--;
		x=( (x<VCMIN) ? VCMAX : x);
		break;

	case 67:	//move right	
		x++;
		x=( (x>VCMAX) ? x-VCMAX : x);
		break;

	case 'p':
		pause=((pause==0) ? 1 : 0);
		timeout((pause==0) ? tout : touti);	
		break;

	case '+':

		if (tout-dtout>=0 && pause == 0)
		{
			tout=tout-dtout;
			timeout(tout);
		}
		break;

	case '-':

		if (tout+dtout<=1500 & pause == 0)
		{
			tout=tout+dtout;
			timeout(tout);

		}
		break;

	case 'w':	//move view up 	

		vwposy-=5;
		vwposy=( (vwposy<0) ? 0 : vwposy);
		break;

	case 's':	//move view down 

		vwposy+=5;
		vwposy=( (vwposy>WLINES-VLMAX-1) ? WLINES-VLMAX-1 : vwposy);
		break;

	case 'a':	//move view left	

		vwposx-=5;
		vwposx=( (vwposx<0) ? 0 : vwposx);
		break;

	case 'd':	//move view right	

		vwposx+=5;
		vwposx=( (vwposx>WCOLS-VCMAX-1) ? WCOLS-VCMAX-1 : vwposx);
		break;

	case 'q':

		return false;
		break;

	// frame skips
	case '*':
		fskip++;
		break;

	case '/':
		fskip--;
		fskip=((fskip<1) ? 1 : fskip );
		break;

	case 126: // "del" button

		clear_cworld();
		break;

	}

	wmove(life, y, x);

	info_status();
	draw_vmap();

	//wmove(life, y, x);

	return true;

}


void init_world()
{
	int i=0, x, y;

	// alloc memory for cells and init in with 0
	cells = calloc(WSIZE, sizeof(int));

	// alloc mem for buffer without initialization
	cells_buf = malloc(SIZEWI);

}





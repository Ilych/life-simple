#ifndef LMACRO_H



# define VLMAX (LINES-4)	// max lines of view
# define VCMAX (COLS-PCOLS-3)	// max columns of view
# define VLMIN 1
# define VCMIN 1

# define MLMAX (VLMAX+2)	// max lines of marks-window
# define MCMAX (VCMAX+2)	// max cols of marks-window
# define MLMIN	(VLMIN-1)
# define MCMIN	(VCMIN-1)

# define WLINES (wlines+2)	// world lines with borders +1 each side 
# define WCOLS (wcols+2)	// world columns w borders +1 each side



#define PCOLS 25	// Panel width
#define PLINES LINES	// Panel height

//# define VLINES (LINES)		// View lines
//# define VCOLS	(COLS-PCOLS-1)	// View columns

# define VSIZE (VLMAX*VCMAX)		// Amount of view area
# define SIZEV (VSIZE*sizeof(int))	// Real size of view in memory
# define WSIZE ((wlines+2)*(wcols+2))		/* Amount of world area
						   +2 = 2*1 - border */  
# define SIZEW (WSIZE*sizeof(struct cellstr))	// Amount of memory 
# define SIZEWI (WSIZE*sizeof(int))	// Amount of memory 
						// of world area

# define COORD(y, x) ((y)*(wcols+2)+(x))	// Convert 2d data to 1d
# define DATA(y, x) (cell [ COORD( (y), (x) ) ]) // get data from cell
# define CDATA(y, x) (cells [ COORD( (y), (x) ) ]) // get data from cell
# define CBDATA(y, x) (cells_buf [ COORD( (y), (x) ) ]) // get data from cell
# define VDATA(y,x) (cell[COORD( (y+vwposy), (x+vwposx) )])
# define VCDATA(y,x) (cells[COORD( (y+vwposy), (x+vwposx) )])
//# define CELL(y, x) (cell [ COORD( (y), (x) ) ] ) // get 



#endif


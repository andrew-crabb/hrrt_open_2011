/* Authors: Inki Hong, Dsaint31, Merence Sibomana (MS)
  Creation 08/2004
  Modification history: Merence Sibomana
	10-DEC-2007: Modifications for compatibility with windelays.
  24-MAR-2009: changed filenames extensions to .cpp
  11-MAY-2009: Add span and max ring difference parameters
  02-JUL-2009: Add Transmission(TX) LUT
  2/14/13 ahc: Made Rebinner LUT file a required argument.
*/
#ifndef gen_delays_h
#define gen_delays_h
// #include <stdio.h>
#include <cstdio>
int gen_delays(int argc, char **argv,int is_inline, float scan_duration,
			   float ***result,FILE *p_coins_file,  char *p_delays_file, 
               int span=9, int max_rd=67,
               // ahc
               char *p_rebinner_lut_file = NULL
               );
const char *hrrt_rebinner_lut_path(int tx_flag=0);

static int nmpairs=20;
static int hrrt_mpairs[][2]={{-1,-1},{0,2},{0,3},{0,4},{0,5},{0,6},
                                     {1,3},{1,4},{1,5},{1,6},{1,7},
                                           {2,4},{2,5},{2,6},{2,7},
                                                 {3,5},{3,6},{3,7},
                                                       {4,6},{4,7},
                                                             {5,7}}; 
#endif

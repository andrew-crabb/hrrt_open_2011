/* Authors: Larry Byars, Inki Hong, Dsaint31, Merence Sibomana
  Creation 08/2004
  Modification history: Merence Sibomana
	10-DEC-2007: Modifications for compatibility with windelays.
  11-Nov-2008: Added LUT for 32bit and 64bit compatibility 
  17-Mar-2009: Modified init_sol_lut to load only sinogram map
               if seginfo not provided (segzoffset==NULL)
  07-Apr-2009: Changed filenames from .c to .cpp and removed debugging printf 
  30-Apr-2009: Integrate Peter Bloomfield __linux__ support
  02-JUL-2009: Add Transmission(TX) LUT
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "segment_info.h"
#include "geometry_info.h"
#include "lor_sinogram_map.h"


//lor_sinogram_map.h
float *m_c_zpos=NULL;
float *m_c_zpos2=NULL;
short **m_segplane=NULL;
SOL ***m_solution=NULL;
SOL ***m_solution_tx[2];
static int nmpairs=20;
static int mpairs[][2]={{-1,-1},{0,2},{0,3},{0,4},{0,5},{0,6},
						     {1,3},{1,4},{1,5},{1,6},{1,7},
                             {2,4},{2,5},{2,6},{2,7},
                             {3,5},{3,6},{3,7},
                             {4,6},{4,7},
                             {5,7}}; 

//  This procedure converts the line between 2 detector physical
//  coordinates (deta and detb are 3 value arrays, x, y, z in cm)
//  and LOR projection coordinates (4 value array with values of
//  r, phi, z, tan_theta in appropriate units (cm, radians, cm, value).
/*
function phy_to_pro, adet, bdet
	ax=adet[0]
	bx=bdet[0]
	ay=adet[1]
	by=bdet[1]
	az=adet[2]
	bz=bdet[2]
	dz=az-bz
	dx=ax-bx
	dy=ay-by
	d=sqrt(dx*dx+dy*dy)
	r=(ay*bx-ax*by)/d
	if ((dx < 0.0) && (dy < 0.0)) { dx *= -1.0; dy *= -1.};
	phi=atan(dx,dy)/!dtor
	tan_theta=dz/d
	z=az-(ax*dx+ay*dy)*dz/(d*d)
    if (phi < 0.0) then begin
        phi = phi + 180.
        r = -r
        tan_theta=-tan_theta
    endif
	return,[r,phi,z,tan_theta]
end
*/
void init_phy_to_pro( float deta[3], float detb[3],SOL *sol)
{
  double dx, dy, d;
  float r,phi, z;
  float pi=(float)M_PI;
  int bin, view;
  int addr=-1;
  
  dy = deta[1]-detb[1];
  dx = deta[0]-detb[0];
	d = sqrt( dx*dx + dy*dy);
  r = (float)((deta[1]*detb[0]-deta[0]*detb[1])/d); //Xr
	phi = (float)(atan2( dx, dy));                    //view

  if (phi < 0.0) {
    phi += pi;
    r =-r;
    d = -d;
  }
	if (phi>= M_PI){//M_PI){//pi) {
    phi=0.0;
		r = -r; //(float)(-dy);
    d = -d; //(float)(-dy);
  }

  view = (int)(m_nviews*phi/M_PI);
	if(view>=288) view=0;
	if(view<=-1){
		view=0;
		printf("error \t%e\n",phi);
	}
	z = (float)((deta[0]*dx+deta[1]*dy)/(d*d));  //????
//    view = nviews*phi/M_PI;
	bin = (int)(m_nprojs/2+(r/m_binsize+0.5));
	if ((bin <0) || (bin > m_nprojs-1)){
		sol->nsino=-1;
	} else 	sol->nsino=bin+view*m_nprojs;
	sol->d=(float)(1.0/d/m_d_tan_theta);
	sol->z=(float)(z/m_plane_sep);
}


void init_sol(int *segzoffset)
{
	int i,ax,bx,al,bl,plane;
	int axx,bxx;
	int ahead;
	int bhead;
  float deta_pos[3], detb_pos[3];

	m_c_zpos   = (float *)  calloc(NYCRYS,sizeof(float ));
	m_c_zpos2  = (float *)  calloc(NYCRYS,sizeof(float ));
	m_segplane = (short **) calloc(63,sizeof(short *));

	for(i=0;i<63;i++) {
		m_segplane[i]=(short *) calloc(NYCRYS*2-1,sizeof(short));
	}
  
  // Initialize and populate solution if NULL
  if (m_solution == NULL)
  {
	  //m_solution = phi(=view angle)
	  m_solution=(SOL ***) calloc(21,sizeof(SOL **));
	  for(i=1;i<=20;i++){
  /*		5, 10
		  4  9  14
		  8  13 17
		  7  12 16 19
		  11 15 18
  */
		  m_solution[i]=(SOL**) calloc(NXCRYS*NDOIS,sizeof(SOL *));
		  for(ax=0;ax<NXCRYS*NDOIS;ax++){
			  m_solution[i][ax]=(SOL *) calloc(NXCRYS*NDOIS,sizeof(SOL));
		  }
	  }
  /*	solution[10]=solution[5];
	  solution[9 ]=solution[4];
	  solution[14]=solution[4];
	  solution[13]=solution[8];
	  solution[17]=solution[8];
	  solution[12]=solution[7];
	  solution[16]=solution[7];
	  solution[19]=solution[7];
	  solution[15]=solution[11];
	  solution[18]=solution[11];
  */	
	  for(i=1;i<=20;i++){
  //		if(i==10 || i==9 || i==14 || i==13 || i==17 || i==12 || i==16 || i==19 || i==10 || i==15 || i==18) continue;
	      ahead = mpairs[i][0];
		  bhead = mpairs[i][1];
		  for(al=0;al<NDOIS;al++){ //layer
			  for(ax=0;ax<NXCRYS;ax++){ //x축
				  axx=ax+NXCRYS*al;       //x축+레이어
				  det_to_phy( ahead, al, ax, 0, deta_pos);
				  for(bl=0;bl<NDOIS;bl++){
					  for(bx=0;bx<NXCRYS;bx++){
						  bxx=bx+NXCRYS*bl;
						  det_to_phy( bhead, bl, bx, 0, detb_pos);
						  init_phy_to_pro(deta_pos,detb_pos,&m_solution[i][axx][bxx]);
					  }
				  }
			  }
		  }
	  }
  }
	printf("\n");
	for(i=0;i<NYCRYS;i++){
		det_to_phy( 0,0, 0, i, deta_pos);
		m_c_zpos[i] =deta_pos[2];
		m_c_zpos2[i]=(float)(deta_pos[2]/m_plane_sep+0.5);
	}
	
	for(i=0;i<63;i++){
		for(plane=0;plane<NYCRYS*2-1;plane++){
			if (i>m_nsegs-1){
				m_segplane[i][plane]=-1;
				continue;
			}
			if (plane < m_segz0[i]) m_segplane[i][plane]=-1;
		    if (plane > m_segzmax[i]) m_segplane[i][plane]=-1;
			if(m_segplane[i][plane]!=-1) m_segplane[i][plane]= plane+segzoffset[i];
		}
	}
}


int init_lut_sol(const char* lut_filename, int *segzoffset)
{
	int i,ax,plane;
  FILE *fp=NULL;

  if (m_solution != NULL) return 1;

  // check file opening 
  if ((fp=fopen(lut_filename, "rb")) == NULL) return 0;

  // Initialize and populate solution if NULL
  if (m_solution == NULL)
  {
	  //m_solution = phi(=view angle)
	  m_solution=(SOL ***) calloc(21,sizeof(SOL **));
    for(i=1;i<=20;i++){
 		  m_solution[i]=(SOL**) calloc(NXCRYS*NDOIS,sizeof(SOL *));
		  for(ax=0;ax<NXCRYS*NDOIS;ax++){
			  m_solution[i][ax]=(SOL *) calloc(NXCRYS*NDOIS,sizeof(SOL));
        if (fread(m_solution[i][ax],sizeof(SOL),NXCRYS*NDOIS, fp) != NXCRYS*NDOIS)
        {
          fclose(fp);
          return 0;
        }
		  }
    }
  }
  
  if (segzoffset == NULL)
  {
    fclose(fp);
    return 1;
  }

	printf("\n");
	m_c_zpos   = (float *)  calloc(NYCRYS,sizeof(float ));
	m_c_zpos2  = (float *)  calloc(NYCRYS,sizeof(float ));
	m_segplane = (short **) calloc(63,sizeof(short *));

	for(i=0;i<63;i++) {
		m_segplane[i]=(short *) calloc(NYCRYS*2-1,sizeof(short));
	}
  
  if (fread(m_c_zpos,sizeof(float),NYCRYS, fp) != NYCRYS)
  {
    fclose(fp);
    return 0;
  }
  if (fread(m_c_zpos2,sizeof(float),NYCRYS, fp) != NYCRYS)
  {
    fclose(fp);
    return 0;
  }
  if (fp != NULL) fclose(fp);
	
	for(i=0;i<63;i++){
		for(plane=0;plane<NYCRYS*2-1;plane++){
			if (i>m_nsegs-1){
				m_segplane[i][plane]=-1;
				continue;
			}
			if (plane < m_segz0[i]) m_segplane[i][plane]=-1;
		    if (plane > m_segzmax[i]) m_segplane[i][plane]=-1;
			if(m_segplane[i][plane]!=-1) m_segplane[i][plane]= plane+segzoffset[i];
		}
	}
  return 1;
}

int save_lut_sol(const char* lut_filename)
{
  int i, ax;
  FILE *fp=NULL;
  // check file opening 
  if (m_solution != NULL)
  {
    if ((fp=fopen(lut_filename, "wb")) == NULL) return 0;
    for(i=1;i<=20;i++)
    {
      for(ax=0;ax<NXCRYS*NDOIS;ax++)
      {
        if (fwrite(m_solution[i][ax],sizeof(SOL),NXCRYS*NDOIS, fp) != NXCRYS*NDOIS)
        {
          fclose(fp);
          return 0;
        }
      }
    }
    if (fwrite(m_c_zpos,sizeof(float),NYCRYS, fp) != NYCRYS)
    {
      fclose(fp);
      return 0;
    }
    if (fwrite(m_c_zpos2,sizeof(float),NYCRYS, fp) != NYCRYS)
    {
      fclose(fp);
      return 0;
    }
    fclose(fp);
  }
  return 1;
}

void init_sol_tx(int tx_span)
{
	int i,ax,bx,al,bl;
	int axx,bxx;
	int ahead;
	int bhead;
  float deta_pos[3], detb_pos[3];

/*
	m_c_zpos   = (float *)  calloc(NYCRYS,sizeof(float ));
	m_c_zpos2  = (float *)  calloc(NYCRYS,sizeof(float ));
	m_segplane = NULL;
*/

  m_d_tan_theta = (float)(tx_span*m_plane_sep/m_crystal_radius);
  // Initialize and populate solution if NULL
  // TX m_sol
  //m_solution = phi(=view angle)
   m_solution_tx[0]=(SOL ***) calloc(nmpairs+1,sizeof(SOL **));
   m_solution_tx[1]=(SOL ***) calloc(nmpairs+1,sizeof(SOL **));
	 for(i=1;i<=nmpairs;i++){
  /*		5, 10
		  4  9  14
		  8  13 17
		  7  12 16 19
		  11 15 18
  */
	   m_solution_tx[0][i]=(SOL**) calloc(NXCRYS*NDOIS,sizeof(SOL *));
	    m_solution_tx[1][i]=(SOL**) calloc(NXCRYS*NDOIS,sizeof(SOL *));
	    for(ax=0;ax<NXCRYS*NDOIS;ax++){
        m_solution_tx[0][i][ax]=(SOL *) calloc(NXCRYS,sizeof(SOL));
		    m_solution_tx[1][i][ax]=(SOL *) calloc(NXCRYS,sizeof(SOL));
      }
    }
    // Table 0,  B is  TX source
    for(i=1;i<=nmpairs;i++){
      ahead = mpairs[i][0];
		  bhead = mpairs[i][1];
		  for(al=0;al<NDOIS;al++){ //layer
			  for(ax=0;ax<NXCRYS;ax++){ //x축
				  axx=ax+NXCRYS*al;       //x축+레이어
				  det_to_phy( ahead, al, ax, 0, deta_pos);
					  for(bx=0;bx<NXCRYS;bx++){
						  det_to_phy( bhead, 7, bx, 0, detb_pos);
						  init_phy_to_pro(deta_pos,detb_pos,&m_solution_tx[0][i][axx][bx]);
					  }
				  }
			  }
    }

     // Table 1,  A is  TX source
    for(i=1;i<=nmpairs;i++){
      ahead = mpairs[i][0];
		  bhead = mpairs[i][1];
      for(ax=0;ax<NXCRYS;ax++){ //x축
				  det_to_phy( ahead, 7, ax, 0, deta_pos);
          for(bl=0;bl<NDOIS;bl++){ //layer
					  for(bx=0;bx<NXCRYS;bx++){
						  bxx=bx+NXCRYS*bl;
						  det_to_phy( bhead, bl, bx, 0, detb_pos);
						  init_phy_to_pro(deta_pos,detb_pos,&m_solution_tx[1][i][bxx][ax]);
					  }
				  }
			  }
    }
/*
	printf("\n");
	for(i=0;i<NYCRYS;i++){
		det_to_phy( 0,0, 0, i, deta_pos);
		m_c_zpos[i] =deta_pos[2];
		m_c_zpos2[i]=(float)(deta_pos[2]/m_plane_sep+0.5);
	}
*/
}

int save_lut_sol_tx(const char* lut_filename)
{
  int i, ax;
  FILE *fp=NULL;
  // check file opening 
  if ((fp=fopen(lut_filename, "wb")) == NULL) return 0;
  for(i=1;i<=nmpairs;i++)
    {
      for(ax=0;ax<NXCRYS*NDOIS;ax++)
      {
        if (fwrite(m_solution_tx[0][i][ax],sizeof(SOL),NXCRYS, fp) != NXCRYS ||
            fwrite(m_solution_tx[1][i][ax],sizeof(SOL),NXCRYS, fp) != NXCRYS)
        {
          fclose(fp);
          return 0;
        }
      }
    }
/*
  if (fwrite(m_c_zpos,sizeof(float),NYCRYS, fp) != NYCRYS)
    {
      fclose(fp);
      return 0;
    }
  if (fwrite(m_c_zpos2,sizeof(float),NYCRYS, fp) != NYCRYS)
    {
      fclose(fp);
      return 0;
    }
*/
  fclose(fp);
  return 1;
}

int init_lut_sol_tx(const char* lut_filename)
{
	int i,ax;
  FILE *fp=NULL;

  if (m_solution != NULL) return 1;

  // check file opening 
  if ((fp=fopen(lut_filename, "rb")) == NULL) return 0;

  // Initialize and populate solution if NULL
  if (m_solution_tx[0] == NULL)
  {
	  //m_solution = phi(=view angle)
    m_solution_tx[0]=(SOL ***) calloc(21,sizeof(SOL **));
	  m_solution_tx[1]=(SOL ***) calloc(21,sizeof(SOL **));
    for(i=1;i<=20;i++){
      m_solution_tx[0][i]=(SOL**) calloc(NXCRYS*NDOIS,sizeof(SOL *));
 		  m_solution_tx[1][i]=(SOL**) calloc(NXCRYS*NDOIS,sizeof(SOL *));
		  for(ax=0;ax<NXCRYS*NDOIS;ax++){
			  m_solution_tx[0][i][ax]=(SOL *) calloc(NXCRYS,sizeof(SOL));
			  m_solution_tx[1][i][ax]=(SOL *) calloc(NXCRYS,sizeof(SOL));
        if (fread(m_solution_tx[0][i][ax],sizeof(SOL),NXCRYS, fp) != NXCRYS ||
          fread(m_solution_tx[1][i][ax],sizeof(SOL),NXCRYS, fp) != NXCRYS)
        {
          fclose(fp);
          return 0;
        }
		  }
    }
  }

/*
	printf("\n");
	m_c_zpos   = (float *)  calloc(NYCRYS,sizeof(float ));
	m_c_zpos2  = (float *)  calloc(NYCRYS,sizeof(float ));
	m_segplane = (short **) calloc(63,sizeof(short *));

	for(i=0;i<63;i++) {
		m_segplane[i]=(short *) calloc(NYCRYS*2-1,sizeof(short));
	}
  
  if (fread(m_c_zpos,sizeof(float),NYCRYS, fp) != NYCRYS)
  {
    fclose(fp);
    return 0;
  }
  if (fread(m_c_zpos2,sizeof(float),NYCRYS, fp) != NYCRYS)
  {
    fclose(fp);
    return 0;
  }
*/

  fclose(fp);
  return 1;
}

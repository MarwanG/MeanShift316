#include <stdlib.h>
#include <stdio.h>
#include <inrimage/image.h>
#include <math.h>




//
// calcul.h
//

struct pt_x{
	float i;
	float j;
	float r;
	float g;
	float b;
};

typedef struct pt_x pt_x;

// noyau d'Epanechikov
// returns k(x)
int noyau_Epan(float x);

/*
Noyau pour x qui contient le cord.
i et j le cord de un pofloat pass
hs le rayon
if ||x||2 < h2 retour 1 sinon retour 0
*/
int noyau_coord(float i , float j ,float hs);

int noyau_rgb(float r , float g , float b , float hr);


int noyau_global(float i,float j , float r , float g , float b , float hs , float hr);


float noyau_gaussCoord(float i , float j , float hs);

float noyau_gaussRgb(float r , float g , float b , float hr);

/* puts the difference between pixel ptX2 and ptX1 into pixel pt */

void sub_pt(struct pt_x ptX1 , struct pt_x* ptX2 , struct pt_x* pt);


/* puts the dot product pixel ptX1 and i into pixel pt */

void mult_pt(struct pt_x ptX1 , float i , struct pt_x *pt);

/* divides pixel ptX1 by i value in pixel pt */

void div_pt(struct pt_x ptX1 , float i , struct pt_x *pt);


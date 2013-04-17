
#include "calcul.h"
#include <stdlib.h>
#include <stdio.h>
#include <inrimage/image.h>
#include <math.h>
#include "tools.h"



/* ----NOYAU D'EPANECHIKOV---- */

int noyau_Epan(float x){
	if(x >= 0 && x<=1){
		return 1 - x;
	}else{
		return 0;
	}
}


int noyau_coord (float i,float j,float hs)
{

	float iCarre , jCarre ;
	float hCarre;
	double norm;
	iCarre = i * i;
	jCarre = j * j;
	norm  = sqrt(iCarre + jCarre);
	norm = norm * norm ;
	hCarre = hs * hs;

	if (norm < hCarre){
		return 1;
	}else{

		return 0;
	}

}


int noyau_rgb (float r,float g,float b,float hr)
{

	float rCarre , gCarre , bCarre ;
	float hCarre;
	double norm;
	rCarre = r * r;
	gCarre = g * g;
	bCarre = b * b;
	norm  = sqrt(rCarre + gCarre + bCarre);
	norm = norm * norm ;
	hCarre = hr * hr;

	if( norm < hCarre){
		return 1;
	}else{
		return 0;
	}

}

int noyau_global(float i,float j , float r , float g , float b , float hs , float hr){
	float coord = noyau_coord(i,j,hs);
	float rgb = noyau_rgb(r,g,b,hr);
	float res = -coord * rgb;
	return res;
}

/* NOYAU GAUSSIEN */

float noyau_gaussCoord(float i , float j , float hs){
	float iCarre , jCarre ;
	float hCarre;
	float res;
	double norm;
	iCarre = i * i;
	jCarre = j * j;
	norm  = sqrt(iCarre + jCarre);
	hCarre= pow(hs,2);
	res = -0.5 * (norm/hCarre);
	//fprintf(stderr, "before exp %f\n",res);
	res = exp(res);
	//fprintf(stderr, "after exp %f\n",res);
	res = res * -0.5 * hs;
	//fprintf(stderr, "after 0.5 hs %f\n", res);
	return res;
}

float noyau_gaussRgb(float r , float g , float b , float hr){
	float rCarre , gCarre , bCarre;
	float hCarre;
	float res;
	double norm;
	rCarre = pow(r,2);
	gCarre = pow(g,2);
	bCarre = pow(b,2);
	norm  = sqrt(rCarre + bCarre + gCarre);
	hCarre = pow(hr,2);
	res = -0.5 * (norm/hCarre);
	//fprintf(stderr, "before exp %f\n",res);
	res = exp(res);
	//fprintf(stderr, "after exp %f\n",res);
	res = res * -0.5 * hr;
	//fprintf(stderr, "after 0.5 hs %f\n", res);
	return res;
}

/* FONCTIONS FOR CALCULATIONS */


int moduloPt(struct pt_x ptX1 , struct pt_x ptX2){
	int res = sqrt(pow(ptX2.i - ptX1.i,2)+pow(ptX2.j - ptX1.j,2)+pow(ptX2.r - ptX1.r,2)+pow(ptX2.g - ptX1.g,2) + pow(ptX2.b - ptX1.b,2));
 	return res;
}


void sum_pt(struct pt_x ptX1 , struct pt_x *ptX2 , struct pt_x*  pt){
	pt->i = ptX1.i + ptX2->i;
	pt->j = ptX1.j + ptX2->j;
	pt->r = ptX1.r + ptX2->r;
	pt->g = ptX1.g + ptX2->g;
	pt->b = ptX1.b + ptX2->b;
}

void sub_pt(struct pt_x ptX1 , struct pt_x* ptX2 , struct pt_x* pt){
	
	pt->i = ptX1.i - ptX2->i;
	pt->j = ptX1.j - ptX2->j;
	pt->r = ptX1.r - ptX2->r;
	pt->g = ptX1.g - ptX2->g;
	pt->b = ptX1.b - ptX2->b;
}


void mult_pt(struct pt_x ptX1 , float i , struct pt_x *pt){

	pt->i = ptX1.i * i;
	pt->j = ptX1.j * i;
	pt->r = ptX1.r * i;
	pt->g = ptX1.g * i;
	pt->b = ptX1.b * i;

}



void div_pt(struct pt_x ptX1 , float div , struct pt_x *pt){

	pt->i = ptX1.i / div;
	pt->j = ptX1.j / div;
	pt->r = ptX1.r / div;
	pt->g = ptX1.g / div;
	pt->b = ptX1.b / div;

}

int distanceEuc(int centrei , int centrej,pt_x ptX2){
	float x = pow(centrei - ptX2.i,2);
	float y = pow(centrej - ptX2.j,2);
	printf("%d - %f = %f \n",centrej , ptX2.j ,centrej - ptX2.j);
	return sqrt(pow(centrei - ptX2.i,2)+pow(centrej - ptX2.j,2));
}


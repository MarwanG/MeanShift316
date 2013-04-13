#include "calcul.h"
#include <stdlib.h>
#include <stdio.h>
#include <inrimage/image.h>



void remplir_basic(unsigned char *buf , struct pt_x* tabPt_x , int x , int y){
	printf("remplir_basic \n");

	int i , j , k;
	k=0;
	for( j=0; j<x; j++){
	    for( i=0; i<y; i++){
	      tabPt_x[k].i = i;
	      tabPt_x[k].j = j;
	      tabPt_x[k].r = buf[(i*x)+j];
	      tabPt_x[k].g = 0;
	      tabPt_x[k].b = 0;
	   	  k++;
	   	 }
	}

	printf("fin de remplir_basic\n");
}


// s needs to be changed into the 2 dimentions but todo in uni due to jpg

void remplir_rgb(unsigned char *buf , struct pt_x* tabPt_x , int s){
	
	printf("remplir_rgb\n");
	
    
	int i,j,k,indiceX,indiceY;	
	indiceX=0;
	indiceY=0;	
	k=0;
	while(j < 3*s){
		indiceX=0;	
		i = 0;
		while( i < 3*s){		
			tabPt_x[k].i = indiceX;
			tabPt_x[k].j = indiceY;
			tabPt_x[k].r = buf[(i*s)+j];
			tabPt_x[k].g = buf[(i*s)+j+1];
			tabPt_x[k].b = buf[(i*s)+j+2];
			k++;
			indiceX++;
			i = i + 3;
		}	
		indiceY++;
		j = j + 3;
	}
}

struct pt_x * mean_shift(struct pt_x tabPt_x[] , struct pt_x centre , float hs , float hr , int maxIter , int s, int sizeX , int sizeY){

	int i ;
	int div = 0;
	int noyau;
	int k = 0;
	struct pt_x y , * mult , *sum ,tmp;
	sum  = malloc(sizeof( pt_x));
	mult = malloc(sizeof(pt_x));
	y = centre;
	tmp = centre;
	
	do{
		y = tmp;
		div = 0;
		sum->i = 0;
		sum->j = 0;
		sum->r =0;
		mult->i = 0;
		mult->j = 0;
		mult->r =0;

		for(i = 0 ; i < sizeX*sizeY ; i++){

			if( noyau_coord(y.i - tabPt_x[i].i , y.j - tabPt_x[i].j ,hs) == 1 ){
				
				noyau = -1 * noyau_rgb(y.r - tabPt_x[i].r , y.g - tabPt_x[i].g , y.b - tabPt_x[i].b , hr);			
				div = div + noyau;	
				mult_pt(tabPt_x[i],noyau,mult);
				sum_pt(*mult,sum,sum);
			}
		}
		div_pt(*sum,div,&tmp);
		k++;

	}while (!((k > maxIter) || (moduloPt(tmp,y) < s)));

	y = tmp;
	return &y;
}



void debruit_basic(struct pt_x* tabPt_x , unsigned char *buf , float hs , float hr , int maxIter , int s, int sizeX , int sizeY){
	printf("debruit_basic \n");
	struct pt_x *res;
	int i ,x , y ,value;
	
	x = 0;
	y = 0;
	
	for(i = 0 ; i < sizeX * sizeY ; i++){
	    
	    res = mean_shift(tabPt_x , tabPt_x[i] , hs  , hr , maxIter , s ,sizeX , sizeY);
	    value = (int)res->r;
	    buf[x * sizeX + y]=value;     
	    x++;
	    							
	    if(i%sizeX == 0){				// to verifie
	    	y++;
	    	x=0;
    	}
  	}
 	
 	printf("Fin debruit_basic \n");

}
/*
void debruit_rgb(struct pt_x* tabPt_x , unsigned char *buf ,float hs , float hr , int maxIter , int s ){
	struct pt_x *tmp;
	int i , tmpi , tmpj;
	int k ;
	k = 0;
	for(i = 0 ; i < 256*256 ; i++){
		tmpi = tabPt_x[i].i;
		tmpj = tabPt_x[i].j;
		tmp = mean_shift(tabPt_x,tabPt_x[i],hs,hr,maxIter,s);
		buf[k] = tmp->r;
		buf[k+1] = tmp->g;
		buf[k+2] = tmp->b;
		k=k+3;
	}
}
*/

void affiche_tab(struct pt_x* tabPt_x,int x , int y){
	int i;
	for(i=0; i < x*y ; i++){
		printf("%d ) i = %f j = %f r = %f g = %f b= %f \n", i , tabPt_x[i].i , tabPt_x[i].j , tabPt_x[i].r , tabPt_x[i].g , tabPt_x[i].b);
	}
}
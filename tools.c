#include "calcul.h"
#include <stdlib.h>
#include <stdio.h>
#include <inrimage/image.h>


/*
USE FPRINTF
*/

void remplir_basic(unsigned char *buf , struct pt_x* tabPt_x , int x , int y){
	fprintf(stderr,"remplir_basic \n");

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

	fprintf(stderr,"fin de remplir_basic\n");
}



void remplir_rgb(unsigned char *buf , struct pt_x* tabPt_x , int x , int y){
	
	fprintf(stderr,"remplir_rgb\n");
	
    
	int i,j,k,indiceX,indiceY;	
	indiceX=0;
	indiceY=0;	
	k=0;
	j=0;
	while(j < 3*x){
		indiceX=0;	
		i = 0;
		while( i < 3*y){		
			tabPt_x[k].i = indiceX;
			tabPt_x[k].j = indiceY;
			tabPt_x[k].r = buf[(i*x)+j];
			tabPt_x[k].g = buf[(i*x)+j+1];
			tabPt_x[k].b = buf[(i*x)+j+2];
			k++;
			indiceX++;
			i = i + 3;
		}	
		indiceY++;
		j = j + 3;
	}

	fprintf(stderr,"fin de remplir_rgb \n");
}

<<<<<<< HEAD
void remplir_rgbSeg(unsigned char *buf, pt_x* tabPt_x ,int centerX , int centerY){
	int [9][9] indice;
	int i,j;
	int indiceI,indiceJ = 0;
	for(j = centerX - 1 ; j < centerX + 1 ; j++){
		for(i = centerY - 1 ; i < centerY +1 ; i++){
			
		}
	}
}


=======
>>>>>>> debruit not so sure
void setPixel(pt_x *pt , int i , int j , int r , int g , int b){
	pt->i = i;
	pt->j = j;
	pt->r = r;
	pt->g = g;
	pt->b = b;
}


struct pt_x * mean_shift(struct pt_x tabPt_x[] , struct pt_x centre , float hs , float hr , int maxIter , int s, int sizeX , int sizeY){

	int i ;
	float div = 0;
	float noyau;
	int k = 0;
	struct pt_x *y , *mult , *sum ,tmp;
	sum  = malloc(sizeof(pt_x));
	mult = malloc(sizeof(pt_x));
	y = malloc(sizeof(pt_x));
	*y = centre;
	tmp = centre;
	
	do{
		*y = tmp;
		div = 0;
		setPixel(sum,0,0,0,0,0);
		setPixel(mult,0,0,0,0,0);
		for(i = 0 ; i < sizeX*sizeY ; i++){
<<<<<<< HEAD
			noyau = noyau_coord(y->i - tabPt_x[i].i , y->j - tabPt_x[i].j ,hs);
			//fprintf(stderr, "noyauCoord = %f\n",noyau);			
			if(noyau != 0){				
				noyau = noyau * noyau_rgb(y->r - tabPt_x[i].r , y->g - tabPt_x[i].g , y->b - tabPt_x[i].b , hr);	//CALCULATE REST OF NOYAU		
				div = div + noyau;																						// CALCULATE THE DOMINATOR
				mult_pt(tabPt_x[i],noyau,mult);																			//MULT DE IEME PIXEL WITH NOYAU
				sum_pt(*mult,sum,sum);																					//ADDITION
=======

			if( noyau_coord(y->i - tabPt_x[i].i , y->j - tabPt_x[i].j ,hs) == 1 ){			//TEST IF EQUAL TO 1 SINCE IF EQUAL TO 0 THEN USELESS
				
				noyau = -1 * noyau_rgb(y->r - tabPt_x[i].r , y->g - tabPt_x[i].g , y->b - tabPt_x[i].b , hr);	//CALCULATE REST OF NOYAU		
				div = div + noyau;															// CALCULATE THE DOMINATOR
				mult_pt(tabPt_x[i],noyau,mult);												//MULT DE IEME PIXEL WITH NOYAU
				sum_pt(*mult,sum,sum);														//ADDITION
>>>>>>> debruit not so sure
			}
		}
		div_pt(*sum,div,&tmp);
		k++;

	}while (!((k > maxIter) || (moduloPt(tmp,*y) < s)));

	free(sum);
	free(mult);

	*y = tmp;
	return y;
}



void debruit_basic(struct pt_x* tabPt_x , unsigned char *buf , float hs , float hr , int maxIter , int s, int sizeX , int sizeY){
	fprintf(stderr,"debruit_basic \n");
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
 	
 	fprintf(stderr,"Fin debruit_basic \n");

}

/*
NEED TO FIND A WAY TO PROPERLY FILL THE BUFFER SHIT
*/

void debruit_rgb(struct pt_x* tabPt_x , unsigned char *buf ,float hs , float hr , int maxIter , int s , int sizeX , int sizeY){
	fprintf(stderr,"debruit_rgb \n");
	struct pt_x *tmp;
	int i , tmpi , tmpj;
	int x , y , j;
	x = 0;
	y = 0;
	j = 0;

	for(i = 0 ; i < sizeX*sizeY ; i++){
		tmp = mean_shift(tabPt_x,tabPt_x[i],hs,hr,maxIter,s,sizeX,sizeY);
<<<<<<< HEAD
		//fprintf(stderr,"values  r = %f g =  %f b = %f\n",tmp->r,tmp->g,tmp->b);

		x++;
		buf[j] = tmp->r;
		buf[j+1] = tmp->g;
		buf[j+2] = tmp->b;
		//printf("%f %f %f \n", buf[j] , buf[j+1] , buf[j+2]);
		x++;
=======
		buf[j] = tmp->r;
		buf[j+1] = tmp->g;
		buf[j+2] = tmp->b;
		x++;

>>>>>>> debruit not so sure
		if(i%sizeX == 0){				// to verifie
			fprintf(stderr, "i = %d\n", i);
    	}
    	j = j + 3;
	}
	fprintf(stderr,"fin de debruit_rgb \n");
}
<<<<<<< HEAD


/*
NEED TO BE FINISHED
*/
=======
>>>>>>> debruit not so sure

void segmentation(pt_x* tabPt_x , unsigned char *buf , float hs , float hr ,int maxIter , int s , int sizeX , int sizeY){
	
	int i;
	//create buffer of Image J (debruit)
	debruit_rgb(tabPt_x,buf,hs,hr,maxIter,s,sizeX,sizeY);
	//remplir tableau 2
	pt_x* tabPt_x2 = malloc(sizeX* sizeY* 3 *sizeof(pt_x));
	remplir_rgb(buf,tabPt_x2,sizeX,sizeY);
	//


}

void affiche_tab(struct pt_x* tabPt_x,int x , int y){
	fprintf(stderr,"debut de affiche_tab \n");
	int i;
	for(i=0; i < x*y ; i++){
		printf("%d ) i = %f j = %f r = %f g = %f b= %f \n", i , tabPt_x[i].i , tabPt_x[i].j , tabPt_x[i].r , tabPt_x[i].g , tabPt_x[i].b);
	}
	fprintf(stderr,"fin de affiche_tab \n");
}
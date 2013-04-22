#include "calcul.h"
#include <stdlib.h>
#include <stdio.h>
#include <inrimage/image.h>
#include "tools.h"


/**
* Fills table of pt_x with buf of an image 256 levels of grey.
* @param buf buffered used to filled
* @param tabPt_x table of pt_x that will be filled
* @param sizeX dim de buf
* @param sizeY dim de buf
*/

void remplir_basic(unsigned char *buf , struct pt_x* tabPt_x , int sizeX , int sizeY){
	if(debug == 1){
		fprintf(stderr,"remplir_basic \n");
	}
	int i , j , k;
	k=0;
	for( j=0; j<sizeX; j++){
	    for( i=0; i<sizeY; i++){
	      tabPt_x[k].i = i;
	      tabPt_x[k].j = j;
	      tabPt_x[k].r = buf[(i*sizeX)+j];
	      tabPt_x[k].g = 0;
	      tabPt_x[k].b = 0;
	   	  k++;
	   	 }
	}
	if(debug == 1){
		fprintf(stderr,"fin de remplir_basic\n");
	}
}

/**
* Fills table of pt_x with buf of a rgb image.
* @param buf buffered used to filled
* @param tabPt_x table of pt_x that will be filled
* @param x dim de buf
* @param y dim de buf
*/

void remplir_rgb(unsigned char *buf , struct pt_x* tabPt_x , int sizeX , int sizeY){
	
	if(debug == 1){
		fprintf(stderr,"remplir_rgb\n");
	}
    
	int i,j,k,indiceX,indiceY;	
	indiceX=0;
	indiceY=0;	
	k=0;
	j=0;
	while(j < 3*sizeX){
		indiceX=0;	
		i = 0;
		while( i < 3*sizeY){		
			tabPt_x[k].i = indiceX;
			tabPt_x[k].j = indiceY;
			tabPt_x[k].r = buf[(j*sizeX)+i];
			tabPt_x[k].g = buf[(j*sizeX)+i+1];
			tabPt_x[k].b = buf[(j*sizeX)+i+2];
			if(debug)
				fprintf(stderr, "i = %d\n",i);
			k++;
			indiceX++;
			i = i + 3;
		}	
		indiceY++;
		j = j + 3;
	}
	if(debug==1){
		fprintf(stderr,"fin de remplir_rgb \n");
	}
}


/**
* sets the values for pt.
* @param pt element of type pt_x that will be modifed
* @param i index to be modifed
* @param y index to be modifed
* @param r composant r that to be placed in pt
* @param g composant g that to be placed in pt
* @param b composant b that to be placed in pt
*/

void setPixel(pt_x *pt , int i , int j , int r , int g , int b){
	pt->i = i;
	pt->j = j;
	pt->r = r;
	pt->g = g;
	pt->b = b;
}

/**
* calcalates the mean shift for the pixel center
* @param tabPt_x[] table with neighbors of pixel center
* @param center pixel
* @param hs variable
* @param hr variable
* @param s variable of precision
* @param sizeX variable of size
* @param sizeY variable of size
*/


struct pt_x * mean_shift(struct pt_x tabPt_x[] , struct pt_x centre , float hs , float hr , int s, int sizeX , int sizeY){

	int i , iDebut , jDebut;
	float div = 0;
	float noyau;
	int k = 0;
	struct pt_x *y , *mult , *sum ,tmp;
	sum  = malloc(sizeof(pt_x));
	mult = malloc(sizeof(pt_x));
	y = malloc(sizeof(pt_x));
	*y = centre;
	tmp = centre;
	iDebut = centre.i;
	jDebut = centre.j;
	do{
		*y = tmp;
		div = 0;
		setPixel(sum,0,0,0,0,0);
		setPixel(mult,0,0,0,0,0);
		for(i = 0 ; i < sizeX*sizeY ; i++){

			
			if(noyau == 1){
				noyau = noyau_gaussCoord(y->i - tabPt_x[i].i , y->j - tabPt_x[i].j ,hs);	
				noyau = -noyau * noyau_gaussRgb(y->r - tabPt_x[i].r , y->g - tabPt_x[i].g , y->b - tabPt_x[i].b , hr);			
				div = div + noyau;	
				mult_pt(tabPt_x[i],noyau,mult);
				sum_pt(*mult,sum,sum);		
			}else{
				if(noyau_coord(y->i - tabPt_x[i].i , y->j - tabPt_x[i].j ,hs) == 1 ){	
					noyau = -1 * noyau_rgb(y->r - tabPt_x[i].r , y->g - tabPt_x[i].g , y->b - tabPt_x[i].b , hr);			
					div = div + noyau;	
					mult_pt(tabPt_x[i],noyau,mult);
					sum_pt(*mult,sum,sum);
				}
			}
		}
		div_pt(*sum,div,&tmp);
		k++;

	}while (!( k > max || (moduloPt(tmp,*y) < s)));

	free(sum);
	free(mult);

	*y = tmp;
	return y;
}

/**
* fills table with the neighbors of center pixel picked by 
* index
* @param tabPt_x[] table with all pixels from the original image
* @param indice index of the center pixel
* @param tabPt_x2[] the table to be filled
* @param sizeX size of actual image
* @param sizeY size of actual image 
*/
void getVoisinage(pt_x tabPt_x[] ,int indice, pt_x tabPt_x2[] ,int sizeX , int sizeY){
  int i,j;
  int ix = tabPt_x[indice].i - VOSIN;
  int iy = tabPt_x[indice].j - VOSIN;
  int ll = 0;
  for(j = iy ; j <= tabPt_x[indice].j + VOSIN ; j++){
    for(i = ix ; i  <= tabPt_x[indice].i + VOSIN  ; i++){
      int index = (j * sizeX) + i;
      if(index < 0 || index >= sizeX * sizeY)
      	tabPt_x2[ll] = tabPt_x[indice];
      else
      	tabPt_x2[ll] = tabPt_x[index];
      ll++;	
    }
  } 
}

/**
* removes noise from a image of 256 levels of grey
* @param tabPt_x[] table with pixels to be denoised
* @param *buf buffer to be filled with the new values
* @param hs 
* @param hr
* @param s variable of precision
* @param sizeX size of actual image
* @param sizeY size of actual image 
*/
void debruit_basic(struct pt_x* tabPt_x , unsigned char *buf , float hs , float hr , int s, int sizeX , int sizeY){
	if(debug == 1){
		fprintf(stderr,"debruit_basic");
	}
	struct pt_x *res;
	int i ,x , y ,value;
	
	x = 0;
	y = 0;
	
	for(i = 0 ; i < sizeX * sizeY ; i++){
	    res = mean_shift(tabPt_x , tabPt_x[i] , hs  , hr , s ,sizeX , sizeY);
	    value = (int)res->r;
	    buf[x * sizeX + y]=value;     
	    x++;
	    							
	    if(i%sizeX == 0){				// to verifie
	    	y++;
	    	x=0;
    	}


  	}
  	if(debug == 1){	
 		fprintf(stderr,"Fin debruit_basic \n");
 	}
}

/**
* removes noise from a rgb image
* @param tabPt_x[] table with pixels to be denoised
* @param *buf buffer to be filled with the new values
* @param hs 
* @param hr
* @param s variable of precision
* @param sizeX size of actual image
* @param sizeY size of actual image 
*/

void debruit_rgb(struct pt_x* tabPt_x , unsigned char *buf ,float hs , float hr , int s , int sizeX , int sizeY){
	
	if(debug == 1){
		fprintf(stderr,"debruit_rgb  \n");
	}
	struct pt_x *tmp , *tmp2;
	int i , tmpi , tmpj;
	int x , y , j ,k;
	x = 0;
	y = 0;
	j = 0;
	k=0;
	tmp2 = malloc(pow(VOSIN*2+1,2) *sizeof(pt_x));
	for(i = 0; i < sizeX*sizeY  ; i++){
		getVoisinage(tabPt_x,i,tmp2,sizeX,sizeY);
		tmp = mean_shift(tmp2,tabPt_x[i],hs,hr,s,VOSIN*2+1,VOSIN*2+1);
		buf[(3 * k)]   = (unsigned char)tmp->r;	
		buf[(3 * k)+1] = (unsigned char)tmp->g;
		buf[(3 * k)+2] = (unsigned char)tmp->b;
		k++;
		if(debug){
			fprintf(stderr, "i = %d\n",i);
		}
	}
	if(debug == 1){
		fprintf(stderr,"fin de debruit_rgb \n");
	}
}

/**
* segmenation of an image rgb
* @param tabPt_x[] table with pixels to be segmentated
* @param *buf buffer to be filled with the new values
* @param hs 
* @param hr
* @param s variable of precision
* @param sizeX size of actual image
* @param sizeY size of actual image 
*/

void segmentation(pt_x* tabPt_x , unsigned char *buf , float hs , float hr , int s , int sizeX , int sizeY){
	
	if(debug == 1){
		fprintf(stderr, "debut segmentation\n");
	}
	int i,j,z,k = 0;
	float coord,rgb;
	pt_x x , y ;

	unsigned char *buftmp = (unsigned char*)i_malloc( sizeX * sizeY* 3 *sizeof(unsigned char));
	
	/*create buffer of Image J (debruit)*/
	debruit_rgb(tabPt_x,buftmp,hs,hr,s,sizeX,sizeY);

	/*Histograme*/
	
	int ***his = (int ***)i_malloc(sizeof(int**) * 256);
  	for(i = 0 ; i < 256 ; i++){
    	his[i]=  (int **)i_malloc(sizeof(int*) * 256);
    	for(j = 0 ; j < 256 ; j++){
     	 	his[i][j] = (int *)i_malloc(sizeof(int) * 256);
      		for(z = 0 ; z < 256 ; z++){
         		his[i][j][z]=0;
		       }
    	}
  	}

	calcul_Histogramme(buf,his,sizeX,sizeY);

	pt_x* tabPt_x2 = malloc(sizeX* sizeY* 3 *sizeof(pt_x));
	remplir_rgb(buftmp,tabPt_x2,sizeX,sizeY);

	for(i = 0 ; i < sizeX * sizeY ; i++){
		x = tabPt_x2[i];
		for(j = 0 ; j < sizeX * sizeY ; j++){
			y = tabPt_x2[j];
			coord = distancePointsCoord(x,y);
			rgb = distancePointsRgb(x,y);
			if(coord < hs &&  rgb < hr){
				if(his[(int)x.r][(int)x.g][(int)x.b] > his[(int)y.r][(int)y.g][(int)y.b]){
					buf[k] = x.r;
					buf[k+1] = x.g;
					buf[k+2] = x.b;
				}else{
					buf[k] = y.r;
					buf[k+1] = y.g;
					buf[k+2] = y.b;
				}		
			}
		}
		if(debug == 1)
			fprintf(stderr, "i = %d\n",i);
		k=k+3;
	}
	
	if(debug == 1){
	fprintf(stderr, "Fin de segmentation\n");
	}
}

/**
* prints the values of the table passed in the parameters
* @param tabPt_x table that will be printed
* @param x size of the table
* @param y size of the table
*/
void affiche_tab(struct pt_x* tabPt_x,int x , int y){
	fprintf(stderr,"debut de affiche_tab \n");
	int i;
	for(i=0; i < x*y ; i++){
		printf("%d ) i = %f j = %f r = %f g = %f b= %f \n", i , tabPt_x[i].i , tabPt_x[i].j , tabPt_x[i].r , tabPt_x[i].g , tabPt_x[i].b);
	}
	fprintf(stderr,"fin de affiche_tab \n");
}


/**
* calculate the histograme for the buffer passed in the parameters
* @param buf pointer to the buffer that will be used in calculation
* @param histo the table that will be filled
* @param sizeX size of the buffer
* @param sizeY size of the buffer
*/

void calcul_Histogramme(unsigned char *buf, int*** histo , int sizeX , int sizeY){
	int i,j,k;	
	k=0;
	j=0;
	int r , g , b;
	while(j < 3*sizeX){
		i = 0;
		while( i < 3*sizeY){
			r = buf[(j*sizeX)+i];
			g = buf[(j*sizeX)+i+1];
			b = buf[(j*sizeX)+i+2];
			histo[r][g][b]++;
			i = i + 3;
		}	
		j = j + 3;
	}
}

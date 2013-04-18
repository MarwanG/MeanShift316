#include "calcul.h"
#include <stdlib.h>
#include <stdio.h>
#include <inrimage/image.h>





/**
* Fills table of pt_x with buf of an image 256 levels of grey.
* @param buf buffered used to filled
* @param tabPt_x table of pt_x that will be filled
* @param x dim de buf
* @param y dim de buf
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

/**
* Fills table of pt_x with buf of a rgb image.
* @param buf buffered used to filled
* @param tabPt_x table of pt_x that will be filled
* @param x dim de buf
* @param y dim de buf
*/

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
			tabPt_x[k].r = buf[(j*x)+i];
			tabPt_x[k].g = buf[(j*x)+i+1];
			tabPt_x[k].b = buf[(j*x)+i+2];
			k++;
			indiceX++;
			i = i + 3;
		}	
		indiceY++;
		j = j + 3;
	}

	fprintf(stderr,"fin de remplir_rgb \n");
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


struct pt_x * mean_shift(struct pt_x tabPt_x[] , struct pt_x centre , float hs , float hr , int maxIter , int s, int sizeX , int sizeY){

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
			
			noyau = noyau_gaussCoord(y->i - tabPt_x[i].i , y->j - tabPt_x[i].j ,hs);	
			noyau = -noyau * noyau_gaussRgb(y->r - tabPt_x[i].r , y->g - tabPt_x[i].g , y->b - tabPt_x[i].b , hr);			
			div = div + noyau;	
			mult_pt(tabPt_x[i],noyau,mult);
			sum_pt(*mult,sum,sum);
				
			}
			/*if( noyau_coord(y->i - tabPt_x[i].i , y->j - tabPt_x[i].j ,hs) == 1 ){	
					noyau = -1 * noyau_rgb(y->r - tabPt_x[i].r , y->g - tabPt_x[i].g , y->b - tabPt_x[i].b , hr);			
					div = div + noyau;	
					mult_pt(tabPt_x[i],noyau,mult);
					sum_pt(*mult,sum,sum);
				}
			}*/
		div_pt(*sum,div,&tmp);
		k++;

	}while (!((k > maxIter) || (moduloPt(tmp,*y) < s)));

	free(sum);
	free(mult);

	*y = tmp;
	return y;
}


void getVoisinage(pt_x tabPt_x[] ,int indice, pt_x tabPt_x2[] , int rang , int sizeX , int sizeY){
  int i,j;
  int ix = tabPt_x[indice].i - rang;
  int iy = tabPt_x[indice].j - rang;
  int ll = 0;
  for(j = iy ; j <= tabPt_x[indice].j + rang ; j++){
    for(i = ix ; i  <= tabPt_x[indice].i + rang  ; i++){
      int index = (j * sizeX) + i;
      if(index < 0 || index >= sizeX * sizeY)
      	tabPt_x2[ll] = tabPt_x[indice];
      else
      	tabPt_x2[ll] = tabPt_x[index];
      ll++;	
    }
  } 
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


void debruit_rgb(struct pt_x* tabPt_x , unsigned char *buf ,float hs , float hr , int maxIter , int s , int sizeX , int sizeY){
	fprintf(stderr,"debruit_rgb \n");
	
	struct pt_x *tmp , *tmp2;
	int i , tmpi , tmpj;
	int x , y , j ,k;
	x = 0;
	y = 0;
	j = 0;
	k=0;
	tmp2 = malloc(7*7 *sizeof(pt_x));
	for(i = 0; i < sizeX*sizeY  ; i++){
		getVoisinage(tabPt_x,i,tmp2,3,sizeX,sizeY);
		tmp = mean_shift(tmp2,tabPt_x[i],hs,hr,maxIter,s,5,5);
		buf[(3 * k)]   = (unsigned char)tmp->r;	
		buf[(3 * k)+1] = (unsigned char)tmp->g;
		buf[(3 * k)+2] = (unsigned char)tmp->b;
		k++;
	}
	fprintf(stderr,"fin de debruit_rgb \n");
}


void segmentation(pt_x* tabPt_x , unsigned char *buf , float hs , float hr ,int maxIter , int s , int sizeX , int sizeY){
	
	int i,j,z,k = 0;
	float coord,rgb;
	pt_x x , y ;
	//debruit_rgb(tabPt_x,buf,hs,hr,maxIter,s,sizeX,sizeY);
	printf("dans segmentation %d \n",buf[3]);
	
	/*create buffer of Image J (debruit)*/
	debruit_rgb(tabPt_x,buf,hs,hr,maxIter,s,sizeX,sizeY);
	printf(" apres %d\n",buf[3]);

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
		
	pt_x* tabPt_x2 = malloc(sizeX* sizeY* 3 *sizeof(pt_x));
	
	remplir_rgb(buf,tabPt_x2,sizeX,sizeY);

	printf("START OF LOOP \n");
	for(i = 0 ; i < sizeX * sizeY ; i++){
		x = tabPt_x2[i];
		for(j = 0 ; j < sizeX * sizeY ; j++){
			y = tabPt_x2[j];
			coord = distancePointsCoord(x,y);
			//printf("coord = %f\n",coord);
			rgb = distancePointsRgb(x,y);
			//printf("rgb = %f\n",rgb);
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
		k=k+3;
	}
}

void affiche_tab(struct pt_x* tabPt_x,int x , int y){
	fprintf(stderr,"debut de affiche_tab \n");
	int i;
	for(i=0; i < x*y ; i++){
		printf("%d ) i = %f j = %f r = %f g = %f b= %f \n", i , tabPt_x[i].i , tabPt_x[i].j , tabPt_x[i].r , tabPt_x[i].g , tabPt_x[i].b);
	}
	fprintf(stderr,"fin de affiche_tab \n");
}


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
			printf("inside = %d\n", histo[r][g][b]);
			i = i + 3;
		}	
		j = j + 3;
	}
}

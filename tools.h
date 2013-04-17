#include <stdlib.h>
#include <stdio.h>
#include <math.h>




/* Fills the table of pixels using the buff for an image of 256 levels of grey of size X and Y*/

void remplir_basic(unsigned char *buf , struct pt_x tabPt_x[] ,int x , int y);

/* Fills the table of pixels using the buff for a RGB image */

void remplir_rgb(unsigned char *buf , struct pt_x* tabPt_x , int x , int y);


/* Applies the meanshift for the point center with the given variables */

struct pt_x* mean_shift(struct pt_x tabPt_x[] , struct pt_x centre , float hs , float hr , int maxIter , int s ,int x , int y);

/* does debruit for an image of 256 levels of grey of size s */
void debruit_basic(struct pt_x* tabPt_x , unsigned char *buf , float hs , float hr , int maxIter , int s , int sizeX , int sizeY);

/* does debruit for an image of rgb  */
void debruit_rgb(struct pt_x* tabPt_x , unsigned char *buf ,float hs , float hr , int maxIter , int s , int sizeX , int sizeY);

/*prints out the table with its values */
void affiche_tab(struct pt_x* tabPt_x,int x , int y);

/*sets the pixel to the values*/
void setPixel(pt_x *pt , int i , int j , int r , int g , int b);

void remplir_rgbSeg(unsigned char *buf, pt_x* tabPt_x ,int centerX , int centerY , int sizeX , int sizeY);

void getVoisinage(pt_x tabPt_x[] ,int indice, pt_x tabPt_x2[] , int sizeX , int sizeY);
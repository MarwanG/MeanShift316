/*
 * LI316 - Introduction a l'imagerie numerique
 * Fichier template Inrimage
 */

#include <inrimage/image.h>
#include "calcul.h"
#include "tools.h"

static char version[]="1.00";
static char usage[]="[input][output] -hr -hs";
static char detail[]="";

int main( int argc, char **argv) {
  char fname[256];
  char nom[256];
  struct image *nf;
  Fort_int lfmt[9];
  unsigned char *buf , *buf2;
  int sb = 0, i, j;
  struct pt_x* pt;
  struct pt_x *res;
  struct pt_x * tabPt_x;

  float hr , hs;
  hr = 5;
  hs = 8;
  /* Initialisation */
  inr_init( argc, argv, version, usage, detail);

  /* Lecture des options */
  infileopt( fname);
  igetopt1("-hs", "%f", &hs);
  igetopt1("-hr", "%f", &hr);

  /* Ouverture et lecture des images */
  nf = image_(fname, "e", "", lfmt);

  /* verification du format */
  if((TYPE == FIXE && BSIZE==1 && NDIMV == 1)){
    
  /* allocation memoire adequat */

    buf = (unsigned char*)i_malloc( NDIMX * NDIMY* NDIMV *sizeof(unsigned char));
 
  /* cree tableau de pt_x */

    tabPt_x = malloc(NDIMX* NDIMY* NDIMV *sizeof(pt_x));
     
  /* lecture image */
    c_lect( nf, NDIMY, buf);

  /* Traitement */
    remplir_basic(buf,tabPt_x,NDIMX,NDIMY);
  
  }else if (NDIMV == 3){

    printf("photo color rgb \n");
    tabPt_x = malloc(NDIMX*NDIMY* NDIMV *sizeof(pt_x));
   
    buf = (unsigned char*)i_malloc( NDIMX * NDIMY* NDIMV *sizeof(unsigned char));  
    
     /* lecture image */
    c_lect( nf, NDIMY, buf);

    remplir_rgb(buf,tabPt_x,NDIMX,NDIMY);

  }else{
    imerror( 6, "codage non conforme\n");
  }
  
 

  /* Affichage de tableau */
  
  //affiche_tab(tabPt_x,NDIMX,NDIMY);

  
  /*debruit*/
  buf2 = (unsigned char*)i_malloc( NDIMX * NDIMY*NDIMV*sizeof(unsigned char));

  if(NDIMV == 1){
    debruit_basic(tabPt_x,buf2,hs,hr,1000,1,NDIMX,NDIMY);
  }else{
<<<<<<< HEAD
    debruit_rgb(tabPt_x,buf2,hs,hr,10,10,NDIMX,NDIMY);
=======
    debruit_rgb(tabPt_x,buf2,hs,hr,1000,10,NDIMX,NDIMY);
>>>>>>> debruit not so sure
  }
 

  /*sauvgarde*/
  outfileopt(nom);
  nf = c_image(nom,"c","",lfmt);
  c_ecr(nf,DIMY,buf2);

  /* fermeture image */
  fermnf_( &nf);
  


  i_Free((void*)&buf);
  i_Free((void*)&buf2);
  return 0;
}
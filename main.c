/*
 * LI316 - Introduction a l'imagerie numerique
 * Fichier template Inrimage
 */

#include <inrimage/image.h>
#include "calcul.h"
#include "tools.h"

static char version[]="1.00";
static char usage[]="[input][output] -hr -hs -max -v -n -m -d";
static char detail[]="";


int max = 1000;
int VOSIN = 10;
int noyau = 1;
int debug = 1;
int mode = 1;

int main( int argc, char **argv) {
  char fname[256];
  char nom[256];
  struct image *nf;
  Fort_int lfmt[9];
  unsigned char *buf , *buf2;
  int sb = 0, i, j,z;
  struct pt_x* pt;
  struct pt_x *res;
  struct pt_x * tabPt_x;
  char reponse;

  float hr , hs;
  hr = 20;
  hs = 20;
  /* Initialisation */
  inr_init( argc, argv, version, usage, detail);

  /* Lecture des options */
  infileopt( fname);
  igetopt1("-hs", "%f", &hs);
  igetopt1("-hr", "%f", &hr);
  igetopt1("-max","%d", &max);
  igetopt1("-v","%d",&VOSIN);
  igetopt1("-n","%d",&noyau);
  igetopt1("-m","%d",&mode);
  igetopt1("-d","%d",&debug);
  outfileopt(nom);

  /*affichage the options */
  fprintf(stderr, "=============OPTIONS===========\n");
  fprintf(stderr, "hr = %f hs = %f\n",hr,hs);
  fprintf(stderr, "Voisnage = %d\n",VOSIN);
  if(noyau == 1){
    fprintf(stderr, "Noyau =  gaussian\n");
  }else{
    fprintf(stderr, "Noyau = epankovic\n");
  }
  if(mode == 1){
    fprintf(stderr, "Mode  = segmentation\n");
  }else{
    fprintf(stderr, "Mode  = debruit\n");
  }
  printf("Image src = %s\n",fname);
  printf("Image target = %s\n",nom);
  printf("Mode debug = %d\n",debug);
  fprintf(stderr, "===============================\n");
  fprintf(stderr, "would you like to continue the execution with current options ? (y,n)\n");
  scanf("%c",&reponse);
  if(reponse!='y'){
    return 0;
  }
  fprintf(stderr, "Start of execution\n");
  /* Ouverture et lecture des images */
  nf = image_(fname, "e", "", lfmt);

  /* verification du format */
  if(TYPE == FIXE && BSIZE==1){
    
  /* allocation memoire adequat */

    buf = (unsigned char*)i_malloc( NDIMX * NDIMY* NDIMV *sizeof(unsigned char));
 
  /* cree tableau de pt_x */

    tabPt_x = malloc(NDIMX* NDIMY* NDIMV *sizeof(pt_x));
     
  /* lecture image */
    c_lect( nf, NDIMY, buf);

  }else{
    imerror( 6, "codage non conforme\n");
  }
  
  /* Remplir de Struct Special */
  
  if(NDIMV == 1){
    remplir_basic(buf,tabPt_x,NDIMX,NDIMY);
  }
  if(NDIMV == 3){
    remplir_rgb(buf,tabPt_x,NDIMX,NDIMY);
  }



  /* Traitement */

  /*debruit*/

  buf2 = (unsigned char*)i_malloc( NDIMX * NDIMY*NDIMV*sizeof(unsigned char));

  if(mode == 0){
    if(NDIMV == 1){
      debruit_basic(tabPt_x,buf2,hs,hr,1,NDIMX,NDIMY);
    }else{
      debruit_rgb(tabPt_x,buf2,hs,hr,1,NDIMX,NDIMY);
    }
  }else{
    segmentation(tabPt_x , buf2 , hs , hr , 0.1 , NDIMX, NDIMY);
  }
  
  /*sauvgarde*/
  
  printf("sortir \n");
  nf = c_image(nom,"c","",lfmt);
  c_ecr(nf,DIMY,buf2);

    /* fermeture image */
  fermnf_( &nf);

  //free(&his);
  i_Free((void*)&buf);
  i_Free((void*)&buf2);
  return 0;
}
#include <stdio.h>
#include <math.h>
#include "funct.h"

int CoNuBu(int *CubNum,int ***CubFAI,int ***CubLAI,int *CubLiA,float LaVdWR,
           float *ReMaxC,float *ReMinC,float **ReCoor,float *ReVdWR, int FrAtNu,
           float **RoSFCo,float *FrVdWR,float VdWFaB,int BumpMa,int RecHyN,
           int FraHyN,float BuEvFa)
/* This function counts the number of bumps between the fragment atoms and 
   the receptor. For each fragment atom, the checking is done only with the 
   receptor atoms which are in the cube to which the current fragment atom 
   belongs and the surrounding cubes. The fragment is rejected if the number 
   of bumps is larger than the maximum allowed or as soon as one of the bumps 
   is a large overlap : 
   CubAto  cube number in which the current atom is (1->along x,2->y,3->z)
   BumpNu  number of bumps between the current fragment and the receptor 
   SqDReF  squared distance between the receptor and fragment atoms
   SqDVdW  squared distance of the concerned atoms van der Waals radii sum 
   VdWFaB  factor for the checking of the bumps distances 
   BumpMa  maximum number of tolerated bumps 
   RecHyN  receptor hydrogen number in the current vector (0 if no hydrogen)
   FraHyN  fragment hydrogen number in the current vector (0 if no hydrogen) 
   BuEvFa  factor of bump evaluation 
   FrAcce  fragment accepted or not (1 -> yes, 0 -> no) */
{
  int i,j,k,l,n,CubAto[4],BumpNu,FrAcce;
  float SqDReF,SqDVdW; 

  BumpNu=0;
  FrAcce=1;

/* Loop over the fragment atoms */
  for (i=1;i<=FrAtNu;i++) {

    if (FrAcce) {

/* Find the cube in which the current fragment atom is or the nearest cube 
   if this fragment atom doesn't belong to any cubes of the grid */
      for (j=1;j<=3;j++) {
        if (RoSFCo[i][j]<ReMinC[j])
          CubAto[j]=1;
        else if (RoSFCo[i][j]>ReMaxC[j])
          CubAto[j]=CubNum[j];
        else 
          CubAto[j]=ffloor((RoSFCo[i][j]-ReMinC[j])/(2*LaVdWR))+1;
      }

/* Check whether there are bumps */ 
      for (j=-1;j<=1;j++) {
        for (k=-1;k<=1;k++) {
          for (l=-1;l<=1;l++) {

            if (FrAcce) {

              if (((CubAto[1]+j)>=1)&&((CubAto[1]+j)<=CubNum[1])&&
                  ((CubAto[2]+k)>=1)&&((CubAto[2]+k)<=CubNum[2])&&
                  ((CubAto[3]+l)>=1)&&((CubAto[3]+l)<=CubNum[3])) {

                if (CubFAI[CubAto[1]+j][CubAto[2]+k][CubAto[3]+l]!=0) {
 
                  for (n=CubFAI[CubAto[1]+j][CubAto[2]+k][CubAto[3]+l];
                       n<=CubLAI[CubAto[1]+j][CubAto[2]+k][CubAto[3]+l];n++) {

/* The checking is not done when one of the two atoms is the hydrogen involved 
   in the hydrogen bond (hydrogen of either the fragment or the receptor) */                  
                    if ((i!=FraHyN)&&(CubLiA[n]!=RecHyN)) {

                      SqDReF=DistSq(RoSFCo[i][1],RoSFCo[i][2],RoSFCo[i][3],
                                    ReCoor[CubLiA[n]][1],ReCoor[CubLiA[n]][2],
                                    ReCoor[CubLiA[n]][3]);

                      SqDVdW=(FrVdWR[i]+ReVdWR[CubLiA[n]])*
                             (FrVdWR[i]+ReVdWR[CubLiA[n]]);

                      if (SqDReF<(VdWFaB*VdWFaB*SqDVdW)) { 
                        BumpNu=BumpNu+1;
                        if ((BumpNu>BumpMa)||
                            ((SqDReF/(VdWFaB*VdWFaB*SqDVdW))<(BuEvFa*BuEvFa))) 
                          FrAcce=0;
                      }

                    }

                  }
  
                }

              }

            }

          }
        }
      }

    }

  }

  return FrAcce;

}
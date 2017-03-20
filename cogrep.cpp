#include <stdio.h>
#include <math.h>
#include "funct.h"

/* #ifdef OMP */
/* #include <omp.h> */
/* #endif */

void CoGReP(const int ReAtNu,float **ReCoor,float *RePaCh,const float CoDieV,
            const int CoDieP,const float CoGrIn,const float CoGrSi,float *BSMinC,int *CoGPoN,
            float ***CoGrRP)
/* This function computes the receptor part of the coulombic interaction on 
   a grid :
   CoGPoN  number of points for the grid of the coulombic interaction 
           (1->x,2->y,3->z)
   CoGrRP  value of the receptor part of the coulombic interaction on the 
           grid 
   CoGCor  coordinates of one point of the coulombic grid (1->x,2->y,3->z)
   GRSqDi  squared distance between two points (of the grid and receptor) */
{
  int i,j,k,m;
  float CoGCor[4],GRSqDi,CoGrRP_in;

  /* #pragma omp parallel default(shared) private(CoGrRP_in,CoGCor,GRSqDi,i,j,k) */
  /* CoGrIn,CoGrSi, */
#ifdef OMP 
#pragma omp parallel for default(none) shared(CoGPoN,BSMinC,ReCoor,RePaCh,CoGrRP) private(CoGrRP_in,CoGCor,GRSqDi,i,j,k,m) 
#endif
  for (i=1;i<=CoGPoN[1];i++) {
/*     { */
/* #pragma omp critical */
/*     printf("CBPT %d\n",i);  */
/*   } */
    for (j=1;j<=CoGPoN[2];j++) {
/* #ifdef OMP */
/* #pragma omp parallel for default(none) shared(CoGPoN,BSMinC,ReCoor,RePaCh,CoGrRP,i,j) private(CoGrRP_in,CoGCor,GRSqDi,k,m) sharing(dynamic) */
/* #endif */
      for (k=1;k<=CoGPoN[3];k++) {
 
/* Coordinates of one point of the coulombic grid */
        CoGCor[1]=BSMinC[1]-CoGrIn+(i-1)*CoGrSi;
        CoGCor[2]=BSMinC[2]-CoGrIn+(j-1)*CoGrSi;
        CoGCor[3]=BSMinC[3]-CoGrIn+(k-1)*CoGrSi;

/* Initialization of the "potential" */
        CoGrRP_in=0.0;

        for (m=1;m<=ReAtNu;m++) {
          GRSqDi=DistSq(CoGCor[1],CoGCor[2],CoGCor[3],
                        ReCoor[m][1],ReCoor[m][2],ReCoor[m][3]);
          if (GRSqDi>(1.e-4)) {
            if (CoDieP)
              CoGrRP_in=CoGrRP_in+RePaCh[m]/GRSqDi;
            else
              CoGrRP_in=CoGrRP_in+RePaCh[m]/sqrtf(GRSqDi);
          }
        }

        CoGrRP[i][j][k]=CoGrRP_in*332.0/CoDieV;

      }
    }
  }

}
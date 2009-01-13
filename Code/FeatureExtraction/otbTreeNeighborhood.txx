/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef __otbTreeNeighborhood_txx
#define __otbTreeNeighborhood_txx

#include "otbTreeNeighborhood.h"
#include "otbMacro.h"

namespace otb
{

/**
 *
 */
/* Reinitialise the neighborhood, so that it will be used for a new region */
void
Neighborhood::reinit_neighborhood(TypeOfTree type)
{
  iNbPoints = 0;
  type = type;
}

/* To allocate the structure representing the neighborhood of a region */
void
Neighborhood::init_neighborhood(int iMaxArea,
                                int iWidth,int iHeight)
{
  iMaxArea = 4*(iMaxArea+1);
  if(iMaxArea > iWidth*iHeight)
    iMaxArea = iWidth*iHeight;

  tabPoints = new Neighbor[iMaxArea+1];
  if(tabPoints == NULL)
    std::cerr << "init_neighborhood --> neighbors allocation error" << std::endl;
  reinit_neighborhood( AMBIGUOUS);
}

/* Free the structure representing the neighborhood of a region */
void
Neighborhood::free_neighborhood()
{
  delete[] tabPoints;
}


/* Put the last neighbor at a position so that we fix the heap */
void
Neighborhood::fix_up()
{
  Neighbor *tabPoints = tabPoints;
  int k = iNbPoints;
  int l;

  if(type == MAX)
    while(k > 1 && ORDER_MAX(k, l=k>>1))
      {
  SWAP(k, l);
  k = l;
      }
  else
    while(k > 1 && ORDER_MIN(k, l=k>>1))
      {
  SWAP(k, l);
  k = l;
      }
}

/* Put the first neighbor at a position so that we fix the heap */
void
Neighborhood::fix_down()
{
  Neighbor *tabPoints = tabPoints;
  int N = iNbPoints;
  int k = 1;
  int l;

  if(type == MAX)
    while((l = k << 1) <= N)
      {
  if(l < N && ORDER_MAX(l+1,l)) ++l;
  if(ORDER_MAX2(k,l))
    break;
  SWAP(k, l);
  k = l;
      }
  else
    while((l = k << 1) <= N)
      {
  if(l < N && ORDER_MIN(l+1,l)) ++l;
  if(ORDER_MIN2(k,l))
    break;
  SWAP(k, l);
  k = l;
      }
}


void
Neighborhood::print_neighborhood()
{
  otbMsgDevMacro( << "pNeighborhood : " );
  otbMsgDevMacro( << " iNbPoints : " <<  iNbPoints );
  for(int i= 0 ; i<=iNbPoints ; i++)
    {
    otbMsgDevMacro( << "tabPoints["<<i <<"] =" << tabPoints[i].value << " Position: (" <<  tabPoints[i].point.x << " , " <<  tabPoints[i].point.y << ")" );
    }

}



}
#endif

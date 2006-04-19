/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Role      :  Definition de la classe TreeSource 
  $Id$ 

=========================================================================*/
 

#ifndef __otbTreeNeighborhood_txx
#define __otbTreeNeighborhood_txx

#include "otbTreeNeighborhood.h"

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
  std::cout << "pNeighborhood : " << std::endl;
  std::cout << " iNbPoints : " <<  iNbPoints << std::endl;
  for(int i= 0 ; i<=iNbPoints ; i++)
    {
    std::cout << "tabPoints["<<i <<"] =" << tabPoints[i].value ;
    std::cout << " Position: (" <<  tabPoints[i].point.x;
    std::cout << " , " <<  tabPoints[i].point.y << ")" ;
    std::cout << std::endl;
    }
  
}



}
#endif

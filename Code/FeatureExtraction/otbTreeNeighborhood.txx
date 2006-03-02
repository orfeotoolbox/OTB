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

TreeNeighborhood::TreeNeighborhood()
{
  m_Neighborhood->iNbPoints = 0; 
}

void TreeNeighborhood::ReInit(TypeOfTree type)
{
  m_Neighborhood->iNbPoints = 0;
  m_Neighborhood->type      = type;
}

void TreeNeighborhood::Init(int iMaxArea)
{
#if 0
  iMaxArea = 4*(iMaxArea+1); 
  if(iMaxArea > iWidth*iHeight)
    iMaxArea = iWidth*iHeight;
  m_Neighborhood->tabPoints.resize((iMaxArea+1));
  
  if(m_Neighborhood->tabPoints == NULL)
    std::cerr <<"init_neighborhood --> neighbors allocation error" <<std::endl;
  
  this->ReInit(AMBIGUOUS);
#endif
}

void TreeNeighborhood::Init(int iMaxArea,int iWidth,int iHeight)
{
#if 0
  iMaxArea = 4*(iMaxArea+1);
 
  m_Neighborhood->tabPoints.resize((iMaxArea+1));
  
  if(m_Neighborhood->tabPoints == NULL)
    std::cerr <<"init_neighborhood --> neighbors allocation error" <<std::endl;
  
  this->ReInit(AMBIGUOUS);
#endif
}

void TreeNeighborhood::Free()
{
  m_Neighborhood->tabPoints.clear();
}

 
const int TreeNeighborhood::ORDER_MAX(int k,int l)
{
  return (m_Neighborhood->tabPoints[k].value > m_Neighborhood->tabPoints[l].value);
}

const int TreeNeighborhood::ORDER_MIN(int k,int l)
{
  return (m_Neighborhood->tabPoints[k].value < m_Neighborhood->tabPoints[l].value);
}

const void TreeNeighborhood::SWAP(int k,int l)
{
  m_Neighborhood->tabPoints[0] = m_Neighborhood->tabPoints[k];
  m_Neighborhood->tabPoints[k] = m_Neighborhood->tabPoints[l];
  m_Neighborhood->tabPoints[l] = m_Neighborhood->tabPoints[0];
}

const int TreeNeighborhood::ORDER_MAX2(int k,int l)
{
  return (m_Neighborhood->tabPoints[k].value >= m_Neighborhood->tabPoints[l].value);
}

const int TreeNeighborhood::ORDER_MIN2(int k,int l)
{
 return (m_Neighborhood->tabPoints[k].value <= m_Neighborhood->tabPoints[l].value);
} 

void TreeNeighborhood::FixUp()
{
#if 0
  int k,l;
  k = m_Neighborhood->iNbPoints;
  if (m_Neighborhood->type == MAX)
    while(k>1 && this->ORDER_MAX(k,l=k>>1))
    {
    this->SWAP(k,l);
    k = l;
    }
   else
    while(k>1 && this->ORDER_MIN(k,l=k>>1))
    {
    this->SWAP(k,l);
    k=l;
    }
#endif
}

void TreeNeighborhood::FixDown()
{
#if 0
  int k,l,N;
  N = m_Neighborhood->iNbPoints;
  k = 1;
  if (m_Neighborhood->type == MAX)
     while( (l = k << 1) <= N)
      {
	if(l < N && this->ORDER_MAX(l+1,l)) ++l;
	if(this->ORDER_MAX2(k,l))
	  break;
	this->SWAP(k, l);
	k = l;
      }
  else
    while((l = k << 1) <= N)
      {
	if(l < N && this->ORDER_MIN(l+1,l)) ++l;
	if(this->ORDER_MIN2(k,l))
	  break;
	this->SWAP(k, l);
	k = l;
      }
#endif
}

void TreeNeighborhood::Add(int x, int y,float value)
{
#if 0
  if(m_Neighborhood->iNbPoints == 0)
    m_Neighborhood->otherBound = value;
  else
    switch(m_Neighborhood->type)
      {
      case MIN:
	if(value > m_Neighborhood->otherBound)
	  m_Neighborhood->otherBound = value;
	else if(value < m_Neighborhood->tabPoints[1].value)
	  m_Neighborhood->type = INVALID;
	break;
      case MAX:
	if(value < m_Neighborhood->otherBound)
	  m_Neighborhood->otherBound = value;
	else if(value > m_Neighborhood->tabPoints[1].value)
	  m_Neighborhood->type = INVALID;
	break;
      case AMBIGUOUS:
	if(value != m_Neighborhood->tabPoints[1].value) {
	  m_Neighborhood->type = (value < m_Neighborhood->tabPoints[1].value)? MAX: MIN;
	  m_Neighborhood->otherBound = value;
	}
	break;
      }
  if(m_Neighborhood->type == INVALID)
    return;
  /* 2) Add the point in the heap and update it */
  m_Neighborhood->iNbPoints++;
  m_Neighborhood->tabPoints[m_Neighborhood->iNbPoints].x = x;
  m_Neighborhood->tabPoints[m_Neighborhood->iNbPoints].x = y;
  m_Neighborhood->tabPoints[m_Neighborhood->iNbPoints].value = value;
  
  this->FixUp(); /* Update the heap of neighbors */
#endif
}

void TreeNeighborhood::Remove()
{
#if 0
  float value,valueTop;
  value = m_Neighborhood->tabPoints[1].value;
  
  if(m_Neighborhood->type == INVALID)
    return;
  valueTop = m_Neighborhood->tabPoints[m_Neighborhood->iNbPoints--].value;
  if(m_Neighborhood->iNbPoints == 0)
    return;
  this->FixDown();
  if(value != valueTop && valueTop == m_Neighborhood->otherBound)
    m_Neighborhood->type = AMBIGUOUS;
#endif
}


void TreeNeighborhood
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif

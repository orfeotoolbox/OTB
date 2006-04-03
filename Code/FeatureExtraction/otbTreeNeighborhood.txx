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
  m_NbPoints = 0; 
  m_tabPoints.clear();
}

void TreeNeighborhood::ReInit(TypeOfTree type)
{
  m_NbPoints = 0;
  m_type     = type;
}

void TreeNeighborhood::Init(int iMaxArea,int iWidth,int iHeight)
{

  iMaxArea = 4*(iMaxArea+1); 
  if(iMaxArea > iWidth*iHeight)
    iMaxArea = iWidth*iHeight;
    
  m_tabPoints.resize((iMaxArea+1));
  
  if(m_tabPoints.empty())
    std::cerr <<"init_neighborhood --> neighbors allocation error" <<std::endl;
  
  this->ReInit(AMBIGUOUS);
}

void TreeNeighborhood::Init(int iMaxArea)
{

  iMaxArea = 4*(iMaxArea+1);
 
  m_tabPoints.resize((iMaxArea+1));
  
  if(m_tabPoints.empty())
    std::cerr <<"init_neighborhood --> neighbors allocation error" <<std::endl;
  
  this->ReInit(AMBIGUOUS);

}

void TreeNeighborhood::Free()
{
  m_tabPoints.clear();
}

 
const int TreeNeighborhood::ORDER_MAX(int k,int l)
{
  return (m_tabPoints[k].value > m_tabPoints[l].value);
}

const int TreeNeighborhood::ORDER_MIN(int k,int l)
{
  return (m_tabPoints[k].value < m_tabPoints[l].value);
}

const void TreeNeighborhood::SWAP(int k,int l)
{
  m_tabPoints[0] = m_tabPoints[k];
  m_tabPoints[k] = m_tabPoints[l];
  m_tabPoints[l] = m_tabPoints[0];
//  m_tabPoints[k].swap(m_tabPoints[l]); 
}

const int TreeNeighborhood::ORDER_MAX2(int k,int l)
{
  return (m_tabPoints[k].value >= m_tabPoints[l].value);
}

const int TreeNeighborhood::ORDER_MIN2(int k,int l)
{
 return (m_tabPoints[k].value <= m_tabPoints[l].value);
} 

void TreeNeighborhood::FixUp()
{

  int k,l;
  k = m_NbPoints;
  if (m_type == MAX)
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

}

void TreeNeighborhood::FixDown()
{

  int k,l,N;
  N = m_NbPoints;
  k = 1;
  if (m_type == MAX)
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

}

void TreeNeighborhood::Add(int x, int y,float value)
{

  if(m_NbPoints == 0)
    m_otherBound = value;
  else
    switch(m_type)
      {
      case MIN:
	if(value > m_otherBound)
	  m_otherBound = value;
	else if(value < m_tabPoints[1].value)
	  m_type = INVALID;
	break;
      case MAX:
	if(value < m_otherBound)
	  m_otherBound = value;
	else if(value > m_tabPoints[1].value)
	  m_type = INVALID;
	break;
      case AMBIGUOUS:
	if(value != m_tabPoints[1].value) {
	  m_type = (value < m_tabPoints[1].value)? MAX: MIN;
	  m_otherBound = value;
	}
	break;
      }
  if(m_type == INVALID)
    return;
  /* 2) Add the point in the heap and update it */
  m_NbPoints++;

//  m_tabPoints.resize(m_NbPoints+1);

//  PointType PointToInsert;
//  PointToInsert.x     = x;
//  PointToInsert.y     = y;
//  PointToInsert.value = value ;
//  m_tabPoints.push_back(PointToInsert);

  std::cout<< "TreeNeighborhood::Add : m_NbPoints" << m_NbPoints <<  "  m_tabPoints.size() = "<<m_tabPoints.size()<<std::endl;
//  assert(m_NbPoints <=m_Width*m_Height);   
  assert(m_NbPoints < m_tabPoints.size());
  m_tabPoints[m_NbPoints].x     = x;
  m_tabPoints[m_NbPoints].y     = y;
  m_tabPoints[m_NbPoints].value = value;
  
  this->FixUp(); /* Update the heap of neighbors */

}

void TreeNeighborhood::Remove()
{

  float value;
  float valueTop;

  value = m_tabPoints[1].value;
  
  if(m_type == INVALID)
    return;
//  m_NbPoints--; 
  std::cout << "TreeNeighborhood::Remove : m_NbPoints" << m_NbPoints;
  std::cout << "  m_tabPoints.size() = "<<m_tabPoints.size();
  std::cout << " value "<<value << std::endl;
  
  assert(m_NbPoints < (int)m_tabPoints.size());
//  assert(m_NbPoints >= (int)0);
  valueTop = m_tabPoints[m_NbPoints--].value;
  if(m_NbPoints <= 0)
    return;
  this->FixDown();
  if(value != valueTop && valueTop == m_otherBound)
    m_type = AMBIGUOUS;

}

}
#endif

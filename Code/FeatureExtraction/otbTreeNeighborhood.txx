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
  m_tabPoints.clear();
  m_iNbPoints = 0;
  m_type = AMBIGUOUS;
  m_otherBound = 0.0;
}

void TreeNeighborhood::SetSize(int Taille)
{
  assert( Taille > 0 );
  m_tabPoints.resize(Taille); 
}
  
const int TreeNeighborhood::ORDER_MAX(int k,int l)
{
  int Taille = m_tabPoints.size();
  assert( k > 0);
  assert( l > 0);
  assert( k<= Taille);
  assert( l>= Taille);
  return (m_tabPoints[k].Value > m_tabPoints[l].Value);
}

const int TreeNeighborhood::ORDER_MIN(int k,int l)
{
  int Taille = m_tabPoints.size();
  assert( k > 0);
  assert( l > 0);
  assert( k<= Taille);
  assert( l>= Taille);
  return (m_tabPoints[k].Value < m_tabPoints[l].Value);
}

const void TreeNeighborhood::SWAP(int k,int l)
{
  int Taille = m_tabPoints.size();
  assert( k > 0);
  assert( l > 0);
  assert( k<= Taille);
  assert( l>= Taille);
  m_tabPoints[0] = m_tabPoints[k];
  m_tabPoints[k] = m_tabPoints[l];
  m_tabPoints[l] = m_tabPoints[0];
}

const int TreeNeighborhood::ORDER_MAX2(int k,int l)
{
  int Taille = m_tabPoints.size();
  assert( k > 0);
  assert( l > 0);
  assert( k<= Taille);
  assert( l>= Taille);
  return (m_tabPoints[k].Value >= m_tabPoints[l].Value);

}

const int TreeNeighborhood::ORDER_MIN2(int k,int l)
{
  int Taille = m_tabPoints.size();
  assert( k > 0);
  assert( l > 0);
  assert( k<= Taille);
  assert( l>= Taille);
  return (m_tabPoints[k].Value <= m_tabPoints[l].Value);
} 

void TreeNeighborhood::FixUp()
{
  int k,l;
  k = m_iNbPoints;
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
  N = m_iNbPoints;
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
  if(m_iNbPoints == 0)
    m_otherBound = value;
  else
    switch(m_type)
      {
      case MIN:
	if(value > m_otherBound)
	  m_otherBound = value;
	else if(value < m_tabPoints[1].Value)
	  m_type = INVALID;
	break;
      case MAX:
	if(value < m_otherBound)
	  m_otherBound = value;
	else if(value > m_tabPoints[1].Value)
	  m_type = INVALID;
	break;
      case AMBIGUOUS:
	if(value != m_tabPoints[1].Value) {
	  m_type = (value < m_tabPoints[1].Value)? MAX: MIN;
	  m_otherBound = value;
	}
	break;
      }
  if(m_type == INVALID)
    return;
  /* 2) Add the point in the heap and update it */
  ++m_iNbPoints;
  m_tabPoints[m_iNbPoints].x = x;
  m_tabPoints[m_iNbPoints].x = y;
  m_tabPoints[m_iNbPoints].Value = value;
  
  this->FixUp(); /* Update the heap of neighbors */
}

void TreeNeighborhood::Remove()
{
  int Taille = m_tabPoints.size();
  assert(Taille >0);
  assert(Taille = m_iNbPoints);
  float value,valueTop;
  value = m_tabPoints[1].Value;
  
  if(m_type == INVALID)
    return;
  valueTop = m_tabPoints[m_iNbPoints--].Value;
  if(m_iNbPoints == 0)
    return;
  this->FixDown();
  if(value != valueTop && valueTop == m_otherBound)
    m_type = AMBIGUOUS;
}


void TreeNeighborhood
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif

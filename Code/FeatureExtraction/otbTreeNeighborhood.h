/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Role      :  Definition de la classe TreeNeighborhood
  $Id$ 

=========================================================================*/
 
#ifndef __otbTreeNeighborhood_h
#define __otbTreeNeighborhood_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

namespace otb
{

/** \class TreeNeighborhood
 *  \brief Classe de base pour tous les objets de type "TreeNeighborhood".
 *
 * ....
 *
 * \ingroup TreeNeighborhoodObjects
 */
 
class TreeNeighborhood 
{
public:
    
  typedef enum  {AMBIGUOUS, MIN, MAX, INVALID} TypeOfTree;
  
  
  typedef struct {
	int                        x;
	int                        y;
	float                      value;
  } PointType;
  
  typedef std::vector<PointType>     NeighborType;
  typedef TreeNeighborhood           TreeNeighborhoodType;
  void Init(int iMaxArea);
  void Init(int iMaxArea,int iWidth,int iHeight);
  void ReInit(TypeOfTree type);
  void Add(int x, int y,float value);
  void Remove();

  TreeNeighborhood();
  virtual ~TreeNeighborhood() {m_tabPoints.clear();}
  void Free();
  
  
  void SetSize(int Taille);
  const int ORDER_MAX(int k,int l);
  const int ORDER_MIN(int k,int l);
  const void SWAP(int k,int l);
  const int ORDER_MAX2(int k,int l);
  const int ORDER_MIN2(int k,int l); 
  void FixUp();
  void FixDown();

  NeighborType  m_tabPoints;
  int           m_NbPoints;
  TypeOfTree    m_type;
  float         m_otherBound;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTreeNeighborhood.txx"
#endif

#endif
  

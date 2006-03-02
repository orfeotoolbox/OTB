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

#include "itkDataObject.h"
#include "itkPoint.h"
#include <vector>

namespace otb
{

/** \class TreeNeighborhood
 *  \brief Classe de base pour tous les objets de type "TreeNeighborhood".
 *
 * ....
 *
 * \ingroup TreeNeighborhoodObjects
 */
 
class TreeNeighborhood : public itk::DataObject
{
public:
  /** Standard class typedefs. */
  typedef TreeNeighborhood                       Self;
  typedef itk::DataObject                        Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(TreeSource,itk::FunctionBase);
    
  typedef enum  {AMBIGUOUS, MIN, MAX, INVALID} TypeOfTree;
  
  
  typedef struct {
	int                        x;
	int                        y;
	float                      value;
  } PointType;
  
  typedef std::vector<PointType>     NeighborType;
  
  typedef struct {
        NeighborType  tabPoints;
        int           iNbPoints;
	TypeOfTree    type;
	float         otherBound;
   } NeighborhoodType;
       
protected:
  TreeNeighborhood();
  virtual ~TreeNeighborhood() {}
  void ReInit(TypeOfTree type);
  void Init(int iMaxArea);
  void Init(int iMaxArea,int iWidth,int iHeight);
  void Free();
  
  
  void SetSize(int Taille);
  const int ORDER_MAX(int k,int l);
  const int ORDER_MIN(int k,int l);
  const void SWAP(int k,int l);
  const int ORDER_MAX2(int k,int l);
  const int ORDER_MIN2(int k,int l); 
  void FixUp();
  void FixDown();
  void Add(int x, int y,float value);
  void Remove();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

      
private:
  TreeNeighborhood(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  NeighborhoodType *m_Neighborhood;
  };

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTreeNeighborhood.txx"
#endif

#endif
  

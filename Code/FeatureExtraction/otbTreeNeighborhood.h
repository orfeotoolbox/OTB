/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Role      :  Definition de la classe TreeSource 
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
 
class otbTreeNeighborhood : public itk::DataObject
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
    
  enum TypeOfTree {AMBIGUOUS, MIN, MAX, INVALID};
  
  itk::Point<float,2>        PointType;
  std::vector<PointType>     PointListType;      
protected:
  TreeNeighborhood();
  virtual ~TreeNeighborhood() {m_tabPoints.clear();}
  void SetSize(int Taille);
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  enum TypeOfTree {AMBIGUOUS, MIN, MAX, INVALID};
      
private:
  TreeNeighborhood(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  PointListType m_tabPoints;      /*** The array of neighbors, organized as a binary tree  */
  int           m_iNbPoints;      /*** The size of the previous arrays                     */
  TypeOfTree    m_type;           /*** max- or min- oriented heap?                         */
  float         m_otherBound;     /*** Min gray level if max-oriented, max if min-oriented */
  };

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTreeNeighborhood.txx"
#endif

#endif
  

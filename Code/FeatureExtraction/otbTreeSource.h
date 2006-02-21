/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   20 fevrier 2006
  Role      :  Definition de la classe TreeSource 
  $Id$ 

=========================================================================*/
 
#ifndef __otbTreeSource_h
#define __otbTreeSource_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkProcessObject.h"
#include "otbTreeNeighborhood.h"

namespace otb
{

/** \class TreeSource
 *  \brief Classe de base pour tous les objets ayant en sortie des données du type "Tree".
 *
 * Cette classe définit la méthode du GetOutput qui retourne un pointeur des données en sortie de Tree.
 * Elle définit aussi des membres privées de la classe afin de gérer les données.
 *
 * \ingroup TreeSources
 */
template <class TOutputTree>
class TreeSource : public itk::ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef TreeSource                             Self;
  typedef itk::ProcessObject                     Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(TreeSource,itk::ProcessObject);
  
  typedef TreeNeighborhood                        OutputTreeType;
  typedef typename OutputTreeType::Pointer        OutputTreePointerType;
  typedef typename OutputTreeType::ConstPointer   OutputTreeConstPointerType;
  typedef OutputTreeType                          OutputTreeListType;
  
  /** Get the tree output of this process object.  */
  OutputTreeListType * GetOutput(void)
  {
  	OutputTreeListType * ptr = ( & (this->m_TreeList) );
	return (ptr);
  }

    
protected:
  TreeSource();
  virtual ~TreeSource() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
      
private:
  TreeSource(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputTreeListType m_TreeList;
  
  };

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTreeSource.txx"
#endif

#endif
  

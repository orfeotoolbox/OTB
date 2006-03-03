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
#include "itkTreeContainer.h"
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

  typedef TOutputTree				  PathType; 
  typedef typename PathType::IndexType            PathIndexType;
  typedef typename PathType::Pointer              PathPointerType;    

  typedef itk::TreeContainer< PathType >          OutputTreeType;
  typedef typename OutputTreeType::Pointer        OutputTreePointerType;
  typedef typename OutputTreeType::ConstPointer   OutputTreeConstPointerType;

  typedef struct 
    {
    int x;
    int y;
    } PointPlaneType;
    
  typedef typename std::vector<PointPlaneType>   PointPlaneListType;
    
  typedef struct shape
    {
    char                   inferior_type;
    float                  value;
    char                   open;
    int                    area;
    char                   removed;
    PointPlaneListType     pixels;
    PathPointerType        boundary;
    shape                  *parent;
    shape                  *child;
    shape                  *next_sibling;
    } ShapeType;

  typedef ShapeType*      	ShapePointerType;
  typedef const ShapeType*      ShapeConstPointerType;

  typedef struct 
    {
    int              nrow;
    int              ncol;
    int              interpolation;
    int              nb_shapes;
    ShapeType        *the_shapes;
    ShapeType        **smallest_shape;
    } ShapeTreeType;
   
  typedef ShapeTreeType*       ShapeTreePointerType;    
  typedef const ShapeTreeType* ShapeTreeConstPointerType;    

  /** Get the tree output of this process object.  */
  OutputTreePointerType GetOutput(){ };

  void AllocateShapeTree(int nrow,int ncol,float value);
  void DeAllocateShapeTree();
    
protected:
  TreeSource();
  virtual ~TreeSource() {}
    

  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  
// ATTENTION : g�rer ce ouptut comme un Output ITK (SetNthOuput(0...) pour b�n�ficier du pipeline, etc...
  ShapeTreePointerType      m_GlobalTree;
      
private:
  TreeSource(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  virtual void TranslateTreeShapeToOutputTree( ShapeTreeConstPointerType  Shapes, 
		   			       OutputTreePointerType      TreeContainer );

  
  };

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTreeSource.txx"
#endif

#endif
  

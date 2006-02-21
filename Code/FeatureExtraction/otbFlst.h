/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - P.Imbo
  Language  :   C++
  Date      :   24 janvier 2006
  Role      :    
  $Id$ 

=========================================================================*/
#ifndef __otbFlst_h
#define __otbFlst_h
 
#include "otbImageToTreeFilter.h"
#include "otbTreeNeighborhood.h"

namespace otb
{

/** \class FLST
 * \brief Algorithme de Fast Level Sets Transform of an image
 *
 */
 
 
template <class TInputImage, class TOutputTree>
class FLST :  public ImageToTreeFilter< TInputImage, TOutputTree >
{
public:
  /** Standard class typedefs. */
  typedef FLST                                         Self;
  typedef ImageToTreeFilter<TInputImage,TOutputTree>   Superclass;
  typedef itk::SmartPointer<Self>                      Pointer;
  typedef itk::SmartPointer<const Self>                ConstPointer;

  /** Methode pour la gestion "object factory". */
  itkNewMacro(Self);

  /** Retourne le nom de la classe. */
  itkTypeMacro(FLST, ImageToTreeFilter);
  
  itkStaticConstMacro(InputImageDimension,
  		      unsigned int,
                      TInputImage::ImageDimension);

  /** "typedef" pour simplifier la définition et la déclaration de variables. */
  
  typedef typename Superclass::InputImageType        InputImageType;
  typedef typename Superclass::InputImagePointer     InputImagePointer;
  typedef typename Superclass::InputImageRegionType  InputImageRegionType; 
  typedef typename Superclass::InputImagePixelType   InputImagePixelType; 
  typedef typename Superclass::InputImageSizeType    InputImageSizeType;                 
  typedef typename Superclass::InputImageValueType   InputImageValueType;

  typedef typename Superclass::OutputTreeType         OutputTreeType;
  typedef typename Superclass::OutputTreeListType     OutputTreeListType;
  typedef typename Superclass::OutputTreePointerType  OutputTreePointerType;
  

protected:
  FLST();
  virtual ~FLST() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

 /** Définition des paramètres d'optimisation. 
 * Ils sont dépendant de la taille des images mais ces valeurs semblent 
 * convenir à la majoritée des images. */
  static const int INIT_MAX_AREA; 
  static const int STEP_MAX_AREA;
  
  /** configuration locale des pixels */
  static const int EAST; 
  static const int NORTH_EAST;
  static const int NORTH;
  static const int NORTH_WEST;
  static const int WEST;
  static const int SOUTH_WEST;
  static const int SOUTH;
  static const int SOUTH_EAST;    
  
  static const char tabPatterns[2][256];
  
  itkSetMacro(MinArea, int);
  itkGetConstReferenceMacro(MinArea, int);
  
  void CalculateArea(int Width, int Height);
  void GenerateData(void);
  
private:
  FLST(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  int m_MinArea;
  int m_AreaImage;
  int m_HalfAreaImage;
  int m_PerimeterImage;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFlst.txx"
#endif

  
#endif

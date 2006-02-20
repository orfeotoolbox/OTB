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

/** Définition des paramètres d'optimisation. 
 * Ils sont dépendant de la taille des images mais ces valeurs semblent 
 * convenir à la majoritée des images. */
#define INIT_MAX_AREA 10
#define STEP_MAX_AREA 8

/* A 'local configuration' of the pixel is the logical 'or' of these values,
stored in one byte. Corresponding bit is set if the neighbor is in region */
#define EAST         1
#define NORTH_EAST   2
#define NORTH        4
#define NORTH_WEST   8
#define WEST        16
#define SOUTH_WEST  32
#define SOUTH       64
#define SOUTH_EAST 128
 
 
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

  void GenerateData(void);

private:
  FLST(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFLST.txx"
#endif

  
#endif

/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   09 mars 2006
  Role      :   Filtre de detection de contours 
  $Id$ 

=========================================================================*/
#ifndef __otbTouziEdgeDetector_h
#define __otbTouziEdgeDetector_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

#define MIN(_A,_B) ((_A) < (_B) ? (_A) : (_B))   
#define MAX(_A,_B) ((_A) > (_B) ? (_A) : (_B))


namespace otb
{

/** \class TouziEdgeDetector
 * \brief Applique un filtre de d\'etection de contours.
 *
 * On d\'efinit une fen\^etre carr\'ee de taille 2n+1.  
 * On calcule la moyenne pour chaque demi fen\^etre situ\�e de part
 * et d'autre du contour dans une direction donn\'ee (\mu_{1} et \mu_{2})
 * L'intensit\'e du contour est alors calcul\'e dans les quatre
 * directions verticale (\theta_{i}), horizontale (\theta_{i}), diagonale1 (\theta_{i}) 
 * et diagonale2 (\theta_{i}):
 *	R(\theta_{i}) = 1 - min(\fract{\mu_{1}/\mu_{2}};\fract{\mu_{2}/\mu_{1}})
 *
 * On obtient une image de l'intensit� des contours en prenant :
 *	R = max ( (\theta_{i}), (\theta_{i}), (\theta_{i}), (\theta_{i}))
 *
 * Une m\'ethode permet \'egalement de calculer une image des directions:
 * 	D =  
 * 
 */

template <class TInputImage, class TOutputImage>
class TouziEdgeDetector :  public itk::ImageToImageFilter< TInputImage, TOutputImage >
{
public:
  /** 	Extrait les dimensions aussi bien des images 
  	d'entrée (Input) que de sortie (Output). */
  itkStaticConstMacro(		InputImageDimension,
  				unsigned int,
                      		TInputImage::ImageDimension);
  itkStaticConstMacro(		OutputImageDimension, 
  				unsigned int,
                      		TOutputImage::ImageDimension);

  /** "typedef" pour simplifier la définition et la déclaration de variables. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;

  /** "typedef" pour les classes standards. */
  typedef TouziEdgeDetector Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Methode pour la gestion "object factory". */
  itkNewMacro(Self);

  /** Retourne le nom de la classe. */
  itkTypeMacro(TouziEdgeDetector, ImageToImageFilter);
  
  /** Définition des images supportées. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** "typedef" définissant la taille d'une image. */
  typedef typename InputImageType::SizeType SizeType;

  /** Positionne le rayon définissant le voisinage utilisé pour le calcul du filtre. */
  itkSetMacro(Radius, SizeType);

  /** Récupère le rayon définissant le voisinage utilisé pour le calcul du filtre. */
  itkGetConstReferenceMacro(Radius, SizeType);
 

  /** TouzyEdgeDetector a besoin d'une zone de traitement plus large en entrée qu'en sortie 
   * afin de permettre une utilisation du filtre par la méthode dite pipeline
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

  const OutputImageType * GetOutputDirection();
  
protected:
  TouziEdgeDetector();
  virtual ~TouziEdgeDetector() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void BeforeThreadedGenerateData();

  /** TouziEdgeDetector peut etre implémentée pour un traitement de filtre multithreaded.
   * Ainsi, cette implémentation fournit la méthode ThreadedGenerateData()
   * qui est appelée pour chaque thread du process. Les données image sont allouées automatiquement 
   * par la classe "mère" en appelant la méthode ThreadedGenerateData(). ThreadedGenerateData peut seulement 
   * écrire la portion de l'image spécifiée par le paramètre "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  TouziEdgeDetector(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Déclaration du rayon */
  SizeType m_Radius;
  
  typename OutputImageType::Pointer m_DirectionOuputImage;
  

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTouziEdgeDetector.txx"
#endif

  
#endif

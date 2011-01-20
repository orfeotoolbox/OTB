/*=========================================================================

  Program:   OTB
  Module:    $otbMuellerToPolarisationDegreeAndPowerImageFilter.h$
  Language:  C++
  Date:      $Date: 24/10/2006 $
  Version:   $Revision: 1.0 $

 

=========================================================================*/
#ifndef __MuellerToPolarisationDegreeAndPowerImageFilter_h
#define __MuellerToPolarisationDegreeAndPowerImageFilter_h

#include "math.h"

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkVector.h"

#include "otbImage.h"

/** Déclaration des constantes */

#ifndef PI
#define PI 3.14159265358979323846
#endif

namespace otb
 {
/** \class otbMuellerToPolarisationDegreeAndPowerImageFilter
 * \brief Compute the Circular Polarisation image (3 channels : LL, RR and LR) from the Mueller Matrix image (10 channels)
 */
template <class TPixel>
class  MuellerToPolarisationDegreeAndPowerImageFilter :
   public itk::ImageToImageFilter< otb::Image<itk::Vector<TPixel,10>,2>, otb::Image<itk::Vector<TPixel,4>,2> >
{
public:
  /** Convenient typedefs for simplifying declarations. */
  typedef TPixel InputPixelType;
  typedef otb::Image<itk::Vector<TPixel,10>,2> InputImageType;//L'image de départ est de type Mueller
  typedef otb::Image<itk::Vector<TPixel,4>,2> OutputImageType;// L'image d'arrivée contient les Pmin, Pmax, deg_pol_min, deg_pol_max
  typedef otb::Image<itk::Vector<TPixel,10>,2> MuellerImageType;

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

  
  /** Standard class typedefs. */
  typedef MuellerToPolarisationDegreeAndPowerImageFilter Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MuellerToPolarisationDegreeAndPowerImageFilter, ImageToImageFilter);
  
  /** Image typedef support. */
  typedef typename InputImageType::PixelType InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  
  typedef typename InputImageType::RegionType InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename InputImageType::SizeType InputSizeType;
  typedef typename InputImageType::IndexType IndexType ;

 
  /** Mueller type **/
  typedef itk::Vector<float,10> MuellerType; 

  /**CircularType **/
  typedef itk::Vector<float, 4> DegreeAndPowerType; //va contenir Pmin, Pmax, deg_pol_min, deg_pol_max dans cet ordre

  /**StokesVectorType **/
  typedef itk::Vector<float, 4> StokesVectorType; //Vecteur de stokes définissant une polarisation
  
  
  /** MuellerToPolarisationDegreeAndPowerImageFilter needs a larger input requested region than
   * the output requested region.  As such, MuellerToPolarisationDegreeAndPowerImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

protected:
  MuellerToPolarisationDegreeAndPowerImageFilter();
  virtual ~MuellerToPolarisationDegreeAndPowerImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** MuellerToPolarisationDegreeAndPowerImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  MuellerToPolarisationDegreeAndPowerImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

 
 

};
  
} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbMuellerToPolarisationDegreeAndPowerImageFilter.txx"
#endif

#endif

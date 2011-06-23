/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbMatrixImageFilter_h
#define __otbMatrixImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbMath.h"

namespace otb
{

/** \class MatrixImageFilter
 * \brief Apply a transition matrix over the channel of an image.
 *
 * The templates are the input and output image type.
 * The transition matrix is given using the SetTransitionMatrix() method. The waiting type is a vnl_matrix<double> (ie. MatrixType).
 * The number of rows of the matrix must be the input image number of channels, the number of columns is the number of channels of the output image.
 *
 * For example, if the image has 2 bands, the matrix is \f$ \begin{pmatrix} \alpha & \beta \\ \gama & \delta \end{pmatrix} \f$
 * The pixel \f$ [a, b] \f$ will give the output pixel \f$ [\alpha.a + \beta.b, \gamma.a + \delta.b  ]. \f$
 *
 */

template <class TInputImage, class TOutputImage, class TMatrix = vnl_matrix<typename itk::NumericTraits<typename TInputImage::InternalPixelType>::RealType> >
class ITK_EXPORT MatrixImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Extract input and output images sizes. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedef to simplify variables definition and declaration. */
  typedef TInputImage InputImageType;
  /** typedef to simplify variables definition and declaration. */
  typedef TOutputImage OutputImageType;

  /** typedef for standard classes. */
  typedef MatrixImageFilter                                         Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(MatrixImageFilter, ImageToImageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename InputImageType::InternalPixelType  InputInternalPixelType;
  typedef typename OutputImageType::InternalPixelType OutputInternalPixelType;

  /** MatrixType definition */
  // To support complexe...
  typedef typename itk::NumericTraits<InputInternalPixelType>::RealType InputRealType;
  typedef TMatrix MatrixType;
  typedef vnl_vector<InputRealType> VectorType;

  /** "typedef" to define a real. */
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** "typedef" to define an image size. */
  typedef typename InputImageType::SizeType SizeType;


  /** Transition matrix accessors. */
  void SetMatrix( MatrixType mat)
    {
      m_Matrix = mat;
      this->Modified();
    }
  MatrixType GetMatrix()
    {
     return  m_Matrix;
    }
 
protected:
  MatrixImageFilter();
  virtual ~MatrixImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /**
   * Before threaded generate data method.
   */
  void GenerateOutputInformation();

  /** MatrixImageFilter can be implemented for a multithreaded filter treatment.
   * Thus, this implementation give the ThreadedGenerateData() method.
   * that is called for each process thread. Image datas are automatically allocated
   * throught the parent class calling the ThreadedGenerateData() method.
   * ThreadedGenerateData() can only write the area of the image specified by the parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId);

private:
  MatrixImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Radius declaration */
  //SizeType m_Radius;
  /** Matrix declaration */
  MatrixType m_Matrix;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMatrixImageFilter.txx"
#endif

#endif

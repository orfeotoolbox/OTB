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
#ifndef __otbStreamingWarpImageFilter_h
#define __otbStreamingWarpImageFilter_h

#include "itkWarpImageFilter.h"
#include "otbStreamingTraits.h"

namespace otb
{

/** \class StreamingWarpImageFilter
 * \brief 
 *
 * \TODO: Document
 */

template <class TInputImage, class TOutputImage, class TDeformationField>
class ITK_EXPORT StreamingWarpImageFilter 
  :  public itk::WarpImageFilter< TInputImage, TOutputImage, TDeformationField >
{
public:

  /** Standard class typedefs. */
  typedef StreamingWarpImageFilter                                            Self;
  typedef itk::WarpImageFilter< TInputImage, TOutputImage, TDeformationField> Superclass;
  typedef itk::SmartPointer<Self>                                             Pointer;
  typedef itk::SmartPointer<const Self>                                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

   /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingWarpImageFilter, itk::WarpImageFilter);
  
  /** template parameters typedef */
  typedef TInputImage                              InputImageType;
  typedef typename  InputImageType::Pointer        InputImagePointerType;
  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointerType;
  typedef TDeformationField                        DeformationFieldType;
  typedef typename DeformationFieldType::PixelType DeformationValueType;
  typedef typename DeformationFieldType::Pointer   DeformationFieldPointerType;

  /** Accessors */
  itkSetMacro(MaximumDeformation,DeformationValueType);
  itkGetConstReferenceMacro(MaximumDeformation,DeformationValueType);
  
protected:
  /** Constructor */
  StreamingWarpImageFilter();
  /** Destructor */
  virtual ~StreamingWarpImageFilter() {};
  /** PrintSelf */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** 
   * This filters requires only a part of the input and of the deformation field to
   * produce its output. As such, we need to overload the GenerateInputRequestedRegion() method.
   */
  virtual void GenerateInputRequestedRegion() ;
	
private:
  StreamingWarpImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
	
  // Assesment of the maximum deformation for streaming
  DeformationValueType m_MaximumDeformation;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingWarpImageFilter.txx"
#endif

  
#endif

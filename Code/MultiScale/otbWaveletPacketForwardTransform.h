/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletPacketForwardTransform_h
#define __otbWaveletPacketForwardTransform_h

#include "otbImageToImageListFilter.h"

namespace otb {

/** \class WaveletPacketForwardTransform
 * \brief Wavelet packet transformation framework
 *
 * This class defines the Wavelet Packet transformation of an image
 * by using a (templated) elementary wavelet transform and (templated)
 * cost criteria to stop the decomposition.
 *
 * It yields a list of images...
 *
 * TODO: this class implements the decomposition only. But there is no storage 
 * of the way of decomposition. It should be added before implementing backward
 * transformation.
 *
 * the user is supposed to initialize Cost properly (through GetCost() macro) 
 * depending on its type before calling an Update(). The Cost class has to contain 
 * a New() and Evaluate() function.
 *
 * \sa FullyDecomposedWaveletPacketCost
 * \sa StationaryFilterBank
 * \sa WaveletForwardTransform
 *
 */
template < class TInputImage, class TOutputImage, class TFilter, class TCost >
class ITK_EXPORT WaveletPacketForwardTransform
  : public ImageToImageListFilter< TInputImage, TOutputImage >
{
public:
  /** Standard typedefs */
  typedef WaveletPacketForwardTransform Self;
  typedef ImageToImageListFilter< TInputImage, TOutputImage > Superclass;
  typedef itk::SmartPointer<Self> Pointer;
        typedef itk::SmartPointer<const Self> ConstPointer;

        /** Type macro */
        itkNewMacro(Self);

        /** Creation through object factory macro */
        itkTypeMacro(WaveletPacketForwardTransform,ImageToImageListFilter);

  typedef          TInputImage                  InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointerType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename InputImageType::SizeType     SizeType;
  typedef typename InputImageType::ValueType    ValueType;

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename Superclass::OutputImageListType    OutputImageListType;
  typedef typename OutputImageListType::Pointer       OutputImageListPointerType;
  typedef typename OutputImageListType::Iterator      OutputImageIterator;

  typedef TFilter                           FilterType;
  typedef typename FilterType::Pointer      FilterPointerType;
  typedef ObjectList< FilterType >          FilterListType;
  typedef typename FilterListType::Pointer  FilterListPointerType;
  typedef typename FilterListType::Iterator FilterListIterator;

  itkGetObjectMacro(FilterList,FilterListType);

  typedef TCost CostType;
  typedef typename CostType::Pointer CostPointerType;

  itkGetObjectMacro(Cost,CostType);
 
  itkStaticConstMacro(InputImageDimension, unsigned int,TInputImage::ImageDimension);

protected:
  WaveletPacketForwardTransform();
  virtual ~WaveletPacketForwardTransform() {}

  /** Generate data redefinition */
  virtual void GenerateData ();

  /** Performs (if any) the local decomposition (called recursively) */
  virtual void PerformDecomposition ( unsigned int depth, OutputImageIterator & outputIt );

private:
  WaveletPacketForwardTransform ( const Self &);
  void operator= ( const Self & );

  FilterListPointerType m_FilterList;
  CostPointerType m_Cost;

}; // end of class

} // end of namespace

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletPacketForwardTransform.txx"
#endif

#endif



/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbLabelImageToLabelMapWithAdjacencyFilter_h
#define __otbLabelImageToLabelMapWithAdjacencyFilter_h

#include "itkImageToImageFilter.h"
#include "otbLabelMapWithAdjacency.h"
#include "itkLabelObject.h"

namespace otb {

/** \class LabelImageToLabelMapWithAdjacencyFilter
 * \brief convert a labeled image to a label map with adjacency information.
 *
 */
template<class TInputImage, class TOutputImage=otb::LabelMapWithAdjacency< itk::LabelObject< typename TInputImage::PixelType, TInputImage::ImageDimension > > >
class ITK_EXPORT LabelImageToLabelMapWithAdjacencyFilter :
    public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelImageToLabelMapWithAdjacencyFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>
  Superclass;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename InputImageType::Pointer                      InputImagePointer;
  typedef typename InputImageType::ConstPointer                 InputImageConstPointer;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename InputImageType::PixelType                    InputImagePixelType;
  typedef typename InputImageType::IndexType                    IndexType;

  typedef typename OutputImageType::Pointer                     OutputImagePointer;
  typedef typename OutputImageType::ConstPointer                OutputImageConstPointer;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;
  typedef typename OutputImageType::PixelType                   OutputImagePixelType;
  typedef typename OutputImageType::LabelObjectType             LabelObjectType;
  typedef typename OutputImageType::AdjacencyMapType            AdjacencyMapType;
  typedef typename OutputImageType::AdjacentLabelsContainerType AdjacentLabelsContainerType;
  typedef typename OutputImageType::LabelType                   LabelType;

  /** Const iterator over LabelObject lines */
  typedef typename LabelObjectType::ConstLineIterator           ConstLineIteratorType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LabelImageToLabelMapWithAdjacencyFilter,
               ImageToImageFilter);

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetConstMacro(BackgroundValue, OutputImagePixelType);

protected:
  /** Constructor */
  LabelImageToLabelMapWithAdjacencyFilter();
  /** Destructor */
  ~LabelImageToLabelMapWithAdjacencyFilter() {};
  /** Printself */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  // class to store a RLE
  class RLE
  {
  public:
    // Location
    IndexType where;
    // Length
    long unsigned int length;
    // Label
    unsigned long int label;
    // Constructor
    RLE(const IndexType & w, long unsigned int l, unsigned long int la)
    {
      where = w;
      length = l;
      label = la;
    }

  }; // end class RLE

  // RLE Vector
  typedef std::vector<RLE> RLEVectorType;

  /** LabelImageToLabelMapWithAdjacencyFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** LabelImageToLabelMapWithAdjacencyFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(itk::DataObject *itkNotUsed(output));

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  virtual void AfterThreadedGenerateData();

  /** Add a new adjacency */
  void AddAdjacency(LabelType label1, LabelType label2, itk::ThreadIdType threadId);

  /** Parse one line for horizontal adjacency */
  void ParseLine(const RLEVectorType & line, itk::ThreadIdType threadId);

  /** Parse two consecutive lines for vertical adjacency */
  void ParseConsecutiveLines(const RLEVectorType& line1, const RLEVectorType& line2, itk::ThreadIdType threadId);


private:
  LabelImageToLabelMapWithAdjacencyFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImagePixelType m_BackgroundValue;

  typename std::vector< OutputImagePointer > m_TemporaryImages;
  typename std::vector<AdjacencyMapType>     m_TemporaryAdjacencyMaps;

}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelImageToLabelMapWithAdjacencyFilter.txx"
#endif

#endif



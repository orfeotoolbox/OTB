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
#ifndef __otbNumberOfLinesStrippedStreamingManager_h
#define __otbNumberOfLinesStrippedStreamingManager_h

#include "otbStreamingManager.h"
#include "itkImageRegionSplitter.h"

namespace otb
{

/** \class NumberOfLinesStrippedStreamingManager
 *  \brief This class computes the divisions needed to stream an image by strips,
 *  driven by a user-defined desired number of lines per strips.
 *
 * You can use SetNumberOfLinesPerStrip to ask for a specific number of lines per strip.
 * The number of strips will be computed to fit this requirements as close as possible.
 *
 * \sa ImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 */
template<class TImage>
class ITK_EXPORT NumberOfLinesStrippedStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef NumberOfLinesStrippedStreamingManager Self;
  typedef StreamingManager<TImage>              Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(NumberOfLinesStrippedStreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The number of lines per strip desired */
  itkSetMacro(NumberOfLinesPerStrip, unsigned int);

  /** The number of lines per strip desired */
  itkGetMacro(NumberOfLinesPerStrip, unsigned int);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  virtual void PrepareStreaming(itk::DataObject * /*input*/, const RegionType &region);

protected:
  NumberOfLinesStrippedStreamingManager();
  virtual ~NumberOfLinesStrippedStreamingManager();

  /** The splitter type used to generate the different strips */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)> SplitterType;

  /** The number of lines per strip desired by the user.
   *  This may be different than the one computed by the Splitter */
  unsigned int m_NumberOfLinesPerStrip;
private:
  NumberOfLinesStrippedStreamingManager(const NumberOfLinesStrippedStreamingManager &);
  void operator =(const NumberOfLinesStrippedStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNumberOfLinesStrippedStreamingManager.txx"
#endif

#endif


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
#ifndef __otbNumberOfDivisionsStrippedStreamingManager_h
#define __otbNumberOfDivisionsStrippedStreamingManager_h

#include "otbStreamingManager.h"

namespace otb
{

/** \class NumberOfDivisionsStrippedStreamingManager
 *  \brief This class computes the divisions needed to stream an image by strips,
 *  driven by a user-defined desired number of lines per strips.
 *
 * You can use SetNumberOfLinesPerStrip to ask for a specific number of lines per strip.
 * The number of strips will be computed to fit this requirements as close as possible.
 *
 * \sa StreamingImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 */
template<class TImage>
class ITK_EXPORT NumberOfDivisionsStrippedStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef NumberOfDivisionsStrippedStreamingManager Self;
  typedef StreamingManager<TImage>              Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(NumberOfDivisionsStrippedStreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The number of lines per strip desired */
  itkSetMacro(NumberOfDivisions, unsigned int);

  /** The number of lines per strip desired */
  itkGetMacro(NumberOfDivisions, unsigned int);

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  virtual void PrepareStreaming(itk::DataObject * input, const RegionType &region);

protected:
  NumberOfDivisionsStrippedStreamingManager();
  virtual ~NumberOfDivisionsStrippedStreamingManager();

  /** The splitter type used to generate the different strips */
  typedef itk::ImageRegionSplitter<itkGetStaticConstMacro(ImageDimension)> SplitterType;

  /** The number of lines per strip desired by the user.
   *  This may be different than the one computed by the Splitter */
  unsigned int m_NumberOfDivisions;
private:
  NumberOfDivisionsStrippedStreamingManager(const NumberOfDivisionsStrippedStreamingManager &);
  void operator =(const NumberOfDivisionsStrippedStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNumberOfDivisionsStrippedStreamingManager.txx"
#endif

#endif


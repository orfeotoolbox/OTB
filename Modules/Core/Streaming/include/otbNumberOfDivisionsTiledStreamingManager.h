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
#ifndef __otbNumberOfDivisionsTiledStreamingManager_h
#define __otbNumberOfDivisionsTiledStreamingManager_h

#include "otbStreamingManager.h"

namespace otb
{

/** \class NumberOfDivisionsTiledStreamingManager
 *  \brief This class computes the divisions needed to stream an image by strips,
 *  driven by a user-defined number of divisions in which to divide the images.
 *
 * You can use SetNumberOfDivisions to ask for a specific number of divisions.
 * The number of strips will be computed to fit this requirements as close as possible.
 *
 * \sa ImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 */
template<class TImage>
class ITK_EXPORT NumberOfDivisionsTiledStreamingManager : public StreamingManager<TImage>
{
public:
  /** Standard class typedefs. */
  typedef NumberOfDivisionsTiledStreamingManager Self;
  typedef StreamingManager<TImage>              Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  typedef TImage                          ImageType;
  typedef typename Superclass::RegionType RegionType;

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Type macro */
  itkTypeMacro(NumberOfDivisionsTiledStreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  /** The number of lines per strip desired */
  itkSetMacro(NumberOfDivisions, unsigned int);

  /** The number of lines per strip desired */
  itkGetMacro(NumberOfDivisions, unsigned int);

  /** Actually computes the stream divisions given a DataObject and its region to write */
  virtual void PrepareStreaming(itk::DataObject * input, const RegionType &region);

protected:
  NumberOfDivisionsTiledStreamingManager();
  virtual ~NumberOfDivisionsTiledStreamingManager();

  /** The number of lines per strip desired by the user.
   *  This may be different than the one computed by the Splitter */
  unsigned int m_NumberOfDivisions;
private:
  NumberOfDivisionsTiledStreamingManager(const NumberOfDivisionsTiledStreamingManager &);
  void operator =(const NumberOfDivisionsTiledStreamingManager&);
};

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNumberOfDivisionsTiledStreamingManager.txx"
#endif

#endif


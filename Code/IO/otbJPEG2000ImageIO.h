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
#ifndef __otbJPEG2000ImageIO_h
#define __otbJPEG2000ImageIO_h

#include "otbImageIOBase.h"
#include "itkMultiThreader.h"

#include <boost/shared_ptr.hpp>

namespace otb
{

class JPEG2000InternalReader;
class JPEG2000TileCache;

/** \class JPEG2000ImageIO
 *
 * \brief ImageIO object for reading and writing JPEG2000 format images
 *
 * The streaming (read and write) is implemented.
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT JPEG2000ImageIO : public otb::ImageIOBase
{
public:

  /** Standard class typedefs. */
  typedef JPEG2000ImageIO         Self;
  typedef otb::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(JPEG2000ImageIO, otb::ImageIOBase);

  /*-------- This part of the interface deals with reading data. ------ */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream read the specified file */
  virtual bool CanStreamRead()
  {
    return true;
  }

  /** Set the spacing and dimention information for the set filename. */
  virtual void ReadImageInformation();

  /** Get Info about all resolution in jpeg2000 file */
  bool GetResolutionInfo(std::vector<unsigned int>& res, std::vector<std::string>& desc);

  /** Get all resolution in jpeg2000 file from res = 0 which is the full resolution
   *   to nbReslution-1 which is most degraded*/
  bool GetAvailableResolutions(std::vector<unsigned int>& res);

  /** Get number of available overviews in the jpeg2000 file
   *  ( if return = 0 => no overviews available because only one resolution
   *  is encoded in the file) */
  unsigned int GetNumberOfOverviews();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  /** Reads 3D data from multiple files assuming one slice per file. */
  virtual void ReadVolume(void* buffer);

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*);

  /** Determine the file type. Returns true if the ImageIO can stream write the specified file */
  virtual bool CanStreamWrite()
  {
    return true;
  }

  /** Writes the spacing and dimentions of the image.
   * Assumes SetFileName has been called with a valid file name. */
  virtual void WriteImageInformation();

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegion has been set properly. */
  virtual void Write(const void* buffer);


  /** Return the multithreader used by this class. */
  itk::MultiThreader * GetMultiThreader()
  {return m_Threader; }

  itkSetMacro(ResolutionFactor, unsigned int);
  itkGetMacro(ResolutionFactor, unsigned int);

  itkSetMacro(CacheSizeInByte, unsigned int);
  itkGetMacro(CacheSizeInByte, unsigned int);

protected:
  /** Constructor.*/
  JPEG2000ImageIO();
  /** Destructor.*/
  virtual ~JPEG2000ImageIO();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  typedef std::vector<boost::shared_ptr<JPEG2000InternalReader> > ReaderVectorType;

  ReaderVectorType                     m_InternalReaders;
  boost::shared_ptr<JPEG2000TileCache> m_TileCache;

private:
  JPEG2000ImageIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Compute the tile index list from the GetRegion*/
  std::vector<unsigned int> ComputeTileList();

  /** pixel nb of octets */
  unsigned int m_BytePerPixel;

  /** Resolution factor*/
  unsigned int m_ResolutionFactor;

  /** */
  unsigned int m_NumberOfOverviews;

  /** Size of the cache used to reduce number of decoding operations*/
  unsigned int m_CacheSizeInByte;

  /** Load data from a tile into the buffer. 2nd argument is a
* pointer to opj_image_t, hidden in void * to avoid forward declaration. */
  void LoadTileData(void * buffer, void * tile);

  /** Support processing data in multiple threads. Used by subclasses
   * (e.g., ImageSource). */
  itk::MultiThreader::Pointer m_Threader;
  int                    m_NumberOfThreads;

  /** Static function used as a "callback" by the MultiThreader.  The threading
   * library will call this routine for each thread, which will delegate the
   * control to ThreadedGenerateData(). */
  static ITK_THREAD_RETURN_TYPE ThreaderCallback( void *arg );
};

} // end namespace otb

#endif // __otbJPEG2000ImageIO_h

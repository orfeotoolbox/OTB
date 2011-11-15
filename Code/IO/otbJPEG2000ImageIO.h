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

#include "itkImageIOBase.h"
#include "itkMultiThreader.h"


namespace otb
{

class JPEG2000ReaderInternal;
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
class ITK_EXPORT JPEG2000ImageIO : public itk::ImageIOBase
{
public:

  /** Standard class typedefs. */
  typedef JPEG2000ImageIO         Self;
  typedef itk::ImageIOBase        Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(JPEG2000ImageIO, itk::ImageIOBase);

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
  {return m_Threader;}

protected:
  /** Constructor.*/
  JPEG2000ImageIO();
  /** Destructor.*/
  virtual ~JPEG2000ImageIO();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  typedef std::vector<JPEG2000ReaderInternal *> ReaderVectorType;

  ReaderVectorType    m_InternalReaders;
  JPEG2000TileCache*  m_TileCache;

private:
  JPEG2000ImageIO(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Compute the tile index list from the GetRegion*/
  std::vector<unsigned int> ComputeTileList();

  /** pixel nb of octets */
  unsigned int m_BytePerPixel;


  /** Support processing data in multiple threads. Used by subclasses
   * (e.g., ImageSource). */
  itk::MultiThreader::Pointer m_Threader;
  int                    m_NumberOfThreads;
};

} // end namespace otb

#endif // __otbJPEG2000ImageIO_h

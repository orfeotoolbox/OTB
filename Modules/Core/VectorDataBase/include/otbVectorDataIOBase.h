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
#ifndef __otbVectorDataIOBase_h
#define __otbVectorDataIOBase_h

#include <string>

#include "itkLightProcessObject.h"
#include "itkIndent.h"
#include "itkPoint.h"
#include "itkDataObject.h"

namespace otb
{

/** \class VectorDataIOBase
 * \brief Abstract superclass defines VectorData IO interface.
 *
 * VectorDataIOBase is a class that reads and/or writes VectorData data
 * of a particular format (such as shapefile or kml). The
 * VectorDataIOBase encapsulates both the reading and writing of data. The
 * VectorDataIOBase is used by the VectorDataFileReader class (to read data)
 * and the VectorDataFileWriter (to write data) into a single file.
 * Normally the user does not directly
 * manipulate this class other than to instantiate it, set the FileName,
 * and assign it to a VectorDataFileReader/VectorDataFileWriter.
 *
 * A Pluggable factory pattern is used this allows different kinds of readers
 * to be registered (even at run time) without having to modify the
 * code in this class.
 *
 * \sa VectorDataFileWriter
 * \sa VectorDataFileReader
 *
 * \ingroup IOFilters
 *
 */
class ITK_EXPORT VectorDataIOBase : public itk::LightProcessObject
{
public:
  /** Standard class typedefs. */
  typedef VectorDataIOBase        Self;
  typedef itk::LightProcessObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataIOBase, Superclass);

  itkStaticConstMacro(VDimension, unsigned int, 3);
  typedef itk::Vector<double, VDimension> SpacingType;
  typedef itk::Point<double, VDimension>  PointType;

  /** Set/Get the name of the file to be read. */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

  /** Enums used to specify byte order; whether Big Endian or Little Endian.
   * Some subclasses use this, some ignore it. */
  typedef  enum {BigEndian, LittleEndian, OrderNotApplicable} ByteOrder;

  /** These methods indicate the byte ordering of the file you are
   * trying to read in. These methods will then either swap or not
   * swap the bytes depending on the byte ordering of the machine it
   * is being run on. For example, reading in a BigEndian file on a
   * BigEndian machine will result in no swapping. Trying to read the
   * same file on a LittleEndian machine will result in swapping.
   * Note: most UNIX machines are BigEndian while PC's and VAX's are
   * LittleEndian. So if the file you are reading in was generated on
   * a VAX or PC, SetByteOrderToLittleEndian() otherwise
   * SetByteOrderToBigEndian().  Some ImageIOBase subclasses
   * ignore these methods. */
  itkSetEnumMacro(ByteOrder, ByteOrder);
  itkGetEnumMacro(ByteOrder, ByteOrder);
  void SetByteOrderToBigEndian()
  {
    this->SetByteOrder(BigEndian);
  }
  void SetByteOrderToLittleEndian()
  {
    this->SetByteOrder(LittleEndian);
  }

  /** Convenience method returns the ByteOrder as a string. This can be
   * used for writing output files. */
  std::string GetByteOrderAsString(ByteOrder) const;

  /** Type for representing size of bytes, and or positions along a file */
  typedef std::streamoff SizeType;

  /*-------- This part of the interfaces deals with reading data ----- */

  /** Determine the file type. Returns true if this VectorDataIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*) const = 0;

  /** Determine if the VectorDataIO can stream reading from this
      file. Default is false. */
  virtual bool CanStreamRead() const
  {
    return false;
  }

  /*   /\** Read the spacing and dimentions of the VectorData. */
  /*    * Assumes SetFileName has been called with a valid file name. *\/ */
  /*   virtual void ReadVectorDataInformation() = 0; */

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(itk::DataObject* data) = 0;

  /*-------- This part of the interfaces deals with writing data ----- */

  /** Determine the file type. Returns true if this VectorDataIO can read the
   * file specified. */
  virtual bool CanWriteFile(const char*) const  = 0;

  /** Determine if the ImageIO can stream writing to this
      file. Default is false. */
  virtual bool CanStreamWrite() const
  {
    return false;
  }

  /*   /\** Writes the spacing and dimensions of the VectorData. */
  /*    * Assumes SetFileName has been called with a valid file name. *\/ */
  /*   virtual void WriteVectorDataInformation() = 0; */

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. The buffer is cast to a
   * pointer to the beginning of the image data. */
  virtual void Write(const itk::DataObject* data, char ** papszOptions = NULL) = 0;

protected:
  VectorDataIOBase();
  virtual ~VectorDataIOBase();

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Big or Little Endian, and the type of the file. (May be ignored.) */
  ByteOrder m_ByteOrder;

  /** Does the VectorDataIOBase object have enough info to be of use? */
  bool m_Initialized;

  /** Filename to read */
  std::string m_FileName;

  /** Return the object to an initialized state, ready to be used */
  virtual void Reset(const bool freeDynamic = true);

private:
  VectorDataIOBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif // __otbVectorDataIOBase_h

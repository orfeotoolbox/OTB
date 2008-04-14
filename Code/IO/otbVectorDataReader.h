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
#ifndef __otbVectorDataFileReader_h
#define __otbVectorDataFileReader_h

#include "otbVectorDataSource.h"
#include "otbVectorDataIOBase.h"
#include "itkExceptionObject.h"

namespace otb
{
/** \class VectorDataFileReaderException
 *
 * \brief Base exception class for IO conflicts.
 */
class VectorDataFileReaderException : public itk::ExceptionObject 
{
public:
  /** Run-time information. */
  itkTypeMacro( VectorDataFileReaderException, itk::ExceptionObject );

  /** Constructor. */
  VectorDataFileReaderException(const char *file, unsigned int line, 
                           const char* message = "Error in IO",
                           const char* loc = "Unknown") :
    itk::ExceptionObject(file, line, message, loc)
  {
  }

  /** Constructor. */
  VectorDataFileReaderException(const std::string &file, unsigned int line, 
                           const char* message = "Error in IO",
                           const char* loc = "Unknown") : 
    itk::ExceptionObject(file, line, message, loc)
  {
  }
};

/** \brief Data source that reads vector data from a single file.
 *
 * This source object is a general filter to read data from
 * a variety of vector data file formats. It works with a VectorDataIOBase subclass
 * to actually do the reading of the data. Object factory machinery
 * can be used to automatically create the VectorDataIOBase, or the
 * VectorDataIOBase can be manually created and set. Note that this
 * class reads data from a single file.
 *
 * TOutputVectorData is the type expected by the external users of the
 * filter. If data stored in the file is stored in a different format
 * then specified by TOutputVectorData, than this filter converts data 
 * between the file type and the external expected type.
 *
 * A Pluggable factory pattern is used this allows different kinds of readers
 * to be registered (even at run time) without having to modify the
 * code in this class. Normally just setting the FileName with the
 * appropriate suffix is enough to get the reader to instantiate the
 * correct VectorDataIO and read the file properly. However, some files (like
 * raw binary format) have no accepted suffix, so you will have to
 * manually create the ImageIO instance of the write type.
 *
 * \sa VectorDataIOBase
 *
 */

 
template <class TOutputVectorData>
class ITK_EXPORT VectorDataFileReader : public VectorDataSource<TOutputVectorData>
{
public :

  /** Standard class typedefs. */
  typedef VectorDataFileReader													Self;
  typedef VectorDataSource<TOutputVectorData>   Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorDataFileReader, VectorDataSource);
	
  typedef TOutputVectorData       OutputVectorType;
	
  /** Specify the file to read */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);
	
  /** Does the real work. */
  virtual void GenerateData();

protected:
	VectorDataFileReader();
	~VectorDataFileReader();

  VectorDataIOBase::Pointer m_VectorDataIO;

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  std::string m_FileName; // The file to be read
	
private:
  VectorDataFileReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Test whether the given filename exist and it is readable.
		  If the file doesn't exist or it is not readable, and exception with an
      appropriate message will be thrown. */
  void TestFileExistanceAndReadability();


} ;

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataFileReader.txx"
#endif

#endif // __otbVectorDataFileReader_h

	 
	 

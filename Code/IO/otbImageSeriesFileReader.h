/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:	  $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

	 This software is distributed WITHOUT ANY WARRANTY; without even 
	 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
	 PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbImageSeriesFileReader__h
#define __otbImageSeriesFileReader__h

#include <iostream>
#include <fstream>
#include <string>

#include "itkExceptionObject.h"

#include "otbList.h"
#include "otbImageList.h"
#include "otbImageListSource.h"
#include "otbImageFileReader.h"

namespace otb {

  class ImageSeriesFileReaderException : public itk::ExceptionObject 
  {
    public:
      itkTypeMacro( ImageSeriesFileReaderException, ExceptionObject );

      ImageSeriesFileReaderException(const char *file, unsigned int line, 
                                 const char* message = "Error in IO",
                                 const char* loc = "Unknown") :
        itk::ExceptionObject(file, line, message, loc) { }

      ImageSeriesFileReaderException(const std::string &file, unsigned int line, 
                                 const char* message = "Error in IO",
                                 const char* loc = "Unknown") : 
        itk::ExceptionObject(file, line, message, loc) { }
  };


template < class TImage >
class ImageSeriesFileReader : public ImageListSource< TImage >
{
  public:
    /** Standart typedefs */
    typedef ImageSeriesFileReader Self;
    typedef ImageListSource< TImage > Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    /** Creation through object factory macro */
    itkNewMacro(Self);
    /** Runtime informations macro */
    itkTypeMacro(ImageSeriesFileReader,ImageListSource);

    typedef TImage                               OutputImageType;
    typedef typename OutputImageType::Pointer    OutputImagePointerType;
    typedef typename OutputImageType::PixelType  PixelType;
    typedef typename OutputImageType::ValueType  ValueType;
    typedef typename OutputImageType::IndexType  IndexType;
    typedef typename OutputImageType::SizeType   SizeType;
    typedef typename OutputImageType::RegionType RegionType;

    typedef ImageList<OutputImageType>                 OutputImageListType;
    typedef typename OutputImageListType::Pointer      OutputImageListPointerType;
    typedef typename OutputImageListType::ConstPointer OutputImageListConstPointerType;
    
    typedef ImageFileReader< OutputImageType > ReaderType;
    typedef typename ReaderType::Pointer ReaderPointerType;

    typedef List< ReaderType > ListReaderType;
    typedef typename ListReaderType::Pointer ListReaderPointerType;

    /** File to be read */
    itkSetStringMacro(FileName);
    itkGetStringMacro(FileName);

    /** Get the readers */
    itkGetObjectMacro(ListReader,ListReaderType);

    /** Get the output list */
    virtual OutputImageListType * GetOutput(void);
    virtual OutputImageType *     GetOutput( unsigned int idx );

	protected:
    ImageSeriesFileReader(); 
    virtual ~ImageSeriesFileReader () { }

    void TestFileExistanceAndReadability();
    virtual void GenerateData( void );

    /** PrintSelf method */
    void PrintSelf ( std::ostream& os, itk::Indent indent ) const;

  private:
    ImageSeriesFileReader ( const Self & );
    void operator= ( const Self & );

    std::string m_FileName; 
    OutputImageListPointerType m_OutputList;
    ListReaderPointerType m_ListReader;


}; // end of class 

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageSeriesFileReader.txx"
#endif



#endif



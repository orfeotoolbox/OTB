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
#ifndef __otbShapeFileDataReader_h
#define __otbShapeFileDataReader_h

#include "otbShapeFileDataSource.h"

namespace otb
{
  /** \class ShapeFileDataReader
   *  \brief This class reads the ShapeFile format 
   *
   *  \ingroup ObjectSource
   */
	 
template <class TOutputShapeFile>
class ITK_EXPORT ShapeFileDataReader : public ShapeFileDataSource<TOutputShapeFile>
{
public :

	/** Standard class typedefs. */
	typedef ShapeFileDataReader													Self;
  typedef ShapeFileDataSource<TOutputShapeFile>		Superclass;
  typedef itk::SmartPointer<Self> 								Pointer;
  typedef itk::SmartPointer<const Self>  					ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ShapeFileDataReader, ShapeFileDataSource);
	
	typedef TOutputShapeFile									OutputShapeFileType;
	
  /** Specify the file to read */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);
	
  /** Does the real work. */
  virtual void GenerateData();

protected:
	ShapeFileDataReader();
	~ShapeFileDataReader();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  std::string m_FileName; // The file to be read
	
private:
  ShapeFileDataReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Test whether the given filename exist and it is readable.
		  If the file doesn't exist or it is not readable, and exception with an
      appropriate message will be thrown. */
  void TestFileExistanceAndReadability();


} ;

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShapeFileDataReader.txx"
#endif

#endif // __otbShapeFileDataReader_h

	 
	 

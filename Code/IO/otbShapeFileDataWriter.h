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
#ifndef __otbShapeFileDataWriter_h
#define __otbShapeFileDataWriter_h

#include "itkProcessObject.h"

namespace otb
{
  /** \class ShapeFileDataWriter
   *  \brief This class writes the ShapeFile format 
   *
   *  \ingroup IO
   */
	 
template <class TInputShapeFile>
class ITK_EXPORT ShapeFileDataWriter : public itk::ProcessObject
{
public :

	/** Standard class typedefs. */
	typedef ShapeFileDataWriter											Self;
	typedef itk::ProcessObject														Superclass;
  typedef itk::SmartPointer<Self> 								Pointer;
  typedef itk::SmartPointer<const Self>  					ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ShapeFileDataWriter, itk::ProcessObject);
	
	typedef TInputShapeFile									InputShapeFileType;
	
  /** Set/Get the image input of this writer.  */
  void SetInput(const InputShapeFileType *input);
  const InputShapeFileType * GetInput(void);
  const InputShapeFileType * GetInput(unsigned int idx);
	
  /** Does the real work. */
	virtual void Write();

  virtual void GenerateData();

	virtual void Update()  {  this->Write(); }

	
 	/** Specify the name of the output shapefile to write. */
 	itkSetStringMacro(FileName);
	itkGetStringMacro(FileName);

protected:
	ShapeFileDataWriter();
	~ShapeFileDataWriter();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  std::string m_FileName; // The file to be read
	
private:
  ShapeFileDataWriter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ;

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbShapeFileDataWriter.txx"
#endif

#endif // __otbShapeFileDataWriter_h

	 
	 

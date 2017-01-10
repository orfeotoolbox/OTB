/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: NegateFilter.cxx,v $
  Language:  C++
  Date:      $Date: 2003/07/25 14:12:35 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/



#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkImage.h"

namespace itk 
{

class Negate
{
public:
  Negate() {}
  ~Negate() {}
  inline unsigned char operator()( const unsigned char & A )
  {
    return (A==0)?255:0;
  }
};


template <class TImage >
class ITK_EXPORT NegateImageFilter :
    public
UnaryFunctorImageFilter<TImage,TImage, Negate   >
{
public:
  /** Standard class typedefs. */
  typedef NegateImageFilter  Self;
  typedef UnaryFunctorImageFilter<TImage,TImage, Negate > Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
 
protected:
  NegateImageFilter() {}
  virtual ~NegateImageFilter() {}

private:
  NegateImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


} // end namespace itk

int main( int argc, char ** argv )
{

  // Verify the number of parameters in the command line
  if( argc < 3 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    return -1;
    }


  typedef unsigned char        PixelType;

  const   unsigned int        Dimension = 3;

  typedef itk::Image< PixelType,  Dimension >    ImageType;



  typedef itk::ImageFileReader< ImageType >  ReaderType;
  typedef itk::ImageFileWriter< ImageType >  WriterType;


  typedef itk::NegateImageFilter< ImageType > FilterType;

  FilterType::Pointer filter = FilterType::New();


  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  //
  // Here we recover the file names from the command line arguments
  //
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );


  filter->SetInput( reader->GetOutput() );

  writer->SetInput( filter->GetOutput() );


  try 
    { 
    writer->Update(); 
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "ExceptionObject caught !" << std::endl; 
    std::cout << err << std::endl; 
    return -1;
    } 



  return 0;


}




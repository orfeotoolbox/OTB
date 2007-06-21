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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "otbImage.h"
#include <iostream>

#include "otbSVMModel.h"

namespace otb
{

class LinearKernelFunctor : public GenericKernelFunctorBase
{
public:
  LinearKernelFunctor(): GenericKernelFunctorBase() {};
  virtual ~LinearKernelFunctor() {};
  
  virtual double Evaluate(const svm_node *x, const svm_node *y, const svm_parameter& param)const
    {
      return dot(x,y);
    }


   static double dot(const svm_node *px, const svm_node *py)
   {
	double sum = 0;
	while(px->index != -1 && py->index != -1)
	{
		if(px->index == py->index)
		{
			sum += px->value * py->value;
			++px;
			++py;
		}
		else
		{
			if(px->index > py->index)
				++py;
			else
				++px;
		}			
	}
	return sum;
    }

};

}

int otbSVMModelGenericKernelLoadSave( int argc, char* argv[] )
{
  try 
    {         
        typedef unsigned char                                   InputPixelType;
	typedef unsigned char                                   LabelPixelType;
        const   unsigned int        	                        Dimension = 2;

        typedef otb::Image< InputPixelType,  Dimension >        InputImageType;

        typedef otb::SVMModel< InputPixelType, LabelPixelType >   ModelType;
	
	
        ModelType::Pointer svmModel = ModelType::New();

        otb::LinearKernelFunctor lFunctor;
        
        svmModel->SetKernelFunctor(&lFunctor);

	svmModel->LoadModel(argv[1]);
	svmModel->SaveModel(argv[2]);
        
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Unknown exception !" << std::endl; 
    return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}



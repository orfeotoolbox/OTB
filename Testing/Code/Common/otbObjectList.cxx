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
#include "itkExceptionObject.h"

#include "otbImageFileReader.h"
#include "otbObjectList.h"
#include "otbImage.h"


void fail(char * failString)
{
  std::cerr<<"Test failed in "<<failString<<std::endl;
  exit(-1);
}

int otbObjectList(int argc, char * argv[])
{
  try
    {
      const char * inputFilename1 = argv[1];
      const char * inputFilename2 = argv[2];
      const char * inputFilename3 = argv[3];
      const unsigned int Dimension = 2;

      typedef unsigned char InputPixelType;
      typedef otb::Image<InputPixelType,Dimension> InputImageType;
      typedef otb::ImageFileReader<InputImageType> ReaderType;
      typedef otb::ObjectList<InputImageType> ImageListType;

      // Reading image 1
      ReaderType::Pointer reader1 = ReaderType::New();
      reader1->SetFileName(inputFilename1);
      reader1->Update();

      // Reading image 2
      ReaderType::Pointer reader2 = ReaderType::New();
      reader2->SetFileName(inputFilename2);
      reader2->Update();

      // Reading image 3
      ReaderType::Pointer reader3 = ReaderType::New();
      reader3->SetFileName(inputFilename3);
      reader3->Update();

      // Instantiating the tested object
      ImageListType::Pointer imageList = ImageListType::New();
      
      // Testing reserve/capacity
      imageList->Reserve(2);

      if(imageList->Capacity()!=2)
	{
	  fail("Reserve/Capacity()");
	}

      // Testing Size/Element accessor
      imageList->PushBack(reader1->GetOutput());
      imageList->PushBack(reader2->GetOutput());

      if(imageList->Size()!=2)	  
	{
	  fail("PushBack/Size()");
	}
      if(imageList->GetNthElement(0)!=reader1->GetOutput())
	{
	  fail("PushBack/GetNthElement(0)");
	}
      if(imageList->GetNthElement(1)!=reader2->GetOutput())
	{
	  fail("PushBack/GetNthElement(1)");
	}
      if(imageList->Front()!=reader1->GetOutput())
	{
	  fail("PushBack/Front()");
	}
      if(imageList->Back()!=reader2->GetOutput())
	{
	  fail("PushBack/Back()");
	}

      // Testing resizing and related method 
      imageList->Resize(3);
       if(imageList->Size()!=3)
 	{
 	  fail("Resize/Size()");
 	}
      
      // Testing explicit setter
      imageList->SetNthElement(2,reader3->GetOutput());

      if(imageList->Size()!=3)
	{
	  fail("SetNthElement/Size()");
	}
      if(imageList->GetNthElement(2)!=reader3->GetOutput())
	{
	  fail("SetNthElement/GetNthElement(2)");
	}

      // Testing erase operation
      imageList->Erase(2);
      if((imageList->Size()!=2)
	 ||(imageList->GetNthElement(0)!=reader1->GetOutput())
	 ||(imageList->GetNthElement(1)!=reader2->GetOutput()))
	{
	  fail("Erase(3)");
	}

      // Testing iterator
      ImageListType::Iterator iter = imageList->Begin();

      if(!(iter!=imageList->End()))
      {
	fail("Iterator/Begin()!=Iterator/End()");
      }
      unsigned int index = 0;
      while(iter!=imageList->End())
	{
	  if((index==0)&&(reader1->GetOutput()!=iter.Get()))
	    {
	      fail("Iterator/1/iter.Get()");
	    }
	  if((index==1)&&(reader2->GetOutput()!=iter.Get()))
	    {
	      fail("Iterator/2/iter.Get()");
	    }
	  if(index>1)
	    {
	      fail("Iterator/OutOfBound/iter.Get()");
	    }
	  ++index;
	  ++iter;
	}
    
      // Testing const iterator
      ImageListType::ConstIterator constIter = imageList->Begin();
      index = 0;
      while(constIter!=imageList->End())
	{
	  if((index==0)&&(reader1->GetOutput()!=constIter.Get()))
	    {
	      fail("ConstIterator/1/iter.Get()");
	    }
	  if((index==1)&&(reader2->GetOutput()!=constIter.Get()))
	    {
	      fail("ConstIterator/2/iter.Get()");
	    }
	  if(index>1)
	    {
	      fail("ConstIterator/OutOfBound/iter.Get()");
	    }
	  ++index;
	  ++constIter;
	}

      //Testing reverse iterator
      ImageListType::ReverseIterator revIter = imageList->ReverseBegin();
      if(!(revIter!=imageList->ReverseEnd()))
      {
	fail("ReverseIterator/ReverseBegin()!=ReverseIterator/ReverseEnd()");
      }

      index = 0;
      while(revIter!=imageList->ReverseEnd())
	{
	  if((index==0)&&(reader2->GetOutput()!=revIter.Get()))
	    {
	      fail("ReverseIterator/1/iter.Get()");
	    }
	  if((index==1)&&(reader1->GetOutput()!=revIter.Get()))
	    {
	      fail("ReverseIterator/2/iter.Get()");
	    }
	  if(index>1)
	    {
	      fail("ReverseIterator/OutOfBound/iter.Get()");
	    }
	  ++index;
	  ++revIter;
	}

      //Testing const reverse iterator
      ImageListType::ReverseConstIterator revConstIter = imageList->ReverseBegin();
      index = 0;
      while(revConstIter!=imageList->ReverseEnd())
	{
	  if((index==0)&&(reader2->GetOutput()!=revConstIter.Get()))
	    {
	      fail("ReverseConstIterator/1/iter.Get()");
	    }
	  if((index==1)&&(reader1->GetOutput()!=revConstIter.Get()))
	    {
	      fail("ReverseConstIterator/2/iter.Get()");
	    }
	  if(index>1)
	    {
	      fail("ReverseConstIterator/OutOfBound/iter.Get()");
	    }
	  ++index;
	  ++revConstIter;
	}
    
      // Testing clear
      imageList->Clear();

      if(imageList->Size()!=0)
	{
	  fail("Clear()");
	}
    }

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}


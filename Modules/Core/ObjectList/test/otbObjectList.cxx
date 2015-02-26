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


#include "otbImageFileReader.h"
#include "otbObjectList.h"
#include "otbImage.h"
#include "otbMacro.h"

int otbObjectList(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename1 = argv[1];
  const char *       inputFilename2 = argv[2];
  const char *       inputFilename3 = argv[3];
  const unsigned int Dimension = 2;

  typedef unsigned char                         InputPixelType;
  typedef otb::Image<InputPixelType, Dimension> InputImageType;
  typedef otb::ImageFileReader<InputImageType>  ReaderType;
  typedef otb::ObjectList<InputImageType>       ImageListType;

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

  otbControlConditionTestMacro((imageList->Capacity() != 2), "Reserve/Capacity()");

  // Testing Size/Element accessor
  imageList->PushBack(reader1->GetOutput());
  imageList->PushBack(reader2->GetOutput());

  otbControlConditionTestMacro(imageList->Size() != 2, "PushBack/Size()");
  otbControlConditionTestMacro(imageList->GetNthElement(0) != reader1->GetOutput(), "PushBack/GetNthElement(0)");
  otbControlConditionTestMacro(imageList->GetNthElement(1) != reader2->GetOutput(), "PushBack/GetNthElement(1)");
  otbControlConditionTestMacro(imageList->Front() != reader1->GetOutput(), "PushBack/Front()");
  otbControlConditionTestMacro(imageList->Back() != reader2->GetOutput(), "PushBack/Back()");

  // Testing resizing and related method
  imageList->Resize(3);
  otbControlConditionTestMacro(imageList->Size() != 3, "Resize/Size()");

  // Testing explicit setter
  imageList->SetNthElement(2, reader3->GetOutput());

  otbControlConditionTestMacro(imageList->Size() != 3, "SetNthElement/Size()");
  otbControlConditionTestMacro(imageList->GetNthElement(2) != reader3->GetOutput(), "SetNthElement/GetNthElement(2)");

  // Testing erase operation
  imageList->Erase(2);
  otbControlConditionTestMacro((imageList->Size() != 2)
                               || (imageList->GetNthElement(0) != reader1->GetOutput())
                               || (imageList->GetNthElement(1) != reader2->GetOutput()), "Erase(3)");

  // Testing iterator
  ImageListType::Iterator iter = imageList->Begin();

  otbControlConditionTestMacro(!(iter != imageList->End()), "Iterator/Begin()!=Iterator/End()");
  unsigned int index = 0;
  while (iter != imageList->End())
    {
    otbControlConditionTestMacro((index == 0) && (reader1->GetOutput() != iter.Get()), "Iterator/1/iter.Get()");
    otbControlConditionTestMacro((index == 1) && (reader2->GetOutput() != iter.Get()), "Iterator/2/iter.Get()");
    otbControlConditionTestMacro(index > 1, "Iterator/OutOfBound/iter.Get()");
    ++index;
    ++iter;
    }

  // Testing operator+
  iter = imageList->Begin();
  index = 0;
  otbControlConditionTestMacro(imageList->GetNthElement(0) != iter.Get(),
                               "Iterator != GetNthElement(0)");
  otbControlConditionTestMacro(imageList->GetNthElement(1) != (iter + 1).Get(),
                               "Iterator+1 != GetNthElement(1)");
  ++iter;
  otbControlConditionTestMacro(imageList->GetNthElement(1) != iter.Get(),
                               "Iterator != GetNthElement(1)");
  otbControlConditionTestMacro(imageList->GetNthElement(0) != (iter - 1).Get(),
                               "Iterator-1 != GetNthElement(0)");

  // Testing const iterator
  ImageListType::ConstIterator constIter = imageList->Begin();
  index = 0;
  while (constIter != imageList->End())
    {
    otbControlConditionTestMacro(
      (index == 0) && (reader1->GetOutput() != constIter.Get()), "ConstIterator/1/iter.Get()");
    otbControlConditionTestMacro(
      (index == 1) && (reader2->GetOutput() != constIter.Get()), "ConstIterator/2/iter.Get()");
    otbControlConditionTestMacro(index > 1, "ConstIterator/OutOfBound/iter.Get()");
    ++index;
    ++constIter;
    }

  //Testing reverse iterator
  ImageListType::ReverseIterator revIter = imageList->ReverseBegin();
  otbControlConditionTestMacro(
    !(revIter != imageList->ReverseEnd()), "ReverseIterator/ReverseBegin()!=ReverseIterator/ReverseEnd()");

  index = 0;
  while (revIter != imageList->ReverseEnd())
    {
    otbControlConditionTestMacro(
      (index == 0) && (reader2->GetOutput() != revIter.Get()), "ReverseIterator/1/iter.Get()");
    otbControlConditionTestMacro(
      (index == 1) && (reader1->GetOutput() != revIter.Get()), "ReverseIterator/2/iter.Get()");
    otbControlConditionTestMacro(index > 1, "ReverseIterator/OutOfBound/iter.Get()");
    ++index;
    ++revIter;
    }

  //Testing const reverse iterator
  ImageListType::ReverseConstIterator revConstIter = imageList->ReverseBegin();
  index = 0;
  while (revConstIter != imageList->ReverseEnd())
    {
    otbControlConditionTestMacro(
      (index == 0) && (reader2->GetOutput() != revConstIter.Get()), "ReverseConstIterator/1/iter.Get()");
    otbControlConditionTestMacro(
      (index == 1) && (reader1->GetOutput() != revConstIter.Get()), "ReverseConstIterator/2/iter.Get()");
    otbControlConditionTestMacro(index > 1, "ReverseConstIterator/OutOfBound/iter.Get()");
    ++index;
    ++revConstIter;
    }

  // Testing clear
  imageList->Clear();

  otbControlConditionTestMacro(imageList->Size() != 0, "Clear()");

  // Testing erase with iterators
  imageList->PushBack(reader1->GetOutput());
  imageList->PushBack(reader2->GetOutput());
  imageList->PushBack(reader3->GetOutput());

  ImageListType::Iterator begin = imageList->Begin() + 1;
  ImageListType::Iterator end = imageList->End();
  imageList->Erase(begin, end);

  otbControlConditionTestMacro(imageList->Size() != 1, "Erase(Iterator, Iterator)/Size()");
  otbControlConditionTestMacro(imageList->Back() != reader1->GetOutput(), "Erase(Iterator, Iterator)/Back()");

  return EXIT_SUCCESS;
}

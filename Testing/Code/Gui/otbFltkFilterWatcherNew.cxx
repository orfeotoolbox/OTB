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

#include "otbFltkFilterWatcher.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImage.h"

int otbFltkFilterWatcherNew(int argc, char * argv[])
{
//        const char * filename = argv[1];
        typedef otb::Image<char,2> ImageType;
        typedef otb::StreamingImageFileWriter<ImageType> WriterType;
        
        WriterType::Pointer writer = WriterType::New();
//        writer->SetFileName(filename);
//        writer->SetInput(m_ChangeLabelFilter->GetOutput());
        otb::FltkFilterWatcher watcher(writer,0,0,200,20,"Saving result image ...");
//        writer->Update();
        return EXIT_SUCCESS;
}


#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

//  Software Guide : BeginCommandLineArgs
//    INPUTS: {IMAGERY_SSECH.tif}
//    OUTPUTS: {ROI_IMAGERY_RGB.png}, {ROI_IMAGERY_MIR.png}
//    0 0 100 100
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
//
//  Nous commençons par introduire les fichiers d'ent\^etes utilis\'es.
//  \index{itk::ExtractROI!header}
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"
// Software Guide : EndCodeSnippet


#include "itkImage.h"


int main( int argc, char * argv[] )
{ 

  // R\'ecuperation de arguments	
  if( argc < 7 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile outputImageFile startX startY sizeX sizeY" << std::endl;
    return EXIT_FAILURE;
    }

//  Software Guide : BeginLatex
//
//  Les param\`etres d'entr\'ee d\'efinissent le nom des images d'entr\'ee et de sortie
//  ainsi que les coordonn\'ees du point d'origine et la taille de la r\'egion \`a extraire.
//
//  Software Guide : EndLatex


  // Noms des images d'entr\'ee et de sortie
  // Definition de la ROI
  
  // Software Guide : BeginCodeSnippet
  const char * inputFilename  = argv[1];
  const char * outputFilenameRGB = argv[2];
  const char * outputFilenameMIR = argv[3]; 
  
  unsigned int  startX((unsigned int)::atoi(argv[4]));
  unsigned int  startY((unsigned int)::atoi(argv[5]));
  unsigned int  sizeX((unsigned int)::atoi(argv[6]));
  unsigned int  sizeY((unsigned int)::atoi(argv[7]));
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  On d\'efinit ensuite le type de pixel de l'image d'entr\'ee et des images de sortie.
  //
  //  Software Guide : EndLatex 

  // Software Guide : BeginCodeSnippet
  typedef   unsigned char  InputPixelType;
  typedef   unsigned char  OutputPixelType;
  // Software Guide : EndCodeSnippet

  // ***************************************************************************
  // *************** Traitement de l'image trois canaux RGB ********************
  // ***************************************************************************
      
  //  Software Guide : BeginLatex
  //
  //  Dans un premier temps, on s'int\'eresse \`a la g\'en\'eration d'une image
  //  multi canal de la r\'egion extraite. 
  // 
  //  Le type de la r\'egion est d\'efinie par la classe otb::MultiChannelExtractROI.
  //  Il est important de noter que c'est bien le type du pixel qui est utilis\'e comme
  //  param\`etre de la classe template d'extraction d'une r\'egion.  
  //
  //  Software Guide : EndLatex 
 
  // Software Guide : BeginCodeSnippet 
  typedef otb::MultiChannelExtractROI< InputPixelType, 
                                             OutputPixelType >  ExtractROIFilterType;
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  La r\'egion est cr\'e\'ee par l'op\'erateur New() de la classe otb::MultiChannelExtractROI
  //  \`a laquelle on attribue les coordonn\'ees du point d'origine et sa taille.
  //
  //  Software Guide : EndLatex
  
  // Software Guide : BeginCodeSnippet   
  ExtractROIFilterType::Pointer extractROIFilter = ExtractROIFilterType::New();
  
  extractROIFilter->SetStartX( startX );
  extractROIFilter->SetStartY( startY );
  extractROIFilter->SetSizeX( sizeX );
  extractROIFilter->SetSizeY( sizeY );
  // Software Guide : EndCodeSnippet
   
  //  Software Guide : BeginLatex
  //
  //  Reste \`a d\'efinir les canaux utilis\'e pour g\'en\'erer l'image de sortie. 
  //  Dans le cadre de notre exemple, les canaux 1 \`a 3 de l'image initiale quatre 
  //  canaux sont utilis\'es pour g\'en\'erer l'image finale trois canaux RGB. 
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet   
  extractROIFilter->SetFirstChannel( 1 );
  extractROIFilter->SetLastChannel( 3 );    
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Comme pour la classe itk::Image, on d\'efinit un reader et un writer sur les images
  //  dont le type est d\'efini dans la classe otb::MultiChannelExtractROI.  
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet     
  typedef otb::ImageFileReader< ExtractROIFilterType::InputImageType >       ReaderType;
  typedef otb::ImageFileWriter< ExtractROIFilterType::InputImageType >      WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  On assigne ensuite au reader et au writer le nom des images d'entr\'ee et de sortie.
  //
  //  Software Guide : EndLatex  

  // Software Guide : BeginCodeSnippet 
  reader->SetFileName( inputFilename  );
  reader->Update(); //Necessaire pour connaitre le nombre de canaux dans l'image
  writer->SetFileName( outputFilenameRGB );
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  L'image obtenue par le reader est pass\'ee comme param\`etre d'entr\'ee de la 
  //  classe otb::MultiChannelExtractROI et la sortie du filtre est pass\'ee 
  //  au writer pour g\'en\'erer l'image finale. 
  //
  //  Software Guide : EndLatex  
     
  // Software Guide : BeginCodeSnippet  
  extractROIFilter->SetInput( reader->GetOutput() );
        
  writer->SetInput( extractROIFilter->GetOutput() );
  // Software Guide : EndCodeSnippet

  //  Software Guide : BeginLatex
  //
  //  Le filtre n'est ex\'ecut\'e qu'\`a l'appel de la m\'ethode Update() qui met
  //  en route tout le processus de pipeline.
  //
  //  Software Guide : EndLatex 
    
  // Software Guide : BeginCodeSnippet 
  writer->Update(); 
  // Software Guide : EndCodeSnippet

  // *********************************************************************
  // *************** Traitement de l'image mono canal ********************
  // *********************************************************************

  //  Software Guide : BeginLatex
  //
  //  La proc\'edure d'utilisation de la classe otb::MultiToMonoChannelExtractROI est 
  //  similaire \`a celle d\'etaill\'ee ci-dessus pour la classe otb::MultiChannelExtractROI.
  //
  //  Le but de ce deuxi\`eme filtre est d'extraire une r\'egion d'une image multi canaux et
  //  d'en g\'en\'erer une image mono canal.
  //
  //  L'int\^eret du filtre MultiToMonoChannelExtractROI par rapport au filtre MultiChanelExtractROI
  //  est qu'au lieu de g\'en\'erer en sortie des images de type itk::VectorImage, type encore 
  //  tr\`es peu utilis\'e par les classes itk, il produit de simple itk::image. 
  //
  //  Le type du pixel est toujours utilis\'e comme param\`etre d'entr\'ee de cette nouvelle
  //  classe template comme on peut le voir ci-dessous
  //
  //  Software Guide : EndLatex 
 
  // Software Guide : BeginCodeSnippet 
  typedef otb::MultiToMonoChannelExtractROI< InputPixelType, 
                                             OutputPixelType >  ExtractROIMonoFilterType;
  // Software Guide : EndCodeSnippet
   
  ExtractROIMonoFilterType::Pointer extractROIMonoFilter = ExtractROIMonoFilterType::New();
  
  extractROIMonoFilter->SetStartX( startX );
  extractROIMonoFilter->SetStartY( startY );
  extractROIMonoFilter->SetSizeX( sizeX );
  extractROIMonoFilter->SetSizeY( sizeY );

  //  Software Guide : BeginLatex
  //
  // mais cette fois, seul un canal est attribu\'e au filtre d'extraction de la r\'egion.
  //
  //  Software Guide : EndLatex 
  
  // Software Guide : BeginCodeSnippet 
  extractROIMonoFilter->SetChannel( 4 );    
  // Software Guide : EndCodeSnippet
  
  typedef otb::ImageFileReader< ExtractROIMonoFilterType::InputImageType >       monoReaderType;
  typedef otb::ImageFileWriter< ExtractROIMonoFilterType::OutputImageType >      monoWriterType;
  monoReaderType::Pointer monoReader = monoReaderType::New();
  monoWriterType::Pointer monoWriter = monoWriterType::New();

  monoReader->SetFileName( inputFilename  );
  monoReader->Update(); //Necessaire pour connaitre le nombre de canaux dans l'image
  monoWriter->SetFileName( outputFilenameMIR );
  extractROIMonoFilter->SetInput( monoReader->GetOutput() );
        
  monoWriter->SetInput( extractROIMonoFilter->GetOutput() );
  monoWriter->Update();     
 
  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{IMAGERY_SSECH.eps}
  // \itkcaption[Image SPOT5 initale]{Image SPOT5 initiale, sous \'echantillon\'ee (600x600).}
  // \label{fig:IMAGERY_SSECH}
  // \end{figure}
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{ROI_IMAGERY_RGB.eps}
  // \includegraphics[width=0.44\textwidth]{ROI_IMAGERY_MIR.eps}
  // \itkcaption[ROI Image SPOT5]{R\'esultat de l'extraction d'une r\'egion d'int\'er\^et
  // sur une image SPOT5, quatre canaux, sous \'echantillon\'ee. La figure de droite est 
  // une image RGB g\'en\'er\'ee par la classe otb::otbMultiChannelExtractROI. La figure de
  // gauche est une image mono canal g\'en\'er\'ee par la classe 
  // otb::otbMultiToMonoChannelExtractROI.} 
  // \label{fig:ROI_IMAGERY}
  // \end{figure}
  //
  //  La figure \ref{fig:ROI_IMAGERY} illustre le résultat de l'application de ces deux
  //  filtres sur l'image SPOT5 pr\'esent\'ee ci-dessus.
  //
  //  Software Guide : EndLatex

       
  return EXIT_SUCCESS;
  
  
}


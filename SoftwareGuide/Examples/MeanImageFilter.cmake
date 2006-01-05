# Cmake macro to invoke: /ORFEO/thomas/ORFEO-TOOLBOX/OTB/bin/MeanImageFilter  /ORFEO/thomas/ORFEO-TOOLBOX/OTB/Examples/Data/Circle.png /home/ORFEO/thomas/ORFEO-TOOLBOX/OTB-Documents/SoftwareGuide/Art/Generated/CircleMeanOutput.png 10 10
RUN_EXAMPLE( "MeanImageFilter" "CircleMeanOutput.png" "/ORFEO/thomas/ORFEO-TOOLBOX/OTB/Examples/StartExamples/MeanImageFilter.cxx"  /ORFEO/thomas/ORFEO-TOOLBOX/OTB/Examples/Data/Circle.png /home/ORFEO/thomas/ORFEO-TOOLBOX/OTB-Documents/SoftwareGuide/Art/Generated/CircleMeanOutput.png 10 10 )
CONVERT_INPUT_IMG( "Circle.png" "Circle.eps" "/ORFEO/thomas/ORFEO-TOOLBOX/OTB/Examples/Data" )
ADD_DEP_TEX_ON_EPS_FIGS( "/home/ORFEO/thomas/ORFEO-TOOLBOX/OTB-Documents/SoftwareGuide/Examples/MeanImageFilter.tex" "Circle.eps" )
CONVERT_IMG( "CircleMeanOutput.png" "CircleMeanOutput.eps" "/home/ORFEO/thomas/ORFEO-TOOLBOX/OTB-Documents/SoftwareGuide/Art/Generated" )
ADD_DEP_TEX_ON_EPS_FIGS( "/home/ORFEO/thomas/ORFEO-TOOLBOX/OTB-Documents/SoftwareGuide/Examples/MeanImageFilter.tex" "CircleMeanOutput.eps" )

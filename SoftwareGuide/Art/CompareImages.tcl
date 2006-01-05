package require vtk

vtkPNGReader reader1
  reader1 SetFileName "PNG/BrainT1SliceBorder20.png"
  
vtkPNGReader reader2
  reader2 SetFileName "PNG/BrainProtonDensitySliceShifted13x17y.png"

vtkImageCheckerboard checkers
  checkers SetInput 0 [reader1 GetOutput]
  checkers SetInput 1 [reader2 GetOutput]
  checkers SetNumberOfDivisions 4 4 1

vtkPNGWriter writer
  writer  SetInput [ checkers GetOutput ]

  writer SetFileName "PNG/ImageRegistration2CheckboardBefore.png"
  writer  Write
  
  reader2 SetFileName "PNG/ImageRegistration2Output.png"
  writer  SetFileName "PNG/ImageRegistration2CheckboardAfter.png"
  writer  Write

vtkImageViewer viewer
  viewer SetInput [ checkers GetOutput ]

  viewer SetColorWindow  256
  viewer SetColorLevel   128

  viewer Render

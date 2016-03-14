package require vtk

vtkPNGReader reader
  reader SetFileName "PNG/BrainT1SliceBorder20.png"

vtkImageViewer viewer
  viewer SetInput [reader GetOutput]
  viewer SetColorWindow 256
  viewer SetColorLevel  128

viewer Render


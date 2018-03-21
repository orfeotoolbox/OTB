
def test(otb, argv):
  # Create a smoothing application
  app = otb.Registry.CreateApplication("Smoothing")
  app.SetParameterString("in",argv[1])
  app.Execute()

  # Setup a special requested region
  myRegion = otb.itkRegion()
  myRegion.GetSize()[0] = 20
  myRegion.GetSize()[1] = 25
  myRegion.GetIndex().Fill(10)
  app.PropagateRequestedRegion("out",myRegion)
  print(app.GetImageRequestedRegion("in"))
  
  # Create a ReadImageInfo application and plug the output of app
  app2 = otb.Registry.CreateApplication("ReadImageInfo")
  ex = app.ExportImage("out")
  app2.ImportVectorImage("in", ex)
  app2.Execute()
  someKeys = ['sizex', 'sizey', 'spacingx', 'spacingy', 'sensor', 'projectionref']
  for key in someKeys:
    print(key + ' : ' + str(app2.GetParameterValue(key)) )

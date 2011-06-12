#
#  Example on the use of the Smoothing 
#
from sys import argv
import otbApplication

print otbApplication.ApplicationRegistry.GetAvailableApplications()

app = otbApplication.ApplicationRegistry.CreateApplication("Smoothing")
app.SetParameterString("in", argv[1])

for type in ['mean', 'gaussian', 'anidif']:
  app.SetParameterString("type", type)
  app.SetParameterString("out", argv[2] + type + ".tif")
  app.Execute()
  
print dir(app)

import otbApplication,sys

app = otbApplication.Registry.CreateApplication('Rasterization')

try:
  app.GetParameterInt('szx')
except RuntimeError as e:
  print "Exception message : " + e.args[0]
  if e.args[0].startswith("boost::bad_any_cast"):
    sys.exit(1)

sys.exit(0)


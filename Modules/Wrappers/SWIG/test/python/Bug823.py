from sys import exit
def test(otb, argv):
	app = otb.Registry.CreateApplication('Rasterization')
	try:
		app.GetParameterInt('szx')
	except RuntimeError, e:
		print "Exception message : " + e.args[0]
		if e.args[0].startswith("boost::bad_any_cast"):
			exit(1)

		exit(0)

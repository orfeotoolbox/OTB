import otbApplication as otb
import operator
import inspect
# List applications
appNames = [app for app in otb.Registry.GetAvailableApplications()]
for appName in appNames:
	app = otb.Registry.CreateApplication(appName)
	# Generate parameters
	parameters = app.GetParametersKeys()
	for param in parameters:
		upParam = param.upper()
		if app.HasValue(param):
			operator.attrgetter(upParam)(app)


import sys
import re

def parameter_warnings(app_warn, app, key):

    def warn(message):
        app_warn("Parameter '{}' ".format(key) + message)

    name = app.GetParameterName(key)
    description = app.GetParameterDescription(key)

    if name[-1] == " ":
        warn("name ends with a space")

    if ":" in name:
        warn("name contains a special character (:)")

    if "." in name:
        warn("name contains a special character (.)")

def application_documentation_warnings(app):
    "Emit warnings about application documentation"

    def warn(message):
        print("OTB Documentation Warning ({}): {}".format(app.GetName(), message), file=sys.stderr)

    description = app.GetDescription()
    longdescription = app.GetDocLongDescription()

    if re.search("\\n [a-zA-Z]", longdescription):
        warn("Application Long Description contains '\\n ' pattern (usually not intended)")

    if app.GetNumberOfExamples() == 0:
        warn("Application has no examples")

    keys = app.GetParametersKeys()
    for key in app.GetParametersKeys():
        parameter_warnings(warn, app, key)

    if "inxml" in keys and not keys[-2] == "inxml":
        warn("'inxml' parameter is not second from last parameter")

    if "outxml" in keys and not keys[-1] == "outxml":
        warn("'outxml' is not last parameter")


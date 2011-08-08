print ( "Lua test begin" )

r = otbApplication.Registry
apps = r:GetAvailableApplications()

print ( "otbApplication.Registry:GetAvailableApplications OK" )
print ( "apps" )
print ( apps )
os.exit ( 0 )
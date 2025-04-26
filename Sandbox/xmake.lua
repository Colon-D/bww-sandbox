local projectName = "Sandbox"

target(projectName)
	add_rules("ue4ss.mod")
	add_includedirs(".")
	add_files("dllmain.cpp")
	set_basename("main")
	add_packages("sol2")

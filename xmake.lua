package("sol2")
	add_deps("cmake")
	set_sourcedir(path.join(os.scriptdir(), "sol2"))
	on_install(function (package)
		local configs = {}
		table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
		table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
		import("package.tools.cmake").install(package, configs)
	end)
package_end()

add_requires("sol2")

includes("RE-UE4SS")
includes("Sandbox")

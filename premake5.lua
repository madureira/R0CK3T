workspace "R0CK3T"
	architecture "x64"
	startproject "R0CK3T"
	configurations { "Debug", "Release" }
	platforms { "x64", "x32" }
	files {
		".editorconfig"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.system}/%{cfg.buildcfg}/%{cfg.platform}/%{prj.name}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["Boost"] = "Libraries/boost/include"

-- R0CK3T
project "R0CK3T"
	location "R0CK3T/R0CK3T"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"
	objdir("Build/" .. outputdir)
	targetdir("Dist/" .. outputdir)

	pchheader "rtpch.h"
	pchsource "R0CK3T/%{prj.name}/Source/rtpch.cpp"

	makesettings { "CC = gcc" }

	files {
		"R0CK3T/%{prj.name}/Source/**.h",
		"R0CK3T/%{prj.name}/Source/**.cpp"
	}

	includedirs {
		"R0CK3T/%{prj.name}/Source",
		"%{IncludeDir.Boost}",
	}

	links {
		"Boost"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter { "system:windows" }
		systemversion "latest"

		defines {
			"R0CK3T_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines { "DEBUG" }
		symbols "On"
		optimize "On"

	filter "configurations:Release"
		defines { "NDEBUG" }
		symbols "On"
		optimize "Speed"


-- Clean Function --
newaction {
	trigger = "clean",
	description = "clean the build",
	execute = function()
		print("Cleanning the build...")
		os.rmdir("./.vs")
		os.rmdir("./Build")
		os.rmdir("./Dist")
		os.rmdir("./Generated")
		os.remove("./Makefile")
		os.remove("./Morpheus.sln")
		os.remove("./Makefile")
		os.remove("./R0CK3T/R0CK3T/Editor.vcxproj")
		os.remove("./R0CK3T/R0CK3T/Editor.vcxproj.*")
		os.remove("./R0CK3T/R0CK3T/Makefile")
		print("Done.")
	end
}

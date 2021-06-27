CompilerPath = C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe
CompilerFlags = -std=c++17
CompilerRealeaseFlags= -O3
VulkanLibPath = -LF:\Programme\Vulkan\1.2.176.1\Lib
VulkanIncludePath = -IF:\Programme\Vulkan\1.2.176.1\Include
LinkerFLags = -lvulkan-1
SourceCode = $(wildcard src/*.cpp)
Executable = F:\Programme\VulkanPrototype\bin\main.exe
ReleaseExecutable = F:\Programme\VulkanPrototype\release.exe

DebuggingBuild:
	$(CompilerPath) -g $(CompilerFlags) $(SourceCode) -o $(Executable) $(VulkanIncludePath) $(VulkanLibPath) $(LinkerFLags)

ReleaseBuild:
	$(CompilerPath) $(CompilerFlags) $(CompilerRealeaseFlags) $(SourceCode) -o $(ReleaseExecutable) $(VulkanIncludePath) $(VulkanLibPath) $(LinkerFLags)

CompilerPath = /usr/bin/g++
CompilerFlags = -std=c++17
CompilerRealeaseFlags= -O3
VulkanLibPath = -L/home/simon/test/Vulkan/1.2.176.1/x86_64/lib
VulkanIncludePath = -I/home/simon/test/Vulkan/1.2.176.1/x86_64/include
LinkerFLags = -lvulkan
SourceCode = $(wildcard src/*.cpp)
Executable = /home/simon/test/VulkanPrototype/bin/main.out
ReleaseExecutable = /home/simon/test/VulkanPrototype/release.out

DebuggingBuild:
	$(CompilerPath) -g $(CompilerFlags) $(SourceCode) -o $(Executable) $(VulkanIncludePath) $(VulkanLibPath) $(LinkerFLags)

ReleaseBuild:
	$(CompilerPath) $(CompilerFlags) $(CompilerRealeaseFlags) $(SourceCode) -o $(ReleaseExecutable) $(VulkanIncludePath) $(VulkanLibPath) $(LinkerFLags)

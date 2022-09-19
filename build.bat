@echo off
msbuild -v:m -nologo -maxCpuCount console-game.vcxproj -p:Configuration=debug -p:Platform=x64 -t:Build
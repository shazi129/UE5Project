@set EditorPath=D:/Workspace/UnrealEngine
@set ProjectPath=D:/Workspace/UE_Workspace/UE5Project/UE5Project.uproject

call "%EditorPath%\GenerateProjectFiles.bat"
@echo Build Win64 Editor using EnginePath=%EnginePath%

%EditorPath%\Engine\Build\BatchFiles\Build.bat UE5ProjectEditor Win64 Development %ProjectPath%
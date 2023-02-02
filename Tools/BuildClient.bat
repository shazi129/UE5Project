set EditorPath=D:/Workspace/UnrealEngine
echo %EditorPath%

set ProjectPath=D:/Workspace/UE_Workspace/UE5Project/UE5Project.uproject
echo %ProjectPath%

set OutputPath=D:/Workspace/UE_Workspace/UE5Project_out
echo %OutputPath%

"%EditorPath%/Engine/Build/BatchFiles/RunUAT.bat" -ScriptsForProject="%ProjectPath%" Turnkey ^
	-command=VerifySdk -platform=Win64 -UpdateIfNeeded -EditorIO -project="%ProjectPath%" BuildCookRun -nop4 -utf8output -cook ^
	-project="%ProjectPath%"  -ue4exe="%EditorPath%/Engine/Binaries/Win64/UnrealEditor-Cmd.exe" -platform=Win64 ^
	-ddc=InstalledDerivedDataBackendGraph -installed -stage -archive -package -build -prereqs -archivedirectory="%OutputPath%" -config=Debug
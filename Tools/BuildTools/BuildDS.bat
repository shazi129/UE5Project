set EditorPath=C:\MetaWorld\UnrealEngine
echo %EditorPath%

set ProjectPath=C:\MetaWorld\UE5Project\UE5Project.uproject
echo %ProjectPath%

set OutputPath=C:\MetaWorld\UE5Project_DS_out
echo %OutputPath%

rm -rf %OutputPath%

"%EditorPath%/Engine/Build/BatchFiles/RunUAT.bat" -ScriptsForProject="%ProjectPath%" Turnkey -command=VerifySdk ^
    -platform=Win64 -UpdateIfNeeded -EditorIO -project="%ProjectPath%" BuildCookRun -nop4 -utf8output -cook  ^
    -project="%ProjectPath%"  -unrealexe="%EditorPath%/Engine/Binaries/Win64/UnrealEditor-Cmd.exe" ^
    -platform=Win64 -server -noclient -ddc=InstalledDerivedDataBackendGraph -installed -stage -archive -package -build -prereqs ^
    -archivedirectory="%OutputPath%" -nodebuginfo -config=Debug

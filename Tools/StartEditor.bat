rem set EditorExe=%~dp0..\..\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Win64-Debug.exe
set EditorExe=%~dp0..\..\UnrealEngine\Engine\Binaries\Win64\UnrealEditor.exe
set ProjectPath=%~dp0..\UE5Project.uproject

%EditorExe% %ProjectPath% -skipcompile
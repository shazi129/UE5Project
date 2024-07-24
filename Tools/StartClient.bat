set ProjectPath=%~dp0..\UE5Project.uproject
echo %ProjectPath%

set EditorExe=%~dp0..\..\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Win64-Debug.exe


set Map=/Game/MainContent/Login

%EditorExe% %ProjectPath% %Map% -log -LOG=Client.log -game

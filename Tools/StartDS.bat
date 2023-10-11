set EditorPath=C:\MetaWorld\UnrealEngine
echo %EditorPath%

set ProjectPath=C:\MetaWorld\UE5Project\UE5Project.uproject
echo %ProjectPath%

set EditorExe=C:\MetaWorld\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Win64-Debug.exe


set Map=/Game/MainContent/Maps/Main

%EditorExe% %ProjectPath% %Map% -log -LOG=DS.log -server -game -port=7777 -tracehost=127.0.0.1

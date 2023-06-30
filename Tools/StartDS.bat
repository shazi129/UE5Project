set EditorPath=D:\Workspace\UnrealEngine
echo %EditorPath%

set ProjectPath=D:/Workspace/UE_Workspace/UE5Project/UE5Project.uproject
echo %ProjectPath%

set EditorExe=D:\Workspace\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Win64-Debug.exe


set Map=/Game/MainContent/MyTestMap

%EditorExe% %ProjectPath% %Map% -log -LOG=DS.log -server -game -port=7777 -tracehost=127.0.0.1

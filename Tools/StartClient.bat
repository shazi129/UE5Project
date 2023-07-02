set ProjectPath=D:/Workspace/UE_Workspace/UE5Project/UE5Project.uproject
echo %ProjectPath%

set EditorExe=D:\Workspace\UnrealEngine\Engine\Binaries\Win64\UnrealEditor-Win64-Debug.exe


set Map=/Game/MainContent/Login

%EditorExe% %ProjectPath% %Map% -log -LOG=Client.log -game

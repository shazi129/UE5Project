set EditorPath=D:/Workspace/UnrealEngine
set ToolsPath=%~dp0

rem mkdir %ToolsPath%..\Content\Characters\
rem cp %EditorPath%/Templates/TemplateResources/High/Characters/Content/* ../Content/Characters/ -r

rem mkdir %ToolsPath%..\Content\LevelPrototyping\
rem cp %EditorPath%/Templates/TemplateResources/High/LevelPrototyping/Content/* ../Content/LevelPrototyping/ -r

mkdir %ToolsPath%..\Content\ThirdPerson\
cp %EditorPath%/Templates/TP_ThirdPersonBP/Content/ThirdPerson/Blueprints ../Content/ThirdPerson/ -r
cp %EditorPath%/Templates/TP_ThirdPersonBP/Content/ThirdPerson/Input ../Content/ThirdPerson/ -r
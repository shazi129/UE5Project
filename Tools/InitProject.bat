set EditorPath=D:/Workspace/UE/UnrealEngine
set ToolsPath=%~dp0

mkdir %ToolsPath%..\Content\Characters\
cp %EditorPath%/Templates/TemplateResources/High/Characters/Content/* ../Content/Characters/ -r

mkdir %ToolsPath%..\Content\LevelPrototyping\
cp %EditorPath%/Templates/TemplateResources/High/LevelPrototyping/Content/* ../Content/LevelPrototyping/ -r

mkdir %ToolsPath%..\Content\ThirdPerson\
cp %EditorPath%/Templates/TP_ThirdPersonBP/Content/ThirdPerson/Blueprints ../Content/ThirdPerson/ -r
cp %EditorPath%/Templates/TP_ThirdPersonBP/Content/ThirdPerson/Input ../Content/ThirdPerson/ -r
setx UE_Path D:\Workspace\UnrealEngine

set ToolsPath=%~dp0

mkdir %ToolsPath%..\Content\Characters\
cp %UE_Path%/Templates/TemplateResources/High/Characters/Content/* ../Content/Characters/ -r

mkdir %ToolsPath%..\Content\LevelPrototyping\
cp %UE_Path%/Templates/TemplateResources/High/LevelPrototyping/Content/* ../Content/LevelPrototyping/ -r

mkdir %ToolsPath%..\Content\ThirdPerson\
cp %UE_Path%/Templates/TP_ThirdPersonBP/Content/ThirdPerson/Blueprints ../Content/ThirdPerson/ -r
cp %UE_Path%/Templates/TP_ThirdPersonBP/Content/ThirdPerson/Input ../Content/ThirdPerson/ -r
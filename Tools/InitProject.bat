set EditorPath=D:/Workspace/UnrealEngine
set ToolsPath=%~dp0

mkdir %ToolsPath%..\Content\Characters\
cp %EditorPath%/Templates/TemplateResources/High/Characters/Content/* ../Content/Characters/ -r

set EditorPath=D:/Workspace/UnrealEngine
set ToolsPath=%~dp0

mkdir %ToolsPath%..\Content\EngineResources\Characters\
cp %EditorPath%/Templates/TemplateResources/High/Characters/Content/* ../Content/EngineResources/Characters/ -r
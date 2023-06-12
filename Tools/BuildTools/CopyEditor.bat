@set EnginePath=%~dp0..\..\UE5
@set OutEditorPath=%~dp0..\..\Out\MetaWorldEditor

xcopy %EnginePath%\Engine  %OutEditorPath%\Engine  /s /f /i /y /EXCLUDE:CopyEditorExclude.txt

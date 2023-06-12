@set WorkspacePath=%~dp0..\..
@set OutPath=%~dp0..\..\Out

xcopy %WorkspacePath%\MetaWorld  %OutPath%\MetaWorldArt  /s /f /i /y /EXCLUDE:CopyArtExclude.txt
xcopy %WorkspacePath%\MetaWorld\node_modules  %OutPath%\MetaWorldArt\node_modules  /s /f /i /y
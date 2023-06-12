@set ZipName=%~dp0..\..\Out\MetaWorldClient.zip

rm -rf %ZipName%
7z a -xr"@ZipClientExclude.txt" %ZipName% %~dp0..\..\Out\MetaWorldClient
@echo off
pushd %~dp0\..\
set pathToPremake=vendor\bin\premake\premake5.exe
if exist "%pathToPremake%" (
    call "%pathToPremake%" vs2022
    popd
    PAUSE
) else (
    echo "Please place premake.exe into the path <PROJECT-ROOT>\%pathToPremake%"
    PAUSE
)
@echo off
chcp 65001

::pdb 파일 삭제
set FILE_PATH=..\UmrealScripts\bin\Release\UmrealScripts.pdb

if exist "%FILE_PATH%" (
    :: PowerShell을 사용하여 GUID 생성
    for /f "delims=" %%i in ('powershell -command "[guid]::NewGuid().ToString()"') do set NEW_FILE_NAME=%%i

    :: 파일 삭제 시도
    del "%FILE_PATH%" 2>nul

    :: 삭제 실패하면 GUID로 파일 이름 바꾸기
    if exist "%FILE_PATH%" (
        ren "%FILE_PATH%" "%NEW_FILE_NAME%"
        echo 파일 이름이 바뀌었습니다: %NEW_FILE_NAME%
    ) else (
        echo 파일이 성공적으로 삭제되었습니다.
    )
) else (
    echo 파일이 존재하지 않습니다.
)

:: Visual Studio 환경 설정
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

:: 솔루션 파일 열고 빌드
msbuild "../GA6thFinal_Framework.sln" /p:Configuration=Release /t:Build /p:Platform=x64 /p:BuildProject=UmrealScripts

pause
@echo off
REM Script de compilation pour Windows
REM Utilise gcc si disponible, sinon cl (Visual Studio)

echo Compilation du projet BigBinary...

REM Tenter avec gcc
where gcc >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo Utilisation de GCC...
    gcc -Wall -Wextra -std=c99 -g -c bigbinary.c -o bigbinary.o
    gcc -Wall -Wextra -std=c99 -g -c main.c -o main.o
    gcc -Wall -Wextra -std=c99 -g -o bigbinary.exe bigbinary.o main.o
    echo Compilation terminee avec succes!
    echo.
    echo Pour executer le programme, tapez: bigbinary.exe
    goto :end
)

REM Tenter avec cl (Visual Studio)
where cl >nul 2>nul
if %ERRORLEVEL% EQU 0 (
    echo Utilisation de Microsoft C Compiler (cl)...
    cl /W4 /c bigbinary.c
    cl /W4 /c main.c
    cl /Fe:bigbinary.exe bigbinary.obj main.obj
    echo Compilation terminee avec succes!
    echo.
    echo Pour executer le programme, tapez: bigbinary.exe
    goto :end
)

echo.
echo ERREUR: Aucun compilateur C trouve!
echo.
echo Veuillez installer l'un des suivants:
echo   1. MinGW (pour gcc): https://www.mingw-w64.org/
echo   2. Visual Studio Build Tools (pour cl)
echo   3. WSL (Windows Subsystem for Linux)
echo.
goto :end

:end
pause


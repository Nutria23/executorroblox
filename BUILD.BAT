@echo off
chcp 65001
title Compilador Roblox Executor

echo ========================================
echo  Roblox Universal Executor - Compilador
echo ========================================

REM Crear directorios
if not exist "build" mkdir build
if not exist "bin" mkdir bin

REM Compilar DLL de inyección
echo [*] Compilando DLL de inyección...
cl /EHsc /LD /Fe:bin\RobloxInjector.dll /I. /link /DLL /SUBSYSTEM:WINDOWS dllmain.cpp kernel32.lib user32.lib

REM Compilar ejecutor principal
echo [*] Compilando ejecutor principal...
cl /EHsc /Fe:bin\RobloxExecutor.exe /I. /link /SUBSYSTEM:WINDOWS main.cpp gui.cpp /MANIFEST:NO kernel32.lib user32.lib advapi32.lib psapi.lib detours.lib comctl32.lib

REM Crear archivo de configuración
echo [*] Creando configuracion...
(
echo {
echo   "version": "2.0",
echo   "name": "Roblox Universal Executor",
echo   "antiBan": true,
echo   "autoUpdate": true,
echo   "injectionMethods": ["ManualMap", "ThreadHijack", "APC"],
echo   "supportedRobloxVersions": ["Player", "Studio", "Web", "UWP"]
echo }
) > bin\config.json

REM Crear README
echo [*] Creando documentacion...
(
echo # ROBLOX UNIVERSAL EXECUTOR
echo 
echo ## Caracteristicas:
echo - Inyeccion en cualquier version de Roblox
echo - Sistema Anti-Ban avanzado
echo - Ofuscacion automatica de codigo
echo - Multiple metodos de inyeccion
echo - Interfaz grafica profesional
echo - Templates predefinidos
echo 
echo ## Como usar:
echo 1. Ejecuta RobloxExecutor.exe
echo 2. Haz clic en "CONECTAR A ROBLOX"
echo 3. Escribe o selecciona un script
echo 4. Haz clic en "EJECUTAR SCRIPT"
echo 
echo ## Advertencia:
echo Solo para fines educativos.
echo El uso indebido puede resultar en ban.
echo ) > bin\README.txt

echo [+] Compilacion completada!
echo [!] Archivos en carpeta: bin\
pause

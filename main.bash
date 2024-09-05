@echo off
setlocal enabledelayedexpansion

:menu
cls
color 0B
echo ===========================================
echo          Bienvenido al Juego de Ahorcado
echo
echo         Desarrollado por Team Capibara :]
echo ===========================================
echo.
echo Instrucciones:
echo Por cada acierto, una letra se revelara.
echo Revela la palabra oculta y ganaras.
echo En cambio, si juntas 3 errores, perderas.
echo.
echo Modo de juego:
echo 1) Facil: Sumas y restas
echo 2) Dificil: Derivadas e integrales
echo 3) Salir
echo.
set /p choice="Selecciona el modo de juego (1 para Facil, 2 para Dificil, 3 para Salir): "

if "%choice%"=="1" (
    echo Has seleccionado el modo Facil.
    set "archivo_preguntas=preguntas_facil.txt"
    goto start_game
)
if "%choice%"=="2" (
    echo Has seleccionado el modo Dificil.
    set "archivo_preguntas=preguntas_dificil.txt"
    goto start_game
) 
if "%choice%"=="3" (
    echo Gracias por jugar!!
    echo Adios :)...
    exit /b
) else (
    echo Selección inválida, por favor intenta de nuevo.
    pause
    goto menu
)

:start_game
cls
echo Demos inicio...

:: Variables iniciales
set "palabra=PRUEBA"
set "intentos=0"
set "letras_desbloqueadas="
set "strlen=6"

:inicio
cls
echo ===========================================
echo          Bienvenido al Juego de Ahorcado
echo
echo         Desarrollado por Team Capibara :]
echo ===========================================
echo.

:: Mostrar progreso
call :mostrar_palabra

:: Verificar si el archivo de preguntas existe
if not exist "%archivo_preguntas%" (
    echo El archivo de preguntas "%archivo_preguntas%" no existe o no se puede acceder.
    pause
    goto menu
)

:: Mostrar nombre de archivo de preguntas
echo Archivo de preguntas: %archivo_preguntas%
pause

:: Seleccionar una línea al azar del archivo de preguntas
set "pregunta="
set "num_preguntas=0"
for /f "tokens=*" %%A in ('findstr /n "^" "%archivo_preguntas%"') do (
    set /a "num_preguntas+=1"
    set "pregunta_!num_preguntas!=%%A"
)

:: Generar un número aleatorio para seleccionar una pregunta
set /a "pregunta_aleatoria=!random! %% num_preguntas + 1"
set "pregunta=!pregunta_%pregunta_aleatoria%!"

:: Eliminar el número de línea al principio de la pregunta
set "pregunta_sin_numero=!pregunta:*:=!"

echo Pregunta seleccionada: !pregunta_sin_numero!
pause

:: Separar la pregunta y las opciones
for /f "tokens=1,2,3,4,5 delims=|" %%a in ("!pregunta_sin_numero!") do (
    set "pregunta_texto=%%a"
    set "opcion1=%%b"
    set "opcion2=%%c"
    set "opcion3=%%d"
    set "respuesta_correcta=%%e"
)

:: Mostrar la pregunta y opciones
echo Pregunta: !pregunta_texto!
echo 1) !opcion1!
echo 2) !opcion2!
echo 3) !opcion3!
set /p "respuesta=Selecciona la respuesta correcta (1/2/3): "

:: Comprobar respuesta
if "!respuesta!"=="1" (
    set "respuesta_seleccionada=!opcion1!"
) else if "!respuesta!"=="2" (
    set "respuesta_seleccionada=!opcion2!"
) else if "!respuesta!"=="3" (
    set "respuesta_seleccionada=!opcion3!"
) else (
    echo Ingresa una opción válida.
    pause
    goto inicio
)

if "!respuesta_seleccionada!"=="!respuesta_correcta!" (
    echo La respuesta es correcta!!
    set /a "indice_letra=!random! %% !strlen!"
    set "letras_desbloqueadas=!letras_desbloqueadas!!palabra:~!indice_letra!,1!"
) else (
    echo La respuesta es incorrecta.
    set /a "intentos+=1"
)

:: Fin del juego si se alcanzan 3 intentos fallidos
if %intentos% geq 3 (
    echo Vaya, has perdido :(
    pause
    exit
)

:: Verificar si has ganado
call :verificar_ganador

goto inicio

:mostrar_palabra
set "progreso="
for /l %%i in (0,1,%strlen%) do (
    set "letra=!palabra:~%%i,1!"
    if "!letras_desbloqueadas:%letra%=!" neq "!letras_desbloqueadas!" (
        set "progreso=!progreso!!letra! "
    ) else (
        set "progreso=!progreso!_ "
    )
)

echo !progreso!
goto :eof

:verificar_ganador
set "ganado=1"
for /l %%i in (0,1,%strlen%) do (
    if "!progreso:~%%i,1!" equ "_" (
        set "ganado=0"
    )
)
if "!ganado!"=="1" (
    echo Felicidades, has ganado!
    pause
    exit
)
goto :eof





    
    
    
    
    
    
    




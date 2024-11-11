@echo off
generate-testcase.exe
setlocal enabledelayedexpansion
set success=0
for /l %%i in (1,1,30) do (
    set infile=test%%i.txt

    set results=result-screen%%i.hex
    set resultf=result-file%%i.hex
    set results0=result0-screen%%i.hex
    set resultf0=result0-file%%i.hex

    15-b4 --infile !infile! > !results!
    15-b4 --infile !infile! --outfile !resultf!

    15-b1-demo --infile test%%i.txt > !results0!
    15-b1-demo --infile test%%i.txt --outfile !resultf0!

    echo N | comp !results! !results0! > nul
    set code1=!errorlevel!
    if !errorlevel! == 0 (
        echo test%%i : to screen accepted.
        del !results!
        del !results0!
    ) else (
        echo test%%i : to screen failed, files are saved.
    )
    echo N | comp !resultf! !resultf0! > nul
    set code2=!errorlevel!
    if !errorlevel! == 0 (
        echo         to file   accepted.
        del !resultf!
        del !resultf0!
    ) else (
        echo         to file   failed, files are saved.
    )

    if !code1! == 0 if !code2! == 0 (
        del !infile!
        set /a success+=1
    )
    echo .
)
echo all 30 cases, success !success! cases.
endlocal
pause

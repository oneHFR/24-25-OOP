@echo off
generate-testcase.exe
setlocal enabledelayedexpansion
set success=0
for /l %%i in (1,1,30) do (
    set infile=test%%i.txt
    set temp=temp%%i.hex
    set result=result%%i.txt

    15-b1-demo --infile !infile! --outfile !temp!
    15-b5 --infile !temp! --outfile !result!

    echo N | comp !result! !infile! > nul
    if !errorlevel! == 0 (
        echo test%%i : accepted.
        del !result!
        del !temp!
        del !infile!
        set /a success+=1
    ) else (
        echo test%%i : failed, files are saved.
    )
    echo .
)
echo all 30 cases, success !success! cases.
endlocal
pause
@ECHO OFF
"C:\Program Files\Atmel\AVR Tools\AvrAssembler2\avrasm2.exe" -S "C:\FPC\Work_AVR\TestINT\labels.tmp" -fI -W+ie -o "C:\FPC\Work_AVR\TestINT\TestINT.hex" -d "C:\FPC\Work_AVR\TestINT\TestINT.obj" -e "C:\FPC\Work_AVR\TestINT\TestINT.eep" -m "C:\FPC\Work_AVR\TestINT\TestINT.map" "C:\FPC\Work_AVR\TestINT\TestINT.asm"

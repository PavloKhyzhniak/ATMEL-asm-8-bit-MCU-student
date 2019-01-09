cd "D:\asm\MonU3\"
D:
del "D:\asm\MonU3\monu3.map"
del "D:\asm\MonU3\monu3.lst"
"C:\Program Files\Atmel\AVR Tools\AvrAssembler\avrasm32.exe" -fI "D:\asm\MonU3\MonU3.asm" -o "D:\asm\MonU3\monu3.hex" -d "D:\asm\MonU3\monu3.obj" -e "D:\asm\MonU3\monu3.eep" -I "D:\asm\MonU3" -I "C:\Program Files\Atmel\AVR Tools\AvrAssembler\AppNotes" -w  -m "D:\asm\MonU3\monu3.map" -l "D:\asm\MonU3\monu3.lst"

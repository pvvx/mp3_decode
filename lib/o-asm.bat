@echo off
@if %1x==x goto fr1
md %1.o
cd %1.o
C:\Espressif\xtensa-lx106-elf\bin\xtensa-lx106-elf-ar x ..\%1
for %%o in (*.o) do C:\Espressif\xtensa-lx106-elf\bin\xtensa-lx106-elf-objdump -S %%o > %%o.asm
cd ..
goto end
:fr1
for %%a in (*.a) do (
md %%a.o
cd %%a.o
C:\Espressif\xtensa-lx106-elf\bin\xtensa-lx106-elf-ar x ..\%%a
for %%o in (*.o) do C:\Espressif\xtensa-lx106-elf\bin\xtensa-lx106-elf-objdump -S %%o > %%o.asm
cd ..
)
:end

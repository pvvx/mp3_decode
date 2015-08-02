@del libmphy.a
@md libphy.lib
@cd libphy.lib
@C:\Espressif\xtensa-lx106-elf\bin\xtensa-lx106-elf-ar x ..\libphy.a
@rem phy_chip_v5_ana_romfunc.o phy_chip_v5_cal_romfunc.o phy_chip_v5_romfunc.o 
@rem phy.o phy_chip_v6.o phy_chip_v6_ana.o phy_chip_v6_cal.o phy_sleep.o
@rem delete: ate_test.o phy_chip_v6_unused.o
@del ate_test.o
@del phy_chip_v6_unused.o
@C:\Espressif\xtensa-lx106-elf\bin\xtensa-lx106-elf-ar ru ..\libmphy.a *.o
@cd ..
@rd /q /s libphy.lib

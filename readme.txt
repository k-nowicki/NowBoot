/-----------------------------------------------------------------------------/
/ NowBoot is a Bootloader module for small embedded systems which
/ working with FreeRTOS.
/ With few changes in boot.s containing exception vectors
/ it can be use with any other firmware.
/ C 2010 -2011, Karol Nowicki
/
/ NowBoot contain some code which is provided by third-parties like:
/ 	-FatFs
/	-Atmel peripherial drivers
/	-FreeRTOS (sample boot.s startup file)
/ AND IS LICENSED SEPARATELY FROM NowBoot.
/
/
/
/ This is a free software that opened for education, research and commercial
/ developments under license policy of following terms.
/
/
/ * The NowBoot module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-----------------------------------------------------------------------------/
[PL]
NowBoot jest otwartym, kompletnym bootloaderem dla mikrokontrolerów SAM7 firmy Atmel, 
ładującym program z pliku umieszczonego na karcie SD (ale nie tylko na karcie).

NowBoot jest umieszczany na samym początku pamięci programu (od adresu 0x00) i od niego 
zaczyna się wykonywanie programu. Sprawdza, czy na lokalnym dysku jest plik z 
nowym firmwarem i przepisuje jego zawartość do pamieci flash. 
Po tej operacji plik ma zmienianą nazwę. 
Jeżeli plik z firmwarem nie zostanie znaleziony, NowBoot sprawdza czy pamięć pod adresem 
startowym firmware`u jest zapisana (wartością inną niż 0xFF). Jeżeli nie, szuka pliku z 
firmwarem podstawowym i zapisuje go w pamiêci flash. Kiedy wszystkie operacje się zakończą,
sterowanie jest przekazywane do firmware`u. 
W praktyce, wystarczy więc umieścić np na karcie SD plik z nowym firmwarem i zrestartować procesor, 
aby zaktualizować firmware.


*Gdzie mogę zdobyć dodatkowe informacje w temacie bootloaderów?
1)google: bootloader	//ogólnie o bootloaderach
2)atmel.com: 		//w kontekście SAM7:
	a)"Safe and Secure Firmware Upgrade for AT91SAM Microcontrollers"
	b)"Safe and Secure Bootloader Implementation"
	c)"AT91SAM7x512/256/128 Preliminary" 
		->"Memory Controller" and "Embedded Flash Controller" sections


*Jak umieścić bootloader w pamięci razem z programem?
NowBoot w oryginalnej wersji zakłada, że aplikacja zaczyna się od adresu 0x8000 (32K).
W skrypcie linkera swojego firmware`u musisz więc deklarować odpowiednio początek pamięci 
flash (flash_start + 32K) oraz jej wielkość (flash_length - 32K). 
Następnie są dwie możliwości:
-Zaprogramować pamięć programu jednocześnie obydwoma plikami ustawiając w opcjach programatora 
adresy początkowe ręcznie (rozwiązanie zalecane)
-Binarnie połączyć pliki w jeden wypełniając przestrzeń między nimi wartościami 0xFF (NOP)- ręcznie raczej odpada.



======================
TO DO

	-Sprawdzania poprawności zapisu programu w pamięci flash- MD5. 
	-Szyfrowania Firmware`u
	-Zabezpieczenie przed odczytem firmware`u

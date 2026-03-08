## Príprava pred používaním
1. Nastavte microSD kartu pomocou: https://www.ev3dev.org/docs/getting-started/
2. Stiahnite a presunte main subory na SD kartu do  particie `ROOTFS` a priečinku `/home/robot/libc++/`
3. Na Windows nainštalujte  `putty`(pscp.exe musý byť v tom istom súbore ako executable nášho vlastného programovacieho jazyka) a na Linuxe `shh` a  `sshpass`
4. Stiahnite náš vlastný programovací jazyk (žiadna inštalácia není potrebná)
5. Všetko je pripravené na to aby ste začali programovať, PS: nezabudnite nastaviť konštanty robota

## Základné hodnoty
- program_1.prgm je spustený z main1 (program_2.prgm z main2)
- robot_1.conf je využívaný v main1 pre triedu Robot (robot_2.conf je využívaný v main2)
### Pripojenie motorov
- ľavé rameno  -> A
- ľavé koleso    -> B
- práve koleso -> C
- práve koleso -> D
### Pripojenie Senzorov
- gyro  -> 1
- farba lavá -> 2
- farba pravá -> 3
## Pozicia
- uhol 0 je na osi Y
- uhol je v rad
- uhol ide v protismere hodinových ručičiek (vľavo je kladné,vpravo je záporné)
## vlastný programovací jazyk M++
- pre posielanie súborov na  linuxe `sshpass` a `ssh` musý byť nainštalovaný, Pre Windows `putty` musý byť nainstalovaný (pscp.exe musý byť v tom istom súbore ako executable nášho vlastného programovacieho jazyka)
- podporuje Anglictinu a Slovenčinu 
- Pre viac informácií si precitajte [príručku pre M++](prirucka_m++.txt)
## Nápomocné linky
- https://www.ev3dev.org/
- https://github.com/ev3dev/ev3dev/issues/1032
- https://docs.ev3dev.org/en/ev3dev-stretch/

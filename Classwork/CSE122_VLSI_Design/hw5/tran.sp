
** 1. TRY THE DIFFERENT CORNERS FOR THE WORST CASE **
.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" tt
*.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" ff
*.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" ss
*.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" fs
*.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" sf


* Valid minimum L=0.15 tx widths
* nfet: 0.36, 0.39, 0.42, 0.52, 0.54, 0.55, 0.58, 0.6, 0.61, 0.64, 0.65, 0.74, 0.84, 1.0, 1.26, 1.68, 2.0, 3.0, 5.0, 7.0
* pfet: 0.42, 0.55, 0.64, 0.84, 1.0, 1.12, 1.26, 1.65, 1.68, 2.0, 3.0, 5.0, 7.0


** 2. UNCOMMENT THE APPROPRIATE TEST INSTANCE **

*.include 'sram_sp_cell.sp'
* Only use one of these. _sim for DC and other for TRAN
*Xbitcell BL0 BR0 0 VDD VDD 0 WL0 Q QB QD QBD sram_sp_cell_sim
*Xbitcell BL0 BR0 0 VDD VDD 0 WL0 sram_sp_cell
*8CBL0 BL0 0 {BITCAP}
*CBR0 BR0 0 {BITCAP}
*.IC V(BL0)='VDD'
*.IC V(BR0)='VDD'

*.include 'sram_dp_cell.sp'
* Only use one of these. _sim for DC and other for TRAN
*Xbitcell BL0 BR0 BL1 BR1 WL0 WL1 VDD 0 Q QB QD QBD sram_dp_cell_sim
*Xbitcell BL0 BR0 BL1 BR1 WL0 WL1 VDD 0 sram_dp_cell
*CBL0 BL0 0 {BITCAP}
*CBR0 BR0 0 {BITCAP}
*CBL1 BL1 0 {BITCAP}
*CBR1 BR1 0 {BITCAP}
*.IC V(BL0)='VDD'
*.IC V(BR0)='VDD'
*.IC V(BL1)='VDD'
*.IC V(BR1)='VDD'
*

 .include 'ex_cell_1rw.sp'
* Only use one of these. _sim for DC and other for TRAN
* Xbitcell BL0 BR0 0 VDD VDD 0 WL0 Q QB QD QBD cell_1rw_sim
 Xbitcell BL0 BR0 0 VDD VDD 0 WL0 cell_1rw
 CBL0 BL0 0 {BITCAP}
 CBR0 BR0 0 {BITCAP}
 .IC V(xbitcell.q) = 0
 .IC V(BL0)='VDD'
 .IC V(BR0)='VDD'

*.include 'cell_2rw.sp'
* Only use one of these. _sim for DC and other for TRAN
*Xbitcell BL0 BR0 BL1 BR1 WL0 WL1 VDD 0 Q QB QD QBD cell_2rw_sim
*Xbitcell BL0 BR0 BL1 BR1 WL0 WL1 VDD 0 cell_2rw
* CBL0 BL0 0 {BITCAP}
* CBR0 BR0 0 {BITCAP}
* CBL1 BL1 0 {BITCAP}
* CBR1 BR1 0 {BITCAP}

* .IC V(BL0)='VDD'
* .IC V(BR0)='VDD'
* .IC V(BL1)='VDD'
* .IC V(BR1)='VDD'


*.options list post
.temp 25
.param VDD=1.8V
.param UL='-VDD/sqrt(2)'
.param UH='VDD/sqrt(2)'
.param BITCAP=1pF

Vdd VDD 0 dc 'VDD'


** 3. UNCOMMENT THE APPROPRIATE TEST SETTINGS **
*****Read****
*VWL0 WL0 0 DC 'VDD'
*VWL1 WL1 0 DC 'VDD'
*****Hold****
*VWL0 WL0 0 DC 0
*VWL1 WL1 0 DC 0
*****Write***
VBL0 BL0 GND PULSE (0 1.8 0n 5n 5n 40n 100n)
VBR0 BR0 GND PULSE (1.8 0 0n 5n 5n 40n 100n)
VWL0 WL0 GND PULSE(0 1.8 0n 1n 1n 75n 200n)
*.IC V(BL1)=VDD
*.IC V(BR1)=VDD
*VWL1 WL1 0 DC 0v


** 4. DC SNM ANALYSIS **
** To use these, you SHOULD use the _sim bitcell subckt
*VU U 0 DC=0V
** use voltage controlled voltage sources (VCVS)
** changing the co-ordinates in 45 degree angle
*EQ Q 0 VOL='1/sqrt(2)*V(U) + 1/sqrt(2)*V(V1)'
*EQB QB 0 VOL='-1/sqrt(2)*V(U) + 1/sqrt(2)*V(V2)'
** inverter characteristics
*EV1 V1 0 VOL='V(U) + sqrt(2)*V(QBD)'
*EV2 V2 0 VOL='-V(U) + sqrt(2)*V(QD)'
** take the absolute value for determination of SNM
*EVD VD 0 VOL='ABS(V(V1) - V(V2))'
*.DC VU 'UL' 'UH' 0.01
*.MEASURE DC MAXVD MAX V(VD)
*.MEASURE DC SNM param='1/sqrt(2)*MAXVD'
*.control
*run
*plot V(V1) V(V2) V(VD)
*plot V(QD) V(QBD)
**quit
*.endc

** 5. TRAN WRITE ANALYSIS **
** To use these, you should NOT use the _sim bitcell subckt
.TRAN 0.5n 125n
.control
run
plot V(WL0) V(BL0) V(BR0)
plot V(Xbitcell.Q) V(Xbitcell.QB)
*quit
.endc



.end

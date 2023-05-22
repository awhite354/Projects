* comment

.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" ff
.include "OSC.spice"

.param supply_voltage = '1.8V'

.option temp=27

Xosc EN SEL VDD 0 OUT OSC

Vvdd VDD 0 1.8
Ven EN 0 1.8
*Vsel 0 1.8
Vsel A 0 pulse(0 1.8 0ns 1ns 1ns 5ns 10ns) DC 0V

.DC Vsel 0 'supply_voltage' 0.1

.param half_supply = '0.5 * supply_voltage'
.param slew_low = '.1 * supply_voltage'
.param slew_high = '.9 * supply_voltage'

.tran 0.004ns 50ns
.meas tran fall_time trig v(OUT) val=slew_high fall=1 targ v(OUT) val=slew_low fall=1
.print TRAN v(OUT)

.END

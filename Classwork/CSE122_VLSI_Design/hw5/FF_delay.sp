* comment

.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" tt
.include "sky130_fd_sc_hd__dfxtp_1.spice"

.param supply_voltage = '1.8V'
.param half_supply = '0.5 * supply_voltage'
.param slew_low = '.1 * supply_voltage'
.param slew_high = '.9 * supply_voltage'

.option temp=27

Xff CLK D 0 VPWR Q 0 VPB sky130_fd_sc_hd__dfxtp_1

Vvpwr VPWR 0 supply_voltage
Vvpb VPB 0 supply_voltage
Vd D 0 supply_voltage
Vclk CLK 0 PULSE(0 1.8 20ns 0.001ns 0.001ns 10ns 20ns)


.tran 0.001ns 40ns
.meas tran ideal_clk_q trig v(CLK) val=half_supply rise=1 targ v(Q) val=half_supply rise=1
.print TRAN v(CLK)
.print TRAN v(Q)

.control
run
quit
.endc

.END

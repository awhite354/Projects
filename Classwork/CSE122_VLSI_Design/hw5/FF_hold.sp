*finds the FF_setup_time within 10% of the Ideal CLK-Q time

.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" tt
.include "sky130_fd_sc_hd__dfxtp_1.spice"

.param supply_voltage = '1.8V'
.param half_supply = '0.5 * supply_voltage'

.option temp=27

Xff CLK D 0 VPWR Q 0 VPB sky130_fd_sc_hd__dfxtp_1

Vmid MID 0 half_supply
Vvpwr VPWR 0 supply_voltage
Vvpb VPB 0 supply_voltage
Vd D 0 PULSE(0 1.8 1.5ns 0.001ns 0.001ns .5105ns 5ns)
Vclk CLK 0 PULSE(0 1.8 2ns 0.001ns 0.001ns 1ns 2.5ns)


.tran 0.001ns 20ns
.meas tran clk_q_time trig v(CLK) val=half_supply rise=3 targ v(Q) val=half_supply rise=1
.meas tran hold_time trig v(CLK) val = half_supply rise=3 targ v(D) val=half_supply fall=2
.print TRAN v(CLK)
.print TRAN v(Q)

.control 
run
quit
.endc


.END

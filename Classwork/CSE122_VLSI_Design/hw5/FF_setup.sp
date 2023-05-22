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
*Vd D 0 PULSE(0 1.8 1.95ns 0.001ns 0.001ns 5ns 10.001ns)
Vd D 0 PULSE(0 1.8 2ns 0.001ns 0.001ns 5ns 10ns)
*Vclk CLK 0 PULSE(0 1.8 2ns 0.001ns 0.001ns 1ns 5s)
Vclk CLK 0 PULSE(0 1.8 2.0411ns 0.001ns 0.001ns 1ns 5ns)


.tran 0.001ns 20ns
.meas tran clk_q_time trig v(CLK) val=half_supply rise=3 targ v(Q) val=half_supply rise=1
.meas tran setup_time trig v(D) val = half_supply rise=2 targ v(CLK) val=half_supply rise=3
.print TRAN v(CLK)
.print TRAN v(Q)

.control
run
quit
.endc


.END

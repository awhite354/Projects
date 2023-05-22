* Example Inverter with Transient Analysis and Measure

* NOTE: You must have a .spiceinit file in this same directory with this contents:
* set ngbehavior=hsa
* Optional content to turn on multithreading to speed up simulations:
* set num_threads=3

* include the MOSFET models with TT proccess 
.lib "/software/PDKs/sky130A/libs.tech/ngspice/sky130.lib.spice" tt

* include the standard cell library
.include '/software/PDKs/sky130A/libs.ref/sky130_fd_sc_hd/spice/sky130_fd_sc_hd.spice'

* our supplies are global to the hierarchy
*.global vdd gnd
.param supply_voltage=1.8V

* set the operating temperature
.option temp=27

* include the circuit to be simulated
Xnand A B gnd gnd vdd vdd Z sky130_fd_sc_hd__nand2_2

* fanout 4 capacitive load on inverter output
Xinv1 Z gnd gnd vdd vdd Z1 sky130_fd_sc_hd__inv_1
Xinv2 Z gnd gnd vdd vdd Z2 sky130_fd_sc_hd__inv_1
Xinv3 Z gnd gnd vdd vdd Z3 sky130_fd_sc_hd__inv_1
Xinv4 Z gnd gnd vdd vdd Z4 sky130_fd_sc_hd__inv_1

* define the supply voltages
VDD vdd 0 supply_voltage
*VGND gnd 0 0V
VA A 0 supply_voltage


* create a voltage pulse on the input
VB B 0 PULSE(1.8V 0V 500ps 5ps 5ps 1000ps 2000ps)

* perform a 3ns transient analysis
.tran .001ns 3ns

.param half_supply = '0.5*supply_voltage'


* measure the input fall to the output rise time
.meas tran rise_output_delay trig v(B) val=half_supply fall=1 targ v(Z) val=half_supply rise=1


.control
run
quit
.endc

.END

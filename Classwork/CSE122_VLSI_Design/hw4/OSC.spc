*Handwritten SPC file OSC.spc

.include "/software/PDKs/sky130A/libs.ref/sky130_fd_sc_hd/spice/sky130_fd_sc_hd.spice"

.subckt OSC EN SEL VDD VGND OUT

XNAND MUX_OUT EN VGND VGND VDD VDD NAND_OUT sky130_fd_sc_hd__nand2_1
XINV1 NAND_OUT VGND VGND VDD VDD OUT sky130_fd_sc_hd__inv_1
XINV2 OUT VGND VGND VDD VDD A0 sky130_fd_sc_hd__inv_1
XMUX A0 A1 SEL VGND VGND VDD VDD MUX_OUT sky130_fd_sc_hd__mux2_1
XINV3 A0 VGND VGND VDD VDD S1 sky130_fd_sc_hd__inv_1      
XINV4 S1 VGND VGND VDD VDD S2 sky130_fd_sc_hd__inv_1      
XINV5 S2 VGND VGND VDD VDD S3 sky130_fd_sc_hd__inv_1      
XINV6 S3 VGND VGND VDD VDD A1 sky130_fd_sc_hd__inv_1      
XTAP VGND VGND VDD VDD sky130_fd_sc_hd__tap_1
.ends

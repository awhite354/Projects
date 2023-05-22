* Valid minimum L=0.15 tx widths
* nfet: 0.36, 0.39, 0.42, 0.52, 0.54, 0.55, 0.58, 0.60, 0.61, 0.64, 0.65, 0.74, 0.84, 1.0, 1.26, 1.68, 2.0, 3.0, 5.0, 7.0
* pfet: 0.42, 0.55, 0.64, 0.84, 1.00, 1.12, 1.26, 1.65, 1.68, 2.00, 3.00, 5.00, 7.00

*weak
.param pfet_inv_size='.42'
*medium
.param nfet_axis_size='.42'
*strong
.param nfet_inv_size='.64'

.subckt cell_1rw_sim BL BR VGND VPWR VPB VNB WL Q QB QD QBD
X0 QBD WL BR VNB sky130_fd_pr__nfet_01v8 w=nfet_axis_size l=0.15
X2 BL WL QD VNB sky130_fd_pr__nfet_01v8 w=nfet_axis_size l=0.15

X1 QD QB VGND VNB sky130_fd_pr__nfet_01v8 w=nfet_inv_size l=0.15
X6 QD QB VPWR VPB sky130_fd_pr__pfet_01v8 w=pfet_inv_size l=0.15

X5 VPWR Q QBD VPB sky130_fd_pr__pfet_01v8 w=pfet_inv_size l=0.15
X7 VGND Q QBD VNB sky130_fd_pr__nfet_01v8 w=nfet_inv_size l=0.15
.ends


* Same cell as above but with Q and QD as well as QB and QBD shorted
.subckt cell_1rw GND WL Q QB BL BR VDD VPB VNB
X0 QB WL BR VNB sky130_fd_pr__nfet_01v8 w=nfet_axis_size l=0.15
X2 BL WL Q VNB sky130_fd_pr__nfet_01v8 w=nfet_axis_size l=0.15

X1 Q QB GND VNB sky130_fd_pr__nfet_01v8 w=nfet_inv_size l=0.15
X6 Q QB VDD VPB sky130_fd_pr__pfet_01v8 w=pfet_inv_size l=0.15

X5 VDD Q QB VPB sky130_fd_pr__pfet_01v8 w=pfet_inv_size l=0.15
X7 GND Q QB VNB sky130_fd_pr__nfet_01v8 w=nfet_inv_size l=0.15
.ends

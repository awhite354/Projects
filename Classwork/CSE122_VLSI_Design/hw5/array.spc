* comment

.subckt cell_1rw GND WL Q QB BL BR VDD VNB VPB
X0 Q WL BL VNB sky130_fd_pr__nfet_01v8 ad=4.596e+11p pd=2.95e+06u as=1.386e+11p ps=1.5e+06u w=420000u l=150000u
X1 VDD QB Q VPB sky130_fd_pr__pfet_01v8 ad=2.352e+11p pd=2.8e+06u as=1.68e+11p ps=1.64e+06u w=420000u l=150000u
X2 QB WL BR VNB sky130_fd_pr__nfet_01v8 ad=4.596e+11p pd=2.95e+06u as=1.386e+11p ps=1.5e+06u w=420000u l=150000u
X3 Q QB GND VNB sky130_fd_pr__nfet_01v8 ad=0p pd=0u as=3.584e+11p ps=3.68e+06u w=640000u l=150000u
X4 GND Q QB VNB sky130_fd_pr__nfet_01v8 ad=0p pd=0u as=0p ps=0u w=640000u l=150000u
X5 VDD Q QB VPB sky130_fd_pr__pfet_01v8 ad=0p pd=0u as=1.68e+11p ps=1.64e+06u w=420000u l=150000u
.ends

.subckt array vdd gnd WL1 WL2 WL3 BL1 BR1 BL2 BR2 BL3 BR3
Xcell_1rw_0 gnd WL1 cell_1rw_0/Q cell_1rw_0/QB BL1 BR1 vdd gnd vdd cell_1rw
Xcell_1rw_1 gnd WL1 cell_1rw_1/Q cell_1rw_1/QB BL2 BR2 vdd gnd vdd cell_1rw
Xcell_1rw_2 gnd WL1 cell_1rw_2/Q cell_1rw_2/QB BL3 BR3 vdd gnd vdd cell_1rw
Xcell_1rw_4 gnd WL2 cell_1rw_4/Q cell_1rw_4/QB BL2 BR2 vdd gnd vdd cell_1rw
Xcell_1rw_3 gnd WL2 cell_1rw_3/Q cell_1rw_3/QB BL1 BR1 vdd gnd vdd cell_1rw
Xcell_1rw_5 gnd WL2 cell_1rw_5/Q cell_1rw_5/QB BL3 BR3 vdd gnd vdd cell_1rw
Xcell_1rw_6 gnd WL3 cell_1rw_6/Q cell_1rw_6/QB BL1 BR1 vdd gnd vdd cell_1rw
Xcell_1rw_7 gnd WL3 cell_1rw_7/Q cell_1rw_7/QB BL2 BR2 vdd gnd vdd cell_1rw
Xcell_1rw_8 gnd WL3 cell_1rw_8/Q cell_1rw_8/QB BL3 BR3 vdd gnd vdd cell_1rw
.end


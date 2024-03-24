# [WIP] Yosys scan chain stitcher plugin

This is a prototype Yosys pass for scan chain stitching all flip flops in a
given module. The idea is that this could be useful in a FPGA retro gaming
emulation setting where one wants to implement save states without having to
modify the source code RTL.

The included example flow reads some verilog, translates processes to internal
cell netlists, split nets and split cells so that we are left with single bit
flip flops.

The `scanchain` pass iterates over these flip flops (currently only the `$dff`
type) and stitches them into a long chain connecting to newly added `scan_in`
and `scan_out` ports of the module. The scan mode is operational when `scan_en`
is asserted basically turning the entire modules into a big shift register. The
`scan_len` output indicate the total length of the scan chain.

In the design the parent module could use these ports to extract and restore
the full FF state of the module at will. Memories still need to be handled
separately though but typically that is less of a problem since they should be
clearly instantiated.

** Just an idea and not at all tested in reality :) **

## Setup

If needed install a recent [Yosys](https://github.com/YosysHQ/yosys)
```
git clone https://github.com/YosysHQ/yosys.git
cd yosys/
make config-clang
PREFIX=/home/markus/work/install/ make install
```

Build the plugin pass
```
$ yosys-config --build scanchain.so scanchain.cc
```

## Usage
Run the stitcher
```
$ yosys -m ./scanchain.so -s flow.ys
```

Run test testbench and examine results
```
$ iverilog testbench.v cntr.scan.v -o test-cntr && ./test-cntr
$ gtkwave dump.vcd
```

`default_nettype none

module tb;

  reg clk;
  reg rst;
  wire [7:0] result;

  reg scan_en;
  wire scan_loop;


  counter dut(.clk(clk),
              .rst(rst),
              .result(result),
              .scan_en(scan_en),
              .scan_in(scan_loop),
              .scan_out(scan_loop));

  initial begin
    clk = 0;
    forever #1 clk = ~clk;
  end

  initial begin
    rst = 1;
    #2;
    rst = 0;
  end

  initial begin
    $dumpfile("dump.vcd");
    $dumpvars();
    #100
    $finish;
  end

  initial begin
    scan_en = 0;
    #50
    scan_en = 1;
    #16
    scan_en = 0;
  end

endmodule

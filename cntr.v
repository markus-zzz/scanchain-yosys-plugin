`default_nettype none

module counter(input clk, input rst, output [7:0] result);

  reg [7:0] cntr;

  assign result = cntr;

  always @(posedge clk) begin
    if (rst)
      cntr <= 0;
    else
      cntr <= cntr + 1;
  end

endmodule

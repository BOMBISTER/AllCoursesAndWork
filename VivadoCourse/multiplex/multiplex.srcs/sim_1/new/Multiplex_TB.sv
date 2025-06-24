`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 19.02.2024 14:51:15
// Design Name: 
// Module Name: Multiplex_TB
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Multiplex4_TB();
logic [3:0] in;
logic [1:0] sel;
logic q;

Multiplex4 DUT(.in(in), .sel(sel), .q(q));
initial begin
    sel = 2'b00; in = 4'h1; #10;
    repeat (3) begin
        sel = sel +1; in = in << 1; #10;
    end    
$finish;
end

endmodule

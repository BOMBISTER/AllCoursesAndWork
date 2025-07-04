`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 19.02.2024 15:46:45
// Design Name: 
// Module Name: muxer8_TB
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


module Multiplex8_TB();
logic [7:0] in;
logic [2:0] sel;
logic q;

muxer8 DUT(.in(in), .sel(sel), .q(q));
initial begin
    sel = 3'b000; in = 8'h1; #10;
    repeat (7) begin
        sel = sel + 1; in = in << 1; #10;
    end  
    sel = 3'b000; in = 8'hfe; #10;
    repeat (7) begin
        sel = sel + 1; in = (in << 1) + 1; #10;
    end   
$finish;
end

endmodule



module Multiplex64_TB();
logic [63:0] in;
logic [5:0] sel;
logic q;

muxer64 DUT(.in(in), .sel(sel), .q(q));
initial begin
    sel = 6'b000000; in = 64'h1; #10;
    repeat (63) begin
        sel = sel + 1; in = in << 1; #10;
    end  
    sel = 6'b000000; in = 64'hfffffffffffffffe; #10;
    repeat (63) begin
        sel = sel + 1; in = (in << 1) + 1; #10;
    end   
$finish;
end

endmodule

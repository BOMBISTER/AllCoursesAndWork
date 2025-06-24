`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 17:37:28
// Design Name: 
// Module Name: prcoder8_TB
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


module prcoder8_TB();
logic [7:0] in;
logic [2:0] code;

prcoder DUT(.in(in), .code(code));

initial begin
    in = 8'b00000000; #10;
    repeat (255) begin
        in = in + 1; #10;
    end
$finish;
end
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 16:17:05
// Design Name: 
// Module Name: prcoder_TB
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


module prcoder_TB();
logic [3:0] in;
logic [1:0] code;

prcoder DUT(.in(in), .code(code));

initial begin
    in = 4'b0000; #10;
    repeat(15) begin
        in = in + 1; #10;
    end
$finish;
end
endmodule

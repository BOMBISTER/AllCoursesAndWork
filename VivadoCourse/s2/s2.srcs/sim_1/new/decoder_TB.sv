`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 15:20:17
// Design Name: 
// Module Name: decoder_TB
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


module decoder_TB();
logic [1:0] code;
logic [3:0] q;
decoder DUT(.code(code), .q(q));

initial begin
    code = 2'b00; #10;
    code = 2'b01; #10;
    code = 2'b10; #10;
    code = 2'b11; #10;
$finish;
end
endmodule

`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 17:15:25
// Design Name: 
// Module Name: decoder8_TB
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


module decoder8_TB();
logic [2:0] code;
logic [7:0] q;
decoder8 DUT(.code(code), .q(q));

initial begin
    code = 3'b000; #10;
    code = 3'b001; #10;
    code = 3'b010; #10;
    code = 3'b011; #10;
    code = 3'b100; #10;
    code = 3'b101; #10;
    code = 3'b110; #10;
    code = 3'b111; #10;
$finish;
end
endmodule

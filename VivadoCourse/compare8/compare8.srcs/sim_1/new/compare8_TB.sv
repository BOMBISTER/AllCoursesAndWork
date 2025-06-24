`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 26.02.2024 16:03:37
// Design Name: 
// Module Name: compare8_TB
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


//module compare2(
//    input [1:0] a,
//    input [1:0] b,
//    output greater,
//    output less
//    );
//    assign greater = a[1] & !b[1]
//                    | !a[1] & !b[1] & a[0] & !b[0]
//                    | a[1] & b[1] & a[0] & !b[0];
//    assign less = !a[1] & b[1]
//                    | a[1] & b[1] & !a[0] & b[0]
//                    | !a[1] & !b[1] & !a[0] & b[0];                 
//endmodule


module compare8_TB();
logic [7:0] a,b;
logic       GREATER, LESS;

compare8 DUT(.a(a), .b(b), .GREATER(GREATER), .LESS(LESS));

initial begin
{b,a} = 16'b0; #10;
repeat (65535) begin 
    {b,a} = {b, a} + 1; #10;
end
$finish;
end
endmodule

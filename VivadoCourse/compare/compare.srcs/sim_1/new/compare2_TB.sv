`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 26.02.2024 15:24:59
// Design Name: 
// Module Name: compare2_TB
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


//module compare2_TB();
//logic [1:0] a,b;
//logic       greater, less;

//compare2 DUT(.a(a), .b(b), .greater(greater), .less(less));

//initial begin
//{b,a} = 4'b0000; #10;
//repeat (15) begin 
//    {b,a} = {b, a} + 1; #10;
//end
//$finish;
//end
//endmodule


module compare4_TB();
logic [3:0] a,b;
logic       GREATER, LESS;

compare4 DUT(.a(a), .b(b), .GREATER(GREATER), .LESS(LESS));

initial begin
{b,a} = 8'b0; #10;
repeat (255) begin 
    {b,a} = {b, a} + 1; #10;
end
$finish;
end
endmodule

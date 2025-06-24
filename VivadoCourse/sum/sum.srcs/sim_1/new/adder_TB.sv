`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 04.03.2024 14:56:33
// Design Name: 
// Module Name: adder_TB
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


module adder4_TB();
logic [3:0] a, b, s;
logic        cin, cout;

adder4 DUT(.a(a), .b(b), .s(s), .cin(cin), .cout(cout));

initial begin
    {cin, b, a} = 9'b0; #10;
    repeat (511) begin
        {cin, b, a} = {cin, b, a} + 1; #10;
    end
$finish;
end
endmodule

module adder16_TB();
logic [15:0] a, b, s;
logic  cin, cout;

adder16 DUT(.a(a), .b(b), .s(s), .cin(cin), .cout(cout));

initial begin
    cin = 0;
    {b, a} = 32'h0; #10;
    {b, a} = 32'hffff0000; #10;
    {b, a} = 32'h0000ffff; #10;
    {b, a} = 32'hffffffff; #10;
    cin = 1;
    {b, a} = 32'h0; #10;
    {b, a} = 32'hffff0000; #10;
    {b, a} = 32'h0000ffff; #10;
    {b, a} = 32'hffffffff; #10;
$finish;
end
endmodule

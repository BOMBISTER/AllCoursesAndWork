`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 12:50:06
// Design Name: 
// Module Name: sumator
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


module sumator(
    input a,
    input b,
    input cin,
    output s,
    output cout
    );
    assign s = !a & !b & cin |
    !a & b & !cin |
     a & !b & !cin | 
     a & b & cin;
    assign cout = (a&b)|(a&cin)|(b&cin);
endmodule

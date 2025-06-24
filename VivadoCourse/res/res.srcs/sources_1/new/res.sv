`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 13:54:56
// Design Name: 
// Module Name: res
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


module res(
    input a,
    input b,
    input cin,
    output m,
    output cout
    );
    assign m = !(!a&b&cin|!a & !b & !cin | a & b & !cin | a & !b & cin);
    assign cout = !a & b & !cin | !a & !b & cin | !a & b & cin | a & b & cin;
endmodule

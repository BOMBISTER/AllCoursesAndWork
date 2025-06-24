`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 26.02.2024 16:02:33
// Design Name: 
// Module Name: compare8
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


module compare2(
    input [1:0] a,
    input [1:0] b,
    output greater,
    output less
    );
    assign greater = a[1] & !b[1]
                    | !a[1] & !b[1] & a[0] & !b[0]
                    | a[1] & b[1] & a[0] & !b[0];
    assign less = !a[1] & b[1]
                    | a[1] & b[1] & !a[0] & b[0]
                    | !a[1] & !b[1] & !a[0] & b[0];                 
endmodule


module compare8(
    input [7:0] a,
    input [7:0] b,
    output GREATER,
    output LESS
    );
    logic [3:0] g, l;
    logic [1:0] gr, le;
   compare2 COMPARE_LOW1(.a(a[1:0]), .b(b[1:0]), .greater(g[0]), .less(l[0]));   
   compare2 COMPARE_HIGH1(.a(a[3:2]), .b(b[3:2]), .greater(g[1]), .less(l[1]));
   compare2 COMPARE_HIGH2(.a(a[5:4]), .b(b[5:4]), .greater(g[2]), .less(l[2]));  
   compare2 COMPARE_LOW2(.a(a[7:6]), .b(b[7:6]), .greater(g[3]), .less(l[3]));    
   compare2 COMPARE1(.a(g[1:0]), .b(l[1:0]), .greater(gr[0]), .less(le[0]));
   compare2 COMPARE2(.a(g[3:2]), .b(l[3:2]), .greater(gr[1]), .less(le[1]));     
   compare2 COMPARE(.a(gr[1:0]), .b(le[1:0]), .greater(GREATER), .less(LESS));     
endmodule

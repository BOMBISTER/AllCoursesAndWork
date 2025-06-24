`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 26.02.2024 15:21:17
// Design Name: 
// Module Name: compare2
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


module compare4(
    input [3:0] a,
    input [3:0] b,
    output GREATER,
    output LESS
    );
    logic [1:0] l, g;
    
       genvar i;
   generate
      for (i = 0; i < 2; i = i + 1)
      begin: label
         compare2 COMPARE(.a(a[2*i+1:2*i+0]), .b(b[2*i+1:2*i+0]), .greater(g[i]), .less(l[i])); 
      end
   endgenerate
    
//    compare2 COMPARE_LOW(.a(a[1:0]), .b(b[1:0]), .greater(g[0]), less(l[0]));   
//    compare2 COMPARE_HIGH(.a(a[3:2]), .b(b[3:2]), .greater(g[1]), less(l[1]));  
   compare2 COMPARE(.a(g[1:0]), .b(l), .greater(GREATER), .less(LESS));              
endmodule
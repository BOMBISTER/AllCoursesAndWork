`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 16:13:09
// Design Name: 
// Module Name: prcoder
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


module prcoder(
    input [3:0] in,
    output logic [1:0] code
    );
    
//always_comb
//    if (in[3] == 1'b1)
//        code = 2'b11;
//    else if (in[2])
//        code = 2'b10;
//    else if (in[1])
//        code = 2'b01;
//    else
//        code = 2'b00;

always_comb
    casex (in)
        4'b1xxx: code = 2'b11;
        4'b01xx: code = 2'b10;
        4'b001x: code = 2'b01;
        4'b000x: code = 2'b00;
    endcase
    
endmodule

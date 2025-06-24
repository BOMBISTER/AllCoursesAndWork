`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 16:37:45
// Design Name: 
// Module Name: coder8
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


module coder8(
    input [7:0] in,
    output logic [2:0] code
    );
//    assign code[0] = in[7] | in[5] | in[3] | in[1];
//    assign code[1] = in[7] |in[6]| in[3] | in[2];
//    assign code[2] = in[7] | in[6] | in[5] | in[4]; 
always_comb
    case (in)
        8'b00000001: code = 000;
        8'b00000010: code = 001;
        8'b00000100: code = 010;
        8'b00001000: code = 011;
        8'b00010000: code = 100;
        8'b00100000: code = 101;
        8'b01000000: code = 110;
        8'b10000000: code = 111;
    endcase;
endmodule

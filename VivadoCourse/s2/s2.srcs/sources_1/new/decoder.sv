`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 15:17:59
// Design Name: 
// Module Name: decoder
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


module decoder(
    input [1:0] code,
    output logic [3:0] q
    );
//    assign q[0] = !code[1] & !code[0];
//    assign q[1] = !code[1] & code[0];
//    assign q[2] = code[1] & !code[0];
//    assign q[3] = code[1] & code[0];

//always_comb
//    if (code == 2'b00)
//        q = 4'b0001;
//    else if (code == 2'b01)
//        q = 4'b0010;
//    else if (code == 2'b10)
//        q = 4'b0100;
//    else 
//        q = 4'b1000;

always_comb
    case(code)
        2'b00: q = 4'b0001;
        2'b01: q = 4'b0010;
        2'b10: q = 4'b0100;
        2'b11: q = 4'b1000;
    endcase;
endmodule

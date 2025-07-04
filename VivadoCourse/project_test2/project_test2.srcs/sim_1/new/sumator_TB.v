`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 12.02.2024 12:56:44
// Design Name: 
// Module Name: sumator_TB
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


module sumator_TB();
logic a, b, cin, s, cout;
sumator DUT(.a(a), .b(b), .cin(cin), .s(s), .cout(cout));

initial begin
    a = 0; b = 0; cin = 0; #10;
    a = 1; b = 0; cin = 0; #10;
    a = 0; b = 1; cin = 0; #10;
    a = 1; b = 1; cin = 0; #10;
    a = 0; b = 0; cin = 1; #10;
    a = 1; b = 0; cin = 1; #10;
    a = 0; b = 1; cin = 1; #10;
    a = 1; b = 1; cin = 1; #10;
$finish;
end
endmodule

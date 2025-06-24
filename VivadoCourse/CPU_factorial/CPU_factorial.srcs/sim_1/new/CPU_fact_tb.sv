`timescale 1ns / 1ps

module processor_tb_with_factorial_and_units();
    logic clk, reset, start;      // ������� ��������� ����������, ������ � �������
    logic done;                   // ������ ����������
    logic [7:0] result;           // ��������� ���������� ����������
    logic [7:0] initial_value;    // ��������� �������� ��� ����������
    logic [7:0] factorial_result;
    logic [7:0] memory_data [0:0]; // ������ ��� ������ ������ �� memory.mem

    
    processor_with_factorial_and_units DUT(.clk(clk), .reset(reset), .start(start), .initial_value(initial_value), .done(done), .result(result), .factorial_result(factorial_result));

    // ��������� ��������� �������
    initial begin
        clk = 0;
        forever #5 clk = ~clk;    // �������� ������ � �������� 10 ������ �������
    end

    
    initial begin
        // ������ ������ ������ �� memory.mem � ������
        $readmemh("memory.mem", memory_data);
        initial_value = memory_data[0]; // ���������� �������� initial_value �� �������

        // ������������� ������� ��������
        reset = 1; start = 0;
        
        
        #20;
        
        // ������ ������
        reset = 0;
        
        // ������ ���������� ����������
        start = 1;
        
        // �������� ���������� ����������
        @(posedge done);
        
        // ����� ����������
        $display("Factorial Result: %d", result);
        
        // ���������� ���������
        $finish;
    end
endmodule


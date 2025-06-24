`timescale 1ns / 1ps
module test_bench;

    // �����
    reg clk;
    reg reset;

    // ������
    wire [7:0] data_out;
    wire [31:0] sum_output;

    // ��������������� ������ CustomCPU
    CustomCPU cpu(
        .clk(clk),
        .reset(reset)
    );

    // ����������� sum_output ��� ���������� ���������� ������������
    assign sum_output = cpu.calculated_sum;

    // ��������� ��������� �������
    initial begin
        clk = 0;
        forever #10 clk = ~clk; // ������������ ������ 10 ����������
    end

    // �������� ��������
    initial begin
        // �������������
        reset = 1;
        #25;                    // ��������, ����� ����� ��� ���������������
        reset = 0;

        // ������ ������ � �������� ������ ������������
        $monitor("At time %t, sum_output = %d", $time, sum_output);
        
        // ���� ��������� ������
        #100;
        
        // �������� ����������
        if (sum_output != 15) begin  // ������� ����� ����� �� 1 �� 5 (1+2+3+4+5=15)
            $display("Test failed: Sum output is incorrect");
        end else begin
            $display("Test passed: Sum output is correct");
        end

        #10;
        $finish;  // ���������� ���������
    end

endmodule

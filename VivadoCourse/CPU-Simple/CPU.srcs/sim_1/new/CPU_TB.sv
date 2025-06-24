module tb_CPU;
    logic clk;
    logic reset;
    logic [31:0] result;
    logic done;

    CPU uut (
.clk(clk),
.reset(reset),
.result(result),
.done(done)
);

// ��������� ��������� �������
    always #5 clk = ~clk;

    initial begin
// ������������� ��������
    clk = 0;
    reset = 1;

// ����� �������
    #10;
    reset = 0;

// �������� ���������� ���������� ������
    wait(done);
    $display("Result: %0d", result);

// ���������� ��������� ����� 1000 ������
    #1000;
    $finish;
end
endmodule



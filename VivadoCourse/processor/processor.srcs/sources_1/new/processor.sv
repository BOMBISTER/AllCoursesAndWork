module processor(
    input logic clk,   // ������� �������� ������
    input logic reset, // ������� ������ ������
    input logic [7:0] acc, // ������� ������� ������������
    input logic start, // ������� ������ ������� ��������
    output logic done, // �������� ������ ���������� ��������
    output logic [7:0] result // �������� ��������� ��������
);

// ���������� ���������� ���������
logic [2:0] current_state;
// ������� ��� �������� �������������� ���������� ����������
logic [7:0] factorial_result;
// ������� ��� �������� �������� ����������
logic [7:0] factorial_counter;

// ������ ���������� ��������� ����������
always_ff @(posedge clk or posedge reset) begin
    if (reset) begin
        current_state <= 3'b000; // ��������� ������
        factorial_result <= 8'h00; // ����� �������� ���������� ����������
        factorial_counter <= 8'h00; // ����� �������� �������� ����������
        done <= 0; // ����� ������� ���������� ��������
    end
    else begin
        // ������ ���������� ��������� �� ������ �������� ���������
        case (current_state)
            3'b000: begin // ��������� �������� �������
                if (start) begin
                    current_state <= 3'b001; // ������� � ���������� ��������
                    factorial_result <= acc; // ������������� ���������� ���������� ��������� ������������
                    factorial_counter <= acc - 1; // ������������� �������� ����������
                end
            end
            3'b001: begin // ��������� ���������� ��������
                $display("Factorial Counter: %d, Factorial Result: %d", factorial_counter, factorial_result);
                if (factorial_counter == 0) begin
                    current_state <= 3'b010; // ������� � ��������� ����������
                end
                else begin
                    factorial_result <= factorial_result * factorial_counter; // ��������� ���������� �� �������
                    factorial_counter <= factorial_counter - 1; // ���������� ��������
                end
            end
            3'b010: begin // ��������� ���������� ��������
                // ������������� ������ ���������� � ������� ���������
                done <= 1;
                result <= factorial_result;
                current_state <= 3'b000; // ��������� � ��������� �������� ������� ��������� ��������
            end
        endcase
    end
end

endmodule




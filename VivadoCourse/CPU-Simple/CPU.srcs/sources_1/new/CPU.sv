module CPU (
    input logic clk,
    input logic reset,
    output logic [31:0] result, // ��������� ���������� ������
    output logic done // ���� ���������� ����������
    );
// ����������� ��������� ����������
typedef enum logic [2:0] {
    FETCH,
    DECODE,
    EXECUTE,
    FACTORIAL,
    DONE
} state_t;

// �������� � �������
state_t state, next_state;
    logic [31:0] instruction;
    logic [31:0] accumulator;
    logic [7:0] instruction_pointer;
    logic [31:0] memory [0:255];
    logic [7:0] operand;

// ������������� ������ �� �����
initial begin
    $readmemh("memory.mem", memory);
end

// ����������� ����
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            state <= FETCH;
            accumulator <= 32'b0;
            instruction_pointer <= 8'b0;
            done <= 1'b1;
        end else begin
        state <= next_state;
        if (state == EXECUTE) begin
            case (instruction[31:24])
                8'h01: accumulator <= accumulator + operand; // ������ ������� ADD
                8'h02: state <= FACTORIAL; // ������� FACTORIAL
// ������ ������� ����� �������� �����
            endcase
        end else if (state == FACTORIAL) begin
            accumulator <= 1;
            for (int i = 1; i <= operand; i++) begin
                accumulator <= accumulator * i;
            end
            state <= DONE;
            end else if (done) begin // ����������� ���� ������ �������� ��������� ����� ����������
                state <= DONE;
            end
        end
    end

// ������ �������� ����� �����������
    always_comb begin
        case (state)
            FETCH: next_state = DECODE;
            DECODE: next_state = EXECUTE;
            EXECUTE: next_state = FETCH;
            DONE: next_state = DONE; // ����� ��������� DONE
            default: next_state = FETCH;
        endcase
    end

// ���������� ���������� ������ � �������������
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            instruction_pointer <= 8'b0;
        end else if (state == FETCH) begin
            instruction <= memory[instruction_pointer];
            instruction_pointer <= instruction_pointer + 1;
        end else if (state == DECODE) begin
            operand <= instruction[7:0];
        end
    end

// �������� �������
    assign result = accumulator;
endmodule

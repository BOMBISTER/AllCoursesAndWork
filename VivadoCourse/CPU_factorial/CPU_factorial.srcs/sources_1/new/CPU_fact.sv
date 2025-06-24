`timescale 1ns / 1ps

// ������ ���������� � ����������� ���������� � ������� �������
module processor_with_factorial_and_units(
    input logic reset,             // ������ ������
    input logic clk,               // �������� ������
    input logic start,             // ������ ������� ����������
    input logic [7:0] initial_value, // ��������� �������� ��� ����������
    output logic done,             // ������ ���������� ����������
    output logic [7:0] factorial_result,
    output logic [7:0] result      // ��������� ���������� ����������
);
    //-------------MicroControl-----------------
    logic [2:0] current_state;     // ������� ��������� ����������������
    logic [7:0] factorial_counter; // ������� ��� ���������� ����������

    // ������������� ���� �������� ��� ������
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            // ��������� ��������� �������� ��� ������
            current_state <= 3'b000;       // ��������� ������
            factorial_result <= 8'h01;     // ��������� ���������� (1)
            factorial_counter <= initial_value; // ������� ����������
            done <= 0;                     // ������ ����������
            result <= 8'h00;               // ���������
        end
        else begin
            // ������ �������� ����� �����������
            case (current_state)
                3'b000: begin // �������� ������� ������
                    if (start) begin
                        factorial_counter <= initial_value;
                        factorial_result <= 8'h01; // ����� ���������� ��� ������ ����������
                        current_state <= 3'b001;   // ������� � ���������� ����������
                    end
                end
                3'b001: begin // ��������� ���������� ����������
                    if (factorial_counter <= 8'd1) begin
                        current_state <= 3'b010;   // ������� � ����������
                    end
                    else begin
                        factorial_result <= factorial_result * factorial_counter;
                        factorial_counter <= factorial_counter - 1;
                    end
                end
                3'b010: begin // ��������� ����������
                    done <= 1;                     // ��������� ������� ����������
                    result <= factorial_result;    // ����� ����������
                    current_state <= 3'b000;       // ������� � ��������� ��������
                end
            endcase
        end
    end

    //---Accumulator and Arithmetic Logic Unit-------------------
    logic [7:0] acc;               // ������� ������������
    logic [7:0] d_out;             // �������� ������ �� ������

    // Instruction Pointer
    logic [4:0] ip;                // Instruction Pointer

    // Instruction Register
    logic [2:0] inst_COP;          
    logic [4:0] inst_ADDR;         // Instruction Register

    //-----Memory
    logic [7:0] ram [31:0];        

    //-------Instruction Pointer------
    always_ff @(posedge clk or posedge reset) begin
        if (reset)
            ip <= 5'b00000;        // Instruction Pointer �������
        else  
            ip <= ip + 1;          // ���������� InPo
    end

    //----Accumulator-------
    always_ff @(posedge clk or posedge reset) begin
        if (reset)
            acc <= 8'b00000000;    // ����� 
    end

    //-----Register Pointer---------
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            inst_ADDR <= 5'b00000; // ����� 
            inst_COP  <= 3'b000;  
        end
        else if (current_state == 3'b010) begin
            inst_ADDR <= d_out[4:0]; // �������� ������ 
            inst_COP  <= d_out[7:5]; // �������� ���� �������� 
        end
    end

    //----Memory------
    always_ff @(posedge clk) begin
        if (current_state == 3'b100)
            ram[inst_ADDR] <= acc; // ������ � ������
        d_out <= ram[(current_state == 3'b000) ? ip : inst_ADDR]; // ������ �� ������
    end

endmodule


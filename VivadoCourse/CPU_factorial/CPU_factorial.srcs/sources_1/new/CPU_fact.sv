`timescale 1ns / 1ps

// Модуль процессора с вычислением факториала и другими блоками
module processor_with_factorial_and_units(
    input logic reset,             // Сигнал сброса
    input logic clk,               // Тактовый сигнал
    input logic start,             // Сигнал запуска вычисления
    input logic [7:0] initial_value, // Начальное значение для факториала
    output logic done,             // Сигнал завершения вычисления
    output logic [7:0] factorial_result,
    output logic [7:0] result      // Результат вычисления факториала
);
    //-------------MicroControl-----------------
    logic [2:0] current_state;     // Текущее состояние микроконтроллера
    logic [7:0] factorial_counter; // Счетчик для вычисления факториала

    // Инициализация всех сигналов при сбросе
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            // Установка начальных значений при сбросе
            current_state <= 3'b000;       // Состояние сброса
            factorial_result <= 8'h01;     // Результат факториала (1)
            factorial_counter <= initial_value; // Счетчик факториала
            done <= 0;                     // Сигнал завершения
            result <= 8'h00;               // Результат
        end
        else begin
            // Логика перехода между состояниями
            case (current_state)
                3'b000: begin // Ожидание сигнала старта
                    if (start) begin
                        factorial_counter <= initial_value;
                        factorial_result <= 8'h01; // Сброс результата для нового вычисления
                        current_state <= 3'b001;   // Переход к вычислению факториала
                    end
                end
                3'b001: begin // Состояние вычисления факториала
                    if (factorial_counter <= 8'd1) begin
                        current_state <= 3'b010;   // Переход к завершению
                    end
                    else begin
                        factorial_result <= factorial_result * factorial_counter;
                        factorial_counter <= factorial_counter - 1;
                    end
                end
                3'b010: begin // Состояние завершения
                    done <= 1;                     // Установка сигнала завершения
                    result <= factorial_result;    // Вывод результата
                    current_state <= 3'b000;       // Возврат к состоянию ожидания
                end
            endcase
        end
    end

    //---Accumulator and Arithmetic Logic Unit-------------------
    logic [7:0] acc;               // Регистр аккумулятора
    logic [7:0] d_out;             // Выходные данные из памяти

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
            ip <= 5'b00000;        // Instruction Pointer сброшен
        else  
            ip <= ip + 1;          // Увеличение InPo
    end

    //----Accumulator-------
    always_ff @(posedge clk or posedge reset) begin
        if (reset)
            acc <= 8'b00000000;    // Сброс 
    end

    //-----Register Pointer---------
    always_ff @(posedge clk or posedge reset) begin
        if (reset) begin
            inst_ADDR <= 5'b00000; // Сброс 
            inst_COP  <= 3'b000;  
        end
        else if (current_state == 3'b010) begin
            inst_ADDR <= d_out[4:0]; // Загрузка адреса 
            inst_COP  <= d_out[7:5]; // Загрузка кода операции 
        end
    end

    //----Memory------
    always_ff @(posedge clk) begin
        if (current_state == 3'b100)
            ram[inst_ADDR] <= acc; // Запись в память
        d_out <= ram[(current_state == 3'b000) ? ip : inst_ADDR]; // Чтение из памяти
    end

endmodule


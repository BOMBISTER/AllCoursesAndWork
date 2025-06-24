module processor(
    input logic clk,   // входной тактовый сигнал
    input logic reset, // входной сигнал сброса
    input logic [7:0] acc, // входной регистр аккумулятора
    input logic start, // входной сигнал запуска операции
    output logic done, // выходной сигнал завершения операции
    output logic [7:0] result // выходной результат операции
);

// Объявление переменной состояния
logic [2:0] current_state;
// Регистр для хранения промежуточного результата факториала
logic [7:0] factorial_result;
// Регистр для хранения счетчика факториала
logic [7:0] factorial_counter;

// Логика обновления состояний процессора
always_ff @(posedge clk or posedge reset) begin
    if (reset) begin
        current_state <= 3'b000; // Состояние сброса
        factorial_result <= 8'h00; // Сброс регистра результата факториала
        factorial_counter <= 8'h00; // Сброс регистра счетчика факториала
        done <= 0; // Сброс сигнала завершения операции
    end
    else begin
        // Логика обновления состояний на основе текущего состояния
        case (current_state)
            3'b000: begin // Состояние ожидания запуска
                if (start) begin
                    current_state <= 3'b001; // Переход к выполнению операции
                    factorial_result <= acc; // Инициализация результата факториала значением аккумулятора
                    factorial_counter <= acc - 1; // Инициализация счетчика факториала
                end
            end
            3'b001: begin // Состояние выполнения операции
                $display("Factorial Counter: %d, Factorial Result: %d", factorial_counter, factorial_result);
                if (factorial_counter == 0) begin
                    current_state <= 3'b010; // Переход в состояние завершения
                end
                else begin
                    factorial_result <= factorial_result * factorial_counter; // Умножение результата на счетчик
                    factorial_counter <= factorial_counter - 1; // Уменьшение счетчика
                end
            end
            3'b010: begin // Состояние завершения операции
                // Устанавливаем сигнал завершения и выводим результат
                done <= 1;
                result <= factorial_result;
                current_state <= 3'b000; // Переходим в состояние ожидания запуска следующей операции
            end
        endcase
    end
end

endmodule




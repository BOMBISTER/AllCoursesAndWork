`timescale 1ns / 1ps

module processor_tb_with_factorial_and_units();
    logic clk, reset, start;      // Сигналы тактового генератора, сброса и запуска
    logic done;                   // Сигнал завершения
    logic [7:0] result;           // Результат вычисления факториала
    logic [7:0] initial_value;    // Начальное значение для факториала
    logic [7:0] factorial_result;
    logic [7:0] memory_data [0:0]; // Массив для чтения данных из memory.mem

    
    processor_with_factorial_and_units DUT(.clk(clk), .reset(reset), .start(start), .initial_value(initial_value), .done(done), .result(result), .factorial_result(factorial_result));

    // Генерация тактового сигнала
    initial begin
        clk = 0;
        forever #5 clk = ~clk;    // Тактовый сигнал с периодом 10 единиц времени
    end

    
    initial begin
        // Чтение первой строки из memory.mem в массив
        $readmemh("memory.mem", memory_data);
        initial_value = memory_data[0]; // Присвоение значения initial_value из массива

        // Инициализация входных сигналов
        reset = 1; start = 0;
        
        
        #20;
        
        // Снятие сброса
        reset = 0;
        
        // Запуск вычисления факториала
        start = 1;
        
        // Ожидание завершения вычисления
        @(posedge done);
        
        // Вывод результата
        $display("Factorial Result: %d", result);
        
        // Завершение симуляции
        $finish;
    end
endmodule


`timescale 1ns / 1ps
module test_bench;

    // Входы
    reg clk;
    reg reset;

    // Выходы
    wire [7:0] data_out;
    wire [31:0] sum_output;

    // Инстанцирование модуля CustomCPU
    CustomCPU cpu(
        .clk(clk),
        .reset(reset)
    );

    // Подключение sum_output для наблюдения результата суммирования
    assign sum_output = cpu.calculated_sum;

    // Генерация тактового сигнала
    initial begin
        clk = 0;
        forever #10 clk = ~clk; // Переключение каждые 10 наносекунд
    end

    // Тестовый сценарий
    initial begin
        // Инициализация
        reset = 1;
        #25;                    // Ожидание, чтобы сброс был зарегистрирован
        reset = 0;

        // Чтение данных и проверка работы суммирования
        $monitor("At time %t, sum_output = %d", $time, sum_output);
        
        // Ждем несколько тактов
        #100;
        
        // Проверка результата
        if (sum_output != 15) begin  // Ожидаем сумму чисел от 1 до 5 (1+2+3+4+5=15)
            $display("Test failed: Sum output is incorrect");
        end else begin
            $display("Test passed: Sum output is correct");
        end

        #10;
        $finish;  // Завершение симуляции
    end

endmodule

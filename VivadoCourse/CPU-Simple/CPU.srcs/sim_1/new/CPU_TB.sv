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

// Генератор тактового сигнала
    always #5 clk = ~clk;

    initial begin
// Инициализация сигналов
    clk = 0;
    reset = 1;

// Сброс системы
    #10;
    reset = 0;

// Ожидание завершения выполнения команд
    wait(done);
    $display("Result: %0d", result);

// Завершение симуляции через 1000 тактов
    #1000;
    $finish;
end
endmodule



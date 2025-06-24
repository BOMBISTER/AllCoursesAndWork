module sum_module(
    input logic [7:0] input_num,
    output logic [31:0] output_sum
);

always_comb begin
    output_sum = 0;
    for (int i = 1; i <= input_num; i++) begin
        output_sum = output_sum + i;
    end
end
    
endmodule

module hisCPU(
    input clk,
    input reset
);

logic [4:0] ip; // счетчик инструкций
logic [7:0] acc, d_out;
logic [2:0] inst_COP; // код операции
logic [4:0] inst_ADDR; // адрес инструкции

logic [31:0] sum_result;
sum_module sum_inst(.input_num(acc), .output_sum(sum_result));

enum logic [3:0] {inst_addr = 4'b0000,  // указатель адреса инструкции
                  inst_read = 4'b0001,  // чтение инструкции
                  decode = 4'b0010,     // декодирование инструкции
                  load_read = 4'b0011,  // чтение для операции загрузки
                  load = 4'b0100,       // операция загрузки
                  store = 4'b0101,      // операция сохранения
                  add_read = 4'b0110,   // чтение для операции сложения
                  add = 4'b0111,        // операция сложения
                  decrement = 4'b1000,  // уменьшение
                  branch = 4'b1001,     // ветвление
                  halt = 4'b1010,       // остановка
                  sum_start = 4'b1011,  // начало вычисления суммы
                  sum_done = 4'b1100} state; // завершение вычисления суммы

parameter ld = 3'b000;
parameter st = 3'b001;
parameter ad = 3'b010;
parameter dc = 3'b011;
parameter br = 3'b100;
parameter ht = 3'b101;
parameter su = 3'b110;

// MCU (Micro Control Unit)
always @(posedge clk or posedge reset)
    if (reset) begin
        state <= inst_addr;
    end
    else 
        case (state)
            inst_addr: state <= inst_read;
            inst_read: state <= decode;
            decode: case (inst_COP)
                         ld: state <= load_read;
                         st: state <= store;
                         ad: state <= add_read;
                         dc: state <= decrement;
                         br: state <= branch;
                         ht: state <= halt;
                         su: state <= sum_start;
                     endcase
            load_read: state <= load;
            add_read: state <= add;
            load: state <= inst_addr;
            store: state <= inst_addr;
            add: state <= inst_addr;
            decrement: state <= inst_addr;
            branch: state <= inst_addr;
            halt: state <= halt;
            sum_start: state <= sum_done;
            sum_done: state <= inst_addr;
        endcase

// IP (Instruction Pointer)
always @(posedge clk or posedge reset)
    if (reset)
        ip <= 5'b00000;
    else if ((state == load) | (state == store) | (state == add) | (state == decrement) | (state == sum_done))
        ip <= ip + 1;
    else if (state == branch)
        if (acc != 0)
            ip <= inst_ADDR;
        else 
            ip <= ip + 1;

// Accumulator and Arithmetic Logic Unit
always @(posedge clk or posedge reset) begin
    if (reset)
        acc <= 8'b00000000;
    else if (state == load)
        acc <= d_out;
    else if (state == add)
        acc <= acc + d_out;
    else if (state == decrement)
        acc <= acc - 1;
    else if (state == sum_done)
        acc <= sum_result[7:0]; // Сохраняем младшие 8 бит результата
end

// Instruction Register
always @(posedge clk or posedge reset) begin
    if (reset) begin
        inst_ADDR <= 5'b00000;
        inst_COP <= 3'b000;
    end
    else if (state == inst_read) begin
        inst_ADDR <= d_out[4:0];
        inst_COP <= d_out[7:5];
    end
end

// Memory
logic [7:0] ram [31:0];

initial $readmemb("memory.mem", ram, 0, 31);

always @(posedge clk)
    if (state == store)
        ram[inst_ADDR] <= acc;

always @(posedge clk)
    d_out <= ram[(state == inst_addr) ? ip : inst_ADDR];

endmodule

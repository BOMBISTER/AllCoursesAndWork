module processor_tb;

    // Inputs
    reg clk;
    reg reset;
    reg start;
    reg [7:0] acc;
    
    // Outputs
    wire done;
    wire [7:0] result;
    
    // Instantiate the processor module
    processor dut (
        .clk(clk),
        .reset(reset),
        .acc(acc),
        .start(start),
        .done(done),
        .result(result)
    );
    
    // Clock generation
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    // Testbench logic
    initial begin
        // Initialize inputs
        reset = 1;
        start = 0;
        acc = 5; // Set the initial value of acc
        
        // Wait for 10 clock cycles
        repeat (10) @(posedge clk);
        
        // Release reset
        reset = 0;
        
        // Start the operation
        start = 1;
        
        // Wait for the operation to complete
        @(posedge done);
        
        // Add a small delay to ensure result is updated
        #10;
        
        // Check the result
        $display("Factorial Result: %d", result);
        
        // Finish simulation
        $finish;
    end

endmodule


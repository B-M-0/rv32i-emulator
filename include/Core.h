class Core {
    private:
        static constexpr uint32_t MEM_BASE = 0x80000000;
        static constexpr uint32_t mem_size = 1u << 24;

        uint32_t x[32];
        uint32_t pc = MEM_BASE;

        bool halted = false;        
        static std::vector<uint8_t> mem(mem_size);
        

        
    public:
        void execute(Instruction instr);
    
        void     write_R(uint32_t address, uint32_t value) ;
        uint32_t read_R(uint32_t address) const;
        void     write_M(uint32_t address, uint32_t value) ;
        uint32_t read_M(uint32_t address) const;

        void step();
};
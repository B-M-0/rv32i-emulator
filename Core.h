class Core {
    private:
        uint32_t x[32];
        uint32_t pc;
        static mem_size = 1u << 24;
        static std::vector<uint8_t> mem(mem_size);
        static constexpr MEM_BASE = 0x80000000;

        
    public:
        void write_R(uint32_t address, uint32_t value) const;
        uint32_t read_R(uint32_t address);

        void write_M(uint32_t address, uint32_t value) const;
        uint32_t read_M(uint32_t address);

        void step();
};

  
  uint8_t m_flags;
  enum Flags {
    FLAG_IS_ACTIVE = 1 << 0,
    FLAG_RETRIGGER = 1 << 1
  };
  bool check(uint8_t flag);
  void set(uint8_t flag);
  void clear(uint8_t flag);
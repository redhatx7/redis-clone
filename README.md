# Simple Redis Clone in C

This is a personal Redis clone project written in C forked from @Glyphack. **Note:** This is not the standard Redis implementation from Codecrafters, and it is a custom implementation. The project is still in progress, and some features are incomplete.

---

## Implementation Details

- **TCP Server**:  
  Uses `epoll` for non-blocking I/O and efficient event handling. Might use `io_uring` in future for potential performance improvements.

- **Hash Table**:  
  - The hash table uses an array named `bucket` and another named `nodes`.  
  - Hashing determines the index in the `bucket` array, which points to the first node in the `nodes` array (the start of the chain).  
  - On collision, new entries are added to the beginning of the chain with a `next` pointer to link nodes.  
  - Hash table capacity is the nearest upper-bound prime number to reduce clustering and improve performance. **Why prime numbers?** They minimize hash collisions by ensuring better key distribution.

---

## TODO List

1. **Hashmap**:  
   - Implement key deletion functionality. Will use a dummy pointer in case of deletion.  
   - Improve handling of collisions and edge cases.  

2. **Memory Safety**:  
   - Current implementation is not memory-safe. Potential issues need to be addressed.  

3. **Hash Function**:  
   - Consider upgrading to a 64-bit version of MurmurHash2. The current version is 32-bit.  

4. **Command Parser**:  
   - It's not standard parser yet and only parse standard encoded redis commands like `*2\r\n$3\r\nGET\r\n$4\r\nKey1\r\n`
   - Improve memory management for strings and general parsing efficiency.  

5. **Custom Memory Allocator**:  
   - Evaluate the need for a custom memory allocator to optimize performance.  

---

## Progress Banner

[![progress-banner](https://backend.codecrafters.io/progress/redis/94d9788d-020a-4f54-b813-2fcd07efd8ab)](https://app.codecrafters.io/users/codecrafters-bot?r=2qF)

My solution to ["Build Your Own Redis" Challenge](https://codecrafters.io/challenges/redis).

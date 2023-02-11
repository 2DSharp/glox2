# Things to do

## VM Level
- [x] Basic vm instructions and bytecode setup
- [x] Basic CPU calculation in bytecodes
- [x] Native function call implementation
- [x] Initiate IO (printing to console)
- [x] Implement heap
- [x] Implement arrays
- [x] Return data from native calls
- [x] Constant pool implementation
- [ ] Store multiple data types - floats, boolean (ldc, fload)
- [ ] Basic arithmetic ops on different data types
- [x] String implementation
- [ ] IO - Read from console
- [ ] IO - Read and write from files
- [ ] IO - Networking - Read and Write Sockets
- [x] Allow calling glox program calls from native code (https://stackoverflow.com/a/17083153)
  - Use single function to allow access to internal symbol table (constant pool/function pool)
- [x] Objects (Classes and methods)
- [x] Pass objects to native calls
- [ ] Allow for making libraries

## Compiler level
- [ ] Store bytecode to binary file
- [ ] Convert glox code to bytecode
- [ ] Build symbol table
- [ ] Make GNative code generator

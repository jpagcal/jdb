# JDB
JDB is a distributed key-value store built in C++17

## Planning
JDB is built incrementally. The current stage of development is a **single node, in-memory KV-store with WAL persistence**. Below are the deliverables for the current stage:

** Memory allocation**
- [x] Arena *untested*
- [ ] Concurrency support for allocation

**Database internal**
- [x] Memtable definition
- [ ] Memtable interface

**Persistence**
- [ ] WAL definition
- [ ] WAL instruction parser
- [ ] WAL instruction executor/replay

**User-level interface**
- [ ] REPL
- [ ] exact-match querying interface

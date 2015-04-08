# Neon Event Driven System (EDS) Examples

Neon is a collection of software components for real-time applications.

Neon EDS components are:
* **port** - Portable system that offers consistent API to control the
    underlaying microcontroller and OS (if used).
* **base** - Bare minimum headers needed by other Neon components.
* **mm** - Memory management
* **sched** - Contains priority array and the cooperative scheduler
* **timer** - Virtual general purpose timers. 
* **ep** - Event processing provides event generation and handling, event 
    queues, broadcasting services and execution of finite state machines.

# Using Event Driven System examples

## Configuration

Configuration is done in `neon_eds_app_config.h` header file located in example
port directory. The file is included by `base/config.h` header file, which is 
included by all Neon components.

## Building

### Include paths

- `neon-eds/include` - include path. Submodules are referenced by their 
    respective subdirectories.
- `neon-eds/port/_port-name_` - Neon core port. The _port-name_ is the directory 
    of the port that is being used.
- `port/_port-name_` - Example port (usually BSP sources). The _port-name_ is 
    the directory of the port that is being used.

### Source files

Neon EDS sources:
- `neon-eds/source/epa.c` - Event Processing Agent
- `neon-eds/source/equeue.c` - Event Queue
- `neon-eds/source/etimer.c` - Event Timer
- `neon-eds/source/event.c` - Event Object
- `neon-eds/source/heap.c` - Heap memory allocator
- `neon-eds/source/mem.c` - Memory allocator class
- `neon-eds/source/pool.c` - Pool memory allocator
- `neon-eds/source/sched.c` - Scheduler
- `neon-eds/source/smp.c` - State machine processor
- `neon-eds/source/static.c` - Static memory allocator
- `neon-eds/source/timer.c` - Virtual timer
- `neon-eds/port/_port-name_/p_core.c` - main port source file. Ports may have 
    additional source files located in `port/_port-name_` directory which need 
    to be compiled.
    
Examples additional sources:
- `example/_example-name_/_example-name_.c` - main example source file
- `port/_port-name_/*` - everything under this directory is related to used port
    and should be compiled as needed.



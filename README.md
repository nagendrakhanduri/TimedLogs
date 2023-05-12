TimedLogs
============
The logs which are printed when timer expires for C++ and C

./compile.sh creates `c_timer_logs` and `cpp_timer_logs`

Code is seperated by `#ifdef CPP11` for the C++11 code and `#ifndef CPP11` for the C code.

The string in the `print_timed_logs` is the identifier for the Log instance. 
Different string means difference instance of logs.

# print_timed_logs
Checks the string id, if not exist then create a new.
If id exist then returns if timer for the string is expired or not.

# Usage.

Print after 5 seconds.

```
// First time an id created with int_main1
if (a.print_timed_logs ("int_main1", static_cast<std::chrono::milliseconds>(5000))) { 
  LOG_DEBUG ("5:Hello %s\n", "World"); 
}
// Second time id already exist and checks if there is 5 sec elased from the first instance.
// Here, a.print_timed_logs returns falls
if (a.print_timed_logs ("int_main1", static_cast<std::chrono::milliseconds>(5000))) { 
  LOG_DEBUG ("5:Hello %s\n", "World"); 
}
```

If above snippet is put in  awhile loop.

```
while (1) {
  // First time an id created with int_main1 and all other loop instance will check
  // if the time is passed
  if (a.print_timed_logs ("int_main1", static_cast<std::chrono::milliseconds>(5000))) { 
    LOG_DEBUG ("5:Hello %s\n", "World"); 
  }
}
```

Any time can be checked with the same instance.
```
while (1) {
  // First time an id created with int_main1 and all other loop instance will check
  // if the time is passed
  if (a.print_timed_logs ("int_main1", static_cast<std::chrono::milliseconds>(5000))) { 
    LOG_DEBUG ("5:Hello %s\n", "World"); 
  }
  // Prints after each 1 second.
  if (a.print_timed_logs ("int_main1", static_cast<std::chrono::milliseconds>(1000))) { 
    LOG_DEBUG ("5:Hello %s\n", "World"); 
  }
}
```

# Global Instances
If global instances are used then it could be cleaned up using `del_timed_logs` and `reset_timed_logs`.

# Local Instances - Quick debugging in threads
Create a local instance and use `print_timed_logs`. Different instance id means different timing control.
```
while (1) {
  if (_condition_1) {
    if (a.print_timed_logs ("int_main1", static_cast<std::chrono::milliseconds>(5000))) { 
      LOG_DEBUG ("5:Hello %s\n", "World"); 
  }  else if (_condition_2) {
      if (a.print_timed_logs ("int_main2", static_cast<std::chrono::milliseconds>(1000))) { 
      LOG_DEBUG ("1:Hello %s\n", "World"); 
  }  else {
    if (a.print_timed_logs ("int_main_else", static_cast<std::chrono::milliseconds>(10000))) { 
      LOG_DEBUG ("10:Hello %s\n", "World"); 
  }
}
  ```




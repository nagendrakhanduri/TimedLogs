#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>

#ifndef CPP11
//#define LOG_DEBUG(str, args...) 
#define LOG_DEBUG(str, args...) printf (str, ##args)
typedef struct timedlogs {
  struct timedlogsarr {
    char id[1024];
    struct timespec ret;
  }arr[255];
  int32_t max;
  int32_t index;

}timedlogs;

timedlogs g_timespec_arr={.max=255};

int32_t print_time_find(timedlogs *o, char *id) {
  for (int32_t i=0; i<o->index; i++) {
    if (strcmp(o->arr[i].id, id) == 0) {
      return i;
    }
  }
  return -1;
}



#define mf_mstons(x) ( ((x%1000000))*1000000 )
#ifndef DLOG_PRINT_TIMED_LOGS
#define DLOG_PRINT_TIMED_LOGS(str, args...) 
#else
#undef DLOG_PRINT_TIMED_LOGS
#define DLOG_PRINT_TIMED_LOGS(str, args...) printf ("PrintTimedLogs: "str, ##args)
#endif
#define bool _Bool

bool is_time_spec_add_ns_overflow (struct timespec *s1, int32_t interval_in_ms) {
  struct timespec max_time= {~0,~0};
  max_time.tv_nsec = max_time.tv_nsec << ((sizeof(max_time.tv_nsec)*8)-1);
  if (s1->tv_nsec/2 + (mf_mstons(interval_in_ms)/2) & max_time.tv_nsec) {
    return 1;
  }
  return 0;
}
bool is_time_spec_add_overflow (struct timespec *s1, struct timespec *s2) {
  if (s2->tv_nsec < s2->tv_nsec) {
    return 1;
  }
  return 0;
}


struct timespec time_spec_add_ms (struct timespec *s1, int32_t interval_in_ms ) {
  struct timespec ret_time= {0,0};
  ret_time.tv_sec = s1->tv_sec + interval_in_ms/1000;
  ret_time.tv_nsec = s1->tv_nsec + mf_mstons(interval_in_ms);
  if(is_time_spec_add_ns_overflow(s1, interval_in_ms)) { 
  //if(is_time_spec_add_overflow(s1, &ret_time)) { 
    ret_time.tv_sec++;
  }
  return ret_time;
}

int print_timer_expired (timedlogs *o, char *id, int32_t interval) {
int32_t index=print_time_find(o,id);
    struct timespec now_time = {0,0};
    struct timespec next_time = o->arr[index].ret;
    clock_gettime(CLOCK_REALTIME, &now_time);
    if (now_time.tv_sec > next_time.tv_sec || 
        (now_time.tv_sec==next_time.tv_sec && 
         now_time.tv_nsec >= next_time.tv_nsec)  ) {
      DLOG_PRINT_TIMED_LOGS("refresh  for %ld %ld nowtime %ld %ld input ns time %ld\n",
          o->arr[index].ret.tv_sec, o->arr[index].ret.tv_nsec, now_time.tv_sec, 
          now_time.tv_nsec, mf_mstons(interval));
      o->arr[index].ret=time_spec_add_ms(&now_time, interval);
      return 1;
    }
    return 0;
}

int print_timed_logs (timedlogs *o, char *id, int32_t interval) {
  struct timespec now_time= {0,0};
  struct timespec max_time= {~0,~0};
  max_time.tv_nsec = max_time.tv_nsec << ((sizeof(max_time.tv_nsec)*8)-1);
  if (print_time_find(o, id) == -1) {
    clock_gettime(CLOCK_REALTIME, &now_time);
    o->arr[o->index].ret=time_spec_add_ms (&now_time, interval);
    DLOG_PRINT_TIMED_LOGS ("Added for %ld %ld nowtime %ld %ld input ns time = %ld\n",
        o->arr[o->index].ret.tv_sec, o->arr[o->index].ret.tv_nsec, now_time.tv_sec, 
        now_time.tv_nsec, mf_mstons(interval));
    memcpy(o->arr[o->index].id, id, strlen(id)+1);
    if (o->max < o->index) { return 0;}
    o->index++;
    return 1;
    
  } else {
    int32_t index=print_time_find(o,id);
    struct timespec next_time = o->arr[index].ret;
    clock_gettime(CLOCK_REALTIME, &now_time);
    if (now_time.tv_sec > next_time.tv_sec || 
        (now_time.tv_sec==next_time.tv_sec && 
         now_time.tv_nsec >= next_time.tv_nsec)  ) {
      DLOG_PRINT_TIMED_LOGS("refresh  for %ld %ld nowtime %ld %ld input ns time %ld\n",
          o->arr[index].ret.tv_sec, o->arr[index].ret.tv_nsec, now_time.tv_sec, 
          now_time.tv_nsec, mf_mstons(interval));
      o->arr[index].ret=time_spec_add_ms(&now_time, interval);
      return 1;
    }
  }
  return 0;
}

int  test_timespec() {
  struct timespec a1={1,100};
  struct timespec a2={1,1212};
  //struct timespec a3=time_spec_add(a1, a2);
  //printf ("time is %ld %ld\n", a3.tv_sec, a3.tv_nsec);
  return 0;
 
}

int int_main() {
  while (1) {
    //if (print_timed_logs (&g_timespec_arr, "int_main1", 900)) { LOG_DEBUG (".9:Hello %s\n", "World"); }
    if (print_timed_logs (&g_timespec_arr, "int_main2", 1000)) { LOG_DEBUG ("1:Hello %s\n", "World"); }
    if (print_timed_logs (&g_timespec_arr, "int_main3", 5000)) { LOG_DEBUG ("5:Hello %s\n", "World"); }
  }
}

int main() {
  //test_timespec();
  int_main();
}
#endif


#ifdef CPP11
#include<iostream>
#include<map>
#include<string>
#include<chrono>

#define LOG_DEBUG(str, args...) printf (str, ##args)
class TimerLogs{ 
public:
  bool print_timed_logs (std::string id, std::chrono::milliseconds interval);
  bool delete_timed_logs (std::string id);
  void reset_timed_logs ();
private:
  std::map<std::string, std::chrono::_V2::system_clock::time_point> ret;
};

bool TimerLogs::print_timed_logs (std::string id, std::chrono::milliseconds interval) {
  if (ret.find(id) == ret.end()) {
    ret[id] = std::chrono::system_clock::now() + interval;
  } else {
    if (ret[id] >= std::chrono::system_clock::now()) {
      return false;
    } else {
      ret[id] = std::chrono::system_clock::now() + interval;
      return true;
    }
  }
  return true;
}

bool TimerLogs::delete_timed_logs (std::string id) {
  if (ret.find(id) == ret.end()) {
    ret.erase(id);
  } else {
      return false;
  }
  return true;
}

void TimerLogs::reset_timed_logs () {
  ret.clear();
}



int int_main() {
  TimerLogs a;
 while (1) {
    if (a.print_timed_logs ("int_main1", static_cast<std::chrono::milliseconds>(5000))) { LOG_DEBUG ("5:Hello %s\n", "World"); }
    if (a.print_timed_logs ("int_main2", static_cast<std::chrono::milliseconds>(1000))) { LOG_DEBUG ("1:Hello %s\n", "world"); }
  }
}

int main() {
  int_main();
}

#endif

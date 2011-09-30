#include "time.h"

double timeval_diff(struct timeval *end_time, struct timeval *start_time)
{
  struct timeval diff;

  diff.tv_sec =end_time->tv_sec -start_time->tv_sec ;
  diff.tv_usec=end_time->tv_usec-start_time->tv_usec;

  return (double)(diff.tv_sec+(double)diff.tv_usec/(double)CLOCKS_PER_SEC);
}

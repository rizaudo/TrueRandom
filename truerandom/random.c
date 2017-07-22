#define _GNU_SOURCE
#include "ruby/ruby.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/random.h>
#include <errno.h>


static VALUE
fill_random_bytes_random(void){

  char RandomData[16]={0};
  size_t length = 16;
  /* if entropy is not enough, raise EGAIN. */
  ssize_t result = syscall(SYS_getrandom, RandomData, length, GRND_RANDOM|GRND_NONBLOCK);
  if (result < 0){
    switch(errno){
    case EAGAIN:
      rb_raise(rb_eRuntimeError, "entropy is not available.");
    case EFAULT:
      rb_raise(rb_eRuntimeError, "meomry address is not accessible");
    case EINVAL:
      rb_raise(rb_eRuntimeError, "flag is invalid. please contact TrueRandom author.");
    }
  }
  return rb_str_new(RandomData, length);
}


void Init_truerandom(void){
  VALUE cTrueRandom = rb_define_class("TrueRandom", rb_cObject);
  rb_define_method(cTrueRandom, "random", fill_random_bytes_random, 0);
}

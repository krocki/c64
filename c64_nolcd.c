#include <stdio.h>    // printf
#include <stdlib.h>   // atoi
#include <sys/time.h>
#include "typedefs.h"
#include "6502.h"
double t0; // global start time

double get_time() {
  struct timeval tv; gettimeofday(&tv, NULL);
  return (tv.tv_sec + tv.tv_usec * 1e-6);
}

int read_bin(const char* fname, u8* ptr) {
  FILE *fp = fopen(fname, "rb");
  if (fp) {
    fseek(fp, 0, SEEK_END);
    u32 len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    fread(ptr, len, 1, fp);
    fclose(fp);
    printf("read file %s, %d bytes\n", fname, len);
    return 0;
  } else {
    fprintf(stderr, "error opening %s\n", fname);
    return -1;
  }
}

void *c64(void *args) {

  _6502 cpu;
  reset(&cpu, 0x3ff, 0xff);
  read_bin((const char*)args, &cpu.mem[0x400]);
  u32 iters = 1000000;
  t0=get_time();
  cpu_step(&cpu, iters);
  printf("%9.6f, terminating\n", get_time()-t0);
  printf("ticks %llu, time %.6f s, MHz %.3f\n", cpu.cyc, get_time()-t0, ((double)cpu.cyc/(1000000.0*(get_time()-t0))));
  return NULL;
}

int main(int argc, char **argv) {

  if (argc < 2)  { fprintf(stderr, "usage: %s rom_name\n", argv[0]); return -1; }
  if (argc >= 3) { show_debug=atoi(argv[2]);  }
  if (argc >= 4) { limit_speed=atoi(argv[3]); }

  printf("DEBUG=%d\n", show_debug); printf("SPEED_LIMIT=%d\n", limit_speed);
  c64(argv[1]);

  return 0;
}

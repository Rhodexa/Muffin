#ifndef SYNTH_H
#define SYNTH_H

#include "iobus.h"

/* These are my "abstracted" algorithms based on combinations of three different voices in different modes */
enum Algorithms{   // 2OP FM | 2OP ADD | 3OP FM | 4OP FM | 1OP ADD |
  ALG_1, //      3 |         |        |        |         |
  ALG_2, //      2 |       1 |        |        |         |
  ALG_3, //      1 |       2 |        |        |         |
  ALG_4, //        |       3 |        |        |         |
  ALG_5, //      1 |         |        |      1 |         |
  ALG_6, //        |       1 |        |      1 |         |
  ALG_7, //      1 |         |      1 |        |       1 |
  ALG_8, //        |       1 |      1 |        |       1 |
}

#endif
/* Copyright (c) 2011, Edgar Solomonik>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following 
 * conditions are met:
 *      * Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL EDGAR SOLOMONIK BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 * SERVICES LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF 
 * SUCH DAMAGE. */

#include "cyclopstf.hpp"
#include "dist_tensor_internal.h"
#include "../shared/util.h"
#include "../shared/unit_util.h"
#include "unit_test.h"
#include "unit_test_ctr.h"
#include "test_symmetry.hxx"
#include "test_sym_kernel.h"



/**
 * \brief verifies correctness of symmetric operations
 */
void test_symmetry(int const            argc, 
                   char **              argv, 
                   int const            numPes, 
                   int const            myRank, 
                   CommData_t *         cdt_glb){

  int seed, in_num, tid_A, tid_B, tid_C, pass;
  char ** input_str;
  CTF_ctr_type_t ctype;

  if (argc == 2) {
    read_param_file(argv[1], myRank, &input_str, &in_num);
  } else {
    input_str = argv;
    in_num = argc;
  }

  /*if (getCmdOption(input_str, input_str+in_num, "-nctr")){
    nctr = atoi(getCmdOption(input_str, input_str+in_num, "-nctr"));
    if (nctr < 0) nctr = 1;
  } else nctr = 1;*/
  if (getCmdOption(input_str, input_str+in_num, "-seed")){
    seed = atoi(getCmdOption(input_str, input_str+in_num, "-seed"));
    if (seed < 0) seed = 3;
  } else seed = 3;

  read_topology(input_str, in_num, myRank, numPes);

  const char * str_A = "A"; 
  const char * str_B = "B"; 
  const char * str_C = "C"; 

  read_tensor(input_str, in_num, str_A, &tid_A);
  test_sym_readwrite(seed, tid_A, myRank, numPes);

  read_tensor(input_str, in_num, str_B, &tid_B);
  test_sym_readwrite(seed, tid_B, myRank, numPes);

  read_tensor(input_str, in_num, str_C, &tid_C);
  test_sym_readwrite(seed, tid_C, myRank, numPes);

  read_ctr(input_str, in_num, tid_A, str_A, tid_B, str_B, tid_C, str_C, &ctype, -1);
  pass = test_sym_contract(&ctype, myRank, numPes);
//  test_sym_readwrite(seed, tid_A, myRank, numPes);
//  test_sym_readwrite(seed, tid_B, myRank, numPes);
//  test_sym_readwrite(seed, tid_C, myRank, numPes);

  CTF_exit();

  GLOBAL_BARRIER(cdt_glb);
  if (myRank==0) {
    if (pass){
      printf("Symmetric contraction test successfull.\n");
    } else {
      printf("Symmetric contraction test FAILED!!!\n");
    }
    printf("Symmetry tests completed\n");
  }
}

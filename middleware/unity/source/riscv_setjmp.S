/**
 * @file setimp.S
 * @brief Implement setjmp and longjmp function for at820.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-03-10
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 */

                .type    riscv_setjmp, %function
                .weak    riscv_setjmp
                .func    riscv_setjmp
riscv_setjmp:
                li       a1, 0
                sw       ra, 0*4(a0)
                sw       s0, 1*4(a0)
                sw       s1, 2*4(a0)
                sw       s2, 3*4(a0)
                sw       s3, 4*4(a0)
                sw       s4, 5*4(a0)
                sw       s5, 6*4(a0)
                sw       s6, 7*4(a0)
                sw       s7, 8*4(a0)
                sw       s8, 9*4(a0)
                sw       s9, 10*4(a0)
                sw       s10,11*4(a0)
                sw       s11,12*4(a0)
                sw       sp, 13*4(a0)
                li       a0, 0
                ret
                .endfunc
                .size    riscv_setjmp, . - riscv_setjmp

                .type    riscv_longjmp, %function
                .weak    riscv_longjmp
                .func    riscv_longjmp
riscv_longjmp:
                lw       ra, 0*4(a0)
                lw       s0, 1*4(a0)
                lw       s1, 2*4(a0)
                lw       s2, 3*4(a0)
                lw       s3, 4*4(a0)
                lw       s4, 5*4(a0)
                lw       s5, 6*4(a0)
                lw       s6, 7*4(a0)
                lw       s7, 8*4(a0)
                lw       s8, 9*4(a0)
                lw       s9, 10*4(a0)
                lw       s10,11*4(a0)
                lw       s11,12*4(a0)
                lw       sp, 13*4(a0)
                seqz     a0, a1
                add      a0, a0, a1     // a0 = (a1 == 0) ? 1 : a1
                ret
                .endfunc
                .size    riscv_longjmp, . - riscv_longjmp

                .end
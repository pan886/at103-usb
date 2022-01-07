/* Derived from arch/riscv/include/asm/io.h */
#ifndef __IO_H
#define __IO_H

#define __swab32(x) \
    ((unsigned long)((((unsigned long)(x) & (unsigned long)0x000000ffUL) << 24) | \
                     (((unsigned long)(x) & (unsigned long)0x0000ff00UL) << 8) | \
                     (((unsigned long)(x) & (unsigned long)0x00ff0000UL) >> 8) | \
                     (((unsigned long)(x) & (unsigned long)0xff000000UL) >> 24)))
#define __be32_to_cpu(x) __swab32(x)
#define cpu_to_le16(x)   (x)
#define le16_to_cpu(x)   (x)
#define le32_to_cpu(x)   (x)

// clang-format off
/* Generic IO read/write.  These perform native-endian accesses. */
#define __raw_writeb __raw_writeb
static inline void __raw_writeb(unsigned char val, volatile void *addr)
{
    asm volatile("sb %0, 0(%1)" : : "r" (val), "r" (addr));
}

#define __raw_writew __raw_writew
static inline void __raw_writew(unsigned short val, volatile void *addr)
{
    asm volatile("sh %0, 0(%1)" : : "r" (val), "r" (addr));
}

#define __raw_writel __raw_writel
static inline void __raw_writel(unsigned long val, volatile void *addr)
{
    asm volatile("sw %0, 0(%1)" : : "r" (val), "r" (addr));
}

#define __raw_readb __raw_readb
static inline unsigned char __raw_readb(const volatile void *addr)
{
    unsigned char val;
    asm volatile("lb %0, 0(%1)" : "=r" (val) : "r" (addr));
    return val;
}

#define __raw_readw __raw_readw
static inline unsigned short __raw_readw(const volatile void *addr)
{
    unsigned short val;
    asm volatile("lh %0, 0(%1)" : "=r" (val) : "r" (addr));
    return val;
}

#define __raw_readl __raw_readl
static inline unsigned long __raw_readl(const volatile void *addr)
{
    unsigned long val;
    asm volatile("lw %0, 0(%1)" : "=r" (val) : "r" (addr));
    return val;
}

/*
 * FIXME: I'm flip-flopping on whether or not we should keep this or enforce
 * the ordering with I/O on spinlocks like PowerPC does.  The worry is that
 * drivers won't get this correct, but I also don't want to introduce a fence
 * into the lock code that otherwise only uses AMOs (and is essentially defined
 * by the ISA to be correct).   For now I'm leaving this here: "o,w" is
 * sufficient to ensure that all writes to the device have completed before the
 * write to the spinlock is allowed to commit.  I surmised this from reading
 * "ACQUIRES VS I/O ACCESSES" in memory-barriers.txt.
 */
#define mmiowb() __asm__ __volatile__("fence o,w" : : : "memory");

/*
 * I/O memory access primitives. Reads are ordered relative to any
 * following Normal memory access. Writes are ordered relative to any prior
 * Normal memory access.  The memory barriers here are necessary as RISC-V
 * doesn't define any ordering between the memory space and the I/O space.
 */
#define __io_br() do {} while (0)
#define CONFIG_CORE_TM500_WA_FENCE
#ifdef CONFIG_CORE_TM500_WA_FENCE
/* fence instruction now is treated as UND with TM500. */
#define __io_ar() do {} while (0)
#define __io_bw() do {} while (0)
#else
#define __io_ar() __asm__ __volatile__("fence i,r" : : : "memory");
#define __io_bw() __asm__ __volatile__("fence w,o" : : : "memory");

#endif
#define __io_aw() do {} while (0)

#define readb(c) ({ unsigned char __v; __io_br(); __v = __raw_readb((const volatile void *)c); __io_ar(); __v; })
#define readw(c) ({ unsigned short __v; __io_br(); __v = __raw_readw((const volatile void *)c); __io_ar(); __v; })
#define readl(c) ({ unsigned long __v; __io_br(); __v = __raw_readl((const volatile void *)c); __io_ar(); __v; })
// clang-format on

#define writeb(v, c) ({ __io_bw(); __raw_writeb((v),((volatile void *)c)); __io_aw(); })
#define writew(v, c) ({ __io_bw(); __raw_writew((v),((volatile void *)c)); __io_aw(); })
#define writel(v, c) ({ __io_bw(); __raw_writel((v),((volatile void *)c)); __io_aw(); })

#endif

/*
 * Copyright (c) 2010, Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _ATHRS_8033_PHY_H
#define _ATHRS_8033_PHY_H

#define BOOL int


/* Phy Specific status fields */
#define ATHER_STATUS_LINK_MASK                0xC000
#define ATHER_STATUS_LINK_SHIFT               14
#define ATHER_STATUS_FULL_DEPLEX              0x2000
#define ATHR_STATUS_LINK_PASS                 0x0400 
#define ATHR_LATCH_LINK_PASS                  0x0004 

/* Advertisement register. */
#define ATHR_ADVERTISE_NEXT_PAGE              0x8000
#define ATHR_ADVERTISE_ASYM_PAUSE             0x0800
#define ATHR_ADVERTISE_PAUSE                  0x0400
#define ATHR_ADVERTISE_100FULL                0x0100
#define ATHR_ADVERTISE_100HALF                0x0080  
#define ATHR_ADVERTISE_10FULL                 0x0040  
#define ATHR_ADVERTISE_10HALF                 0x0020  
#define garyk 1

#define ATHR_ADVERTISE_ALL (ATHR_ADVERTISE_ASYM_PAUSE | ATHR_ADVERTISE_PAUSE | \
                            ATHR_ADVERTISE_10HALF | ATHR_ADVERTISE_10FULL | \
                            ATHR_ADVERTISE_100HALF | ATHR_ADVERTISE_100FULL)


//Gary
#ifdef ATH_MDC_GPIO
#if !garyk
#define ATH_GPIO	14
#define GPIO_FUNCTION4_MASK  	(GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MASK)
                		 
#define GPIO_FUNCTION4_ENABLE	(GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_SET(0x20))
#else
#define ATH_GPIO 22
#define ATH_GPIO21 21
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_MASK                       0x0000ff00
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_MASK                       0x00ff0000
#define GPIO_OUT_FUNCTION5_ADDRESS                                   0x18040040
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_LSB                        16
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_LSB                        8
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_SET(x)                     (((x) << GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_LSB) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_MASK)
#define GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_SET(x)                     (((x) << GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_LSB) & GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_MASK)
#define GPIO_FUNCTION5_MASK     (GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_MASK |\
                                 GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_MASK)
#define GPIO_FUNCTION5_ENABLE   (GPIO_OUT_FUNCTION5_ENABLE_GPIO_22_SET(0x20) | \
                                 GPIO_OUT_FUNCTION5_ENABLE_GPIO_21_SET(0x21))
     
#endif         			
#else 
#define GPIO_FUNCTION4_MASK  	(GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_MASK |\
                		 GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_MASK)

#define GPIO_FUNCTION4_ENABLE	(GPIO_OUT_FUNCTION4_ENABLE_GPIO_19_SET(0x20) | \
              			 GPIO_OUT_FUNCTION4_ENABLE_GPIO_17_SET(0x21))

#define ATH_GPIO	19
#endif

#define ATH_GPIO17 	17

/*
 *  Atheros header defines
 */
#ifndef _ATH_HEADER_CONF
#define _ATH_HEADER_CONF



#define ATHR_HEADER_LEN 2

typedef enum {
    NORMAL_PACKET,
    RESERVED0,
    MIB_1ST,
    RESERVED1,
    RESERVED2,
    READ_WRITE_REG,
    READ_WRITE_REG_ACK,
    RESERVED3
} AT_HEADER_TYPE;

#endif // _ATH_HEADER_CONF

#endif
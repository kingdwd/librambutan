/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2010 Perry Hung.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

/**
 * @file libmaple/include/libmaple/i2c.h
 * @brief Inter-Integrated Circuit (I2C) peripheral support
 */

#ifndef _LIBMAPLE_I2C_H_
#define _LIBMAPLE_I2C_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <libmaple/libmaple_types.h>
#include <libmaple/rcc.h>
#include <libmaple/nvic.h>
#include <libmaple/gpio.h>

/** I2C register map type */
typedef struct i2c_reg_map {
    __io uint32 CR1;            /**< Control register 1 */
    __io uint32 CR2;            /**< Control register 2 */
    __io uint32 OAR1;           /**< Own address register 1 */
    __io uint32 OAR2;           /**< Own address register 2 */
    __io uint32 DR;             /**< Data register */
    __io uint32 SR1;            /**< Status register 1 */
    __io uint32 SR2;            /**< Status register 2 */
    __io uint32 CCR;            /**< Clock control register */
    __io uint32 TRISE;          /**< TRISE (rise time) register */
} i2c_reg_map;

/** I2C device states */
typedef enum i2c_state {
    I2C_STATE_DISABLED          = 0, /**< Disabled */
    I2C_STATE_IDLE              = 1, /**< Idle */
    I2C_STATE_XFER_DONE         = 2, /**< Done with transfer */
    I2C_STATE_BUSY              = 3, /**< Busy */
    I2C_STATE_ERROR             = -1 /**< Error occurred */
} i2c_state;

/**
 * @brief I2C message type
 */
typedef struct i2c_msg {
    uint16 addr;                /**< Address */
#define I2C_MSG_READ            0x1
#define I2C_MSG_10BIT_ADDR      0x2
    uint16 flags;               /**< Bitwise OR of I2C_MSG_READ and
                                     I2C_MSG_10BIT_ADDR */
    uint16 length;              /**< Message length */
    uint16 xferred;             /**< Messages transferred */
    uint8 *data;                /**< Data */
} i2c_msg;

/**
 * @brief I2C device type.
 */
typedef struct i2c_dev {
    i2c_reg_map *regs;          /**< Register map */
    i2c_msg *msg;               /**< Messages */
    uint32 error_flags;         /**< Error flags, set on I2C error condition */
    volatile uint32 timestamp;  /**< For internal use */
    struct gpio_dev *gpio_port; /**< SDA, SCL pins' GPIO port */
    uint16 msgs_left;           /**< Messages left */
    uint8 sda_pin;              /**< SDA bit on gpio_port */
    uint8 scl_pin;              /**< SCL bit on gpio_port */
    rcc_clk_id clk_id;          /**< RCC clock information */
    nvic_irq_num ev_nvic_line;  /**< Event IRQ number */
    nvic_irq_num er_nvic_line;  /**< Error IRQ number */
    volatile i2c_state state;   /**< Device state */
} i2c_dev;

/*
 * Devices
 */

extern i2c_dev* const I2C1;
extern i2c_dev* const I2C2;

/*
 * Register map base pointers
 */

/** I2C1 register map base pointer */
#define I2C1_BASE               ((struct i2c_reg_map*)0x40005400)
/** I2C2 register map base pointer */
#define I2C2_BASE               ((struct i2c_reg_map*)0x40005800)

/*
 * Register bit definitions
 */

/* Control register 1 */

#define I2C_CR1_SWRST           (1U << 15)    // Software reset
#define I2C_CR1_ALERT           (1U << 13)    // SMBus alert
#define I2C_CR1_PEC             (1U << 12)    // Packet error checking
#define I2C_CR1_POS             (1U << 11)    // Acknowledge/PEC position
#define I2C_CR1_ACK             (1U << 10)    // Acknowledge enable
#define I2C_CR1_STOP            (1U << 9)     // Stop generation
#define I2C_CR1_START           (1U << 8)     // Start generation
#define I2C_CR1_PE              (1U << 0)     // Peripheral Enable

/* Control register 2 */

#define I2C_CR2_LAST            (1U << 12)    // DMA last transfer
#define I2C_CR2_DMAEN           (1U << 11)    // DMA requests enable
#define I2C_CR2_ITBUFEN         (1U << 10)    // Buffer interrupt enable
#define I2C_CR2_ITEVTEN         (1U << 9)     // Event interupt enable
#define I2C_CR2_ITERREN         (1U << 8)     // Error interupt enable
#define I2C_CR2_FREQ            0xFFF         // Peripheral input frequency

/* Clock control register */

#define I2C_CCR_FS              (1U << 15)    // Fast mode selection
#define I2C_CCR_DUTY            (1U << 14)    // 16/9 duty ratio
#define I2C_CCR_CCR             0xFFF         // Clock control bits

/* Status register 1 */

#define I2C_SR1_SB              (1U << 0)     // Start bit
#define I2C_SR1_ADDR            (1U << 1)     // Address sent/matched
#define I2C_SR1_BTF             (1U << 2)     // Byte transfer finished
#define I2C_SR1_ADD10           (1U << 3)     // 10-bit header sent
#define I2C_SR1_STOPF           (1U << 4)     // Stop detection
#define I2C_SR1_RXNE            (1U << 6)     // Data register not empty
#define I2C_SR1_TXE             (1U << 7)     // Data register empty
#define I2C_SR1_BERR            (1U << 8)     // Bus error
#define I2C_SR1_ARLO            (1U << 9)     // Arbitration lost
#define I2C_SR1_AF              (1U << 10)    // Acknowledge failure
#define I2C_SR1_OVR             (1U << 11)    // Overrun/underrun
#define I2C_SR1_PECERR          (1U << 12)    // PEC Error in reception
#define I2C_SR1_TIMEOUT         (1U << 14)    // Timeout or Tlow error
#define I2C_SR1_SMBALERT        (1U << 15)    // SMBus alert

/* Status register 2 */

#define I2C_SR2_MSL             (1U << 0)     // Master/slave
#define I2C_SR2_BUSY            (1U << 1)     // Bus busy
#define I2C_SR2_TRA             (1U << 2)     // Transmitter/receiver
#define I2C_SR2_GENCALL         (1U << 4)     // General call address
#define I2C_SR2_SMBDEFAULT      (1U << 5)     // SMBus device default address
#define I2C_SR2_SMBHOST         (1U << 6)     // SMBus host header
#define I2C_SR2_DUALF           (1U << 7)     // Dual flag
#define I2C_SR2_PEC             0xFF00        // Packet error checking register

/*
 * Convenience routines
 */

void i2c_init(i2c_dev *dev);

/* I2C enable options */
#define I2C_FAST_MODE           0x1           // 400 khz
#define I2C_DUTY_16_9           0x2           // 16/9 duty ratio
#define I2C_REMAP               0x4           // Use alternate pin mapping
#define I2C_BUS_RESET           0x8           // Perform a bus reset
void i2c_master_enable(i2c_dev *dev, uint32 flags);

#define I2C_ERROR_PROTOCOL      (-1)
#define I2C_ERROR_TIMEOUT       (-2)
int32 i2c_master_xfer(i2c_dev *dev, i2c_msg *msgs, uint16 num, uint32 timeout);

void i2c_bus_reset(const i2c_dev *dev);

/**
 * @brief Disable an I2C device
 *
 * This function disables the corresponding peripheral and marks dev's
 * state as I2C_STATE_DISABLED.
 *
 * @param dev Device to disable.
 */
static inline void i2c_disable(i2c_dev *dev) {
    dev->regs->CR1 &= ~I2C_CR1_PE;
    dev->state = I2C_STATE_DISABLED;
}

/**
 * @brief Turn on an I2C peripheral
 * @param dev Device to enable
 */
static inline void i2c_peripheral_enable(i2c_dev *dev) {
    dev->regs->CR1 |= I2C_CR1_PE;
}

/**
 * @brief Turn off an I2C peripheral
 * @param dev Device to turn off
 */
static inline void i2c_peripheral_disable(i2c_dev *dev) {
    dev->regs->CR1 &= ~I2C_CR1_PE;
}

/**
 * @brief Fill transmit register
 * @param dev I2C device
 * @param byte Byte to write
 */
static inline void i2c_write(i2c_dev *dev, uint8 byte) {
    dev->regs->DR = byte;
}

/**
 * @brief Set input clock frequency, in MHz
 * @param dev I2C device
 * @param freq Frequency in megahertz (2-36)
 */
static inline void i2c_set_input_clk(i2c_dev *dev, uint32 freq) {
    uint32 cr2 = dev->regs->CR2;
    cr2 &= ~I2C_CR2_FREQ;
    cr2 |= freq;
    dev->regs->CR2 = freq;
}

/**
 * @brief Set I2C clock control register. See RM008
 * @param dev I2C device
 * @param val Value to use for clock control register (in
 *            Fast/Standard mode)
 */
static inline void i2c_set_clk_control(i2c_dev *dev, uint32 val) {
    uint32 ccr = dev->regs->CCR;
    ccr &= ~I2C_CCR_CCR;
    ccr |= val;
    dev->regs->CCR = ccr;
}


/**
 * @brief Set SCL rise time
 * @param dev I2C device
 * @param trise Maximum rise time in fast/standard mode (see RM0008
 *              for relevant formula).
 */
static inline void i2c_set_trise(i2c_dev *dev, uint32 trise) {
    dev->regs->TRISE = trise;
}


/**
 * @brief Generate a start condition on the bus.
 * @param dev I2C device
 */
static inline void i2c_start_condition(i2c_dev *dev) {
    uint32 cr1;
    while ((cr1 = dev->regs->CR1) & (I2C_CR1_START |
                                     I2C_CR1_STOP  |
                                     I2C_CR1_PEC)) {
        ;
    }
    dev->regs->CR1 |= I2C_CR1_START;
}

/**
 * @brief Generate a stop condition on the bus
 * @param dev I2C device
 */
static inline void i2c_stop_condition(i2c_dev *dev) {
    uint32 cr1;
    while ((cr1 = dev->regs->CR1) & (I2C_CR1_START |
                                     I2C_CR1_STOP  |
                                     I2C_CR1_PEC)) {
        ;
    }
    dev->regs->CR1 |= I2C_CR1_STOP;
    while ((cr1 = dev->regs->CR1) & (I2C_CR1_START |
                                     I2C_CR1_STOP  |
                                     I2C_CR1_PEC)) {
        ;
    }

}

/**
 * @brief Enable one or more I2C interrupts
 * @param dev I2C device
 * @param irqs Bitwise or of:
 *             I2C_IRQ_ERROR (error interrupt),
 *             I2C_IRQ_EVENT (event interrupt), and
 *             I2C_IRQ_BUFFER (buffer interrupt).
 */
#define I2C_IRQ_ERROR              I2C_CR2_ITERREN
#define I2C_IRQ_EVENT              I2C_CR2_ITEVTEN
#define I2C_IRQ_BUFFER             I2C_CR2_ITBUFEN
static inline void i2c_enable_irq(i2c_dev *dev, uint32 irqs) {
    dev->regs->CR2 |= irqs;
}

/**
 * @brief Disable one or more I2C interrupts
 * @param dev I2C device
 * @param irqs Bitwise or of:
 *             I2C_IRQ_ERROR (error interrupt),
 *             I2C_IRQ_EVENT (event interrupt), and
 *             I2C_IRQ_BUFFER (buffer interrupt).
 */
static inline void i2c_disable_irq(i2c_dev *dev, uint32 irqs) {
    dev->regs->CR2 &= ~irqs;
}


/**
 * @brief Enable I2C acknowledgment
 * @param dev I2C device
 */
static inline void i2c_enable_ack(i2c_dev *dev) {
    dev->regs->CR1 |= I2C_CR1_ACK;
}

/**
 * @brief Disable I2C acknowledgment
 * @param dev I2C device
 */
static inline void i2c_disable_ack(i2c_dev *dev) {
    dev->regs->CR1 &= ~I2C_CR1_ACK;
}

#ifdef __cplusplus
}
#endif

#endif

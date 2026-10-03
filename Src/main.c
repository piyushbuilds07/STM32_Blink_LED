#include <stdint.h> // Standard integer types (uint32_t, etc.)

// 1. Define the base memory addresses for the registers we need
// (These addresses are specific to the STM32L4 series)
#define RCC_BASE      0x40021000UL
#define GPIOA_BASE    0x48000000UL

// 2. Define the specific registers using pointers to those addresses
#define RCC_AHB2ENR   (*((volatile uint32_t *)(RCC_BASE + 0x4C)))
#define GPIOA_MODER   (*((volatile uint32_t *)(GPIOA_BASE + 0x00)))
#define GPIOA_OTYPER  (*((volatile uint32_t *)(GPIOA_BASE + 0x04)))
#define GPIOA_OSPEEDR (*((volatile uint32_t *)(GPIOA_BASE + 0x08)))
#define GPIOA_PUPDR   (*((volatile uint32_t *)(GPIOA_BASE + 0x0C)))
#define GPIOA_ODR     (*((volatile uint32_t *)(GPIOA_BASE + 0x14)))

// 3. Simple software delay function
void simple_delay(volatile uint32_t count) {
    while (count--) {
        __asm("nop");
    }
}

int main(void) {
    // 4. Enable the clock for GPIOA (Bit 0 of AHB2ENR)
    RCC_AHB2ENR |= (1 << 0);

    // 5. Configure PA5 as Output
    // Clear bits 10 and 11 (for pin 5)
    GPIOA_MODER &= ~(3 << (5 * 2));
    // Set bits 10 and 11 to 01 (Output mode)
    GPIOA_MODER |= (1 << (5 * 2));

    // 6. Set PA5 to Push-Pull
    GPIOA_OTYPER &= ~(1 << 5);

    // 7. Set PA5 speed to Low
    GPIOA_OSPEEDR &= ~(3 << (5 * 2));

    // 8. Disable pull-up/pull-down for PA5
    GPIOA_PUPDR &= ~(3 << (5 * 2));

    // 9. Infinite loop
    while (1) {
        // Toggle PA5 (XOR the bit)
        GPIOA_ODR ^= (1 << 5);

        // Wait
        simple_delay(500000);
    }
}

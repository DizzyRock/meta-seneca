#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>

#define I2C_DEVICE "/dev/i2c-5"
#define GAMEPAD_ADDR 0x50
#define BUTTON_MASK 0b011000111
#define JOYSTICK_X_CHANNEL 14
#define JOYSTICK_Y_CHANNEL 15

#define BUTTON_X_MASK (1 << 6)
#define BUTTON_Y_MASK (1 << 5)
#define BUTTON_A_MASK (1 << 4)
#define BUTTON_B_MASK (1 << 3)
#define BUTTON_SELECT_MASK (1 << 2)
#define BUTTON_START_MASK (1 << 1)

#define SEESAW_GPIO_BASE 0x01
#define SEESAW_GPIO_BULK_INPUT 0x04
#define SEESAW_ADC_BASE 0x09
#define SEESAW_ADC_CHANNEL_OFFSET 0x07
#define SEESAW_STATUS_BASE 0x00
#define SEESAW_STATUS_SWRST 0x7F

void seesaw_init(int i2c_fd) {
    uint8_t reset_cmd[] = {SEESAW_STATUS_SWRST, 0xFF};
    write(i2c_fd, reset_cmd, sizeof(reset_cmd));
    usleep(500 * 1000);
}

uint32_t gamepad_read_buttons(int i2c_fd) {
    uint8_t cmd[] = {SEESAW_GPIO_BASE, SEESAW_GPIO_BULK_INPUT};
    uint32_t buttons_state = 0;
    
    write(i2c_fd, cmd, sizeof(cmd));
    read(i2c_fd, &buttons_state, sizeof(buttons_state));
    
    return buttons_state;
}

uint16_t seesaw_analog_read(int i2c_fd, uint8_t channel) {
    uint8_t cmd[] = {SEESAW_ADC_BASE, SEESAW_ADC_CHANNEL_OFFSET + channel};
    uint16_t analog_value = 0;
    
    write(i2c_fd, cmd, sizeof(cmd));
    read(i2c_fd, &analog_value, sizeof(analog_value));
    
    return analog_value;
}

int main() {
    int i2c_fd = open(I2C_DEVICE, O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open the I2C bus");
        return EXIT_FAILURE;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, GAMEPAD_ADDR) < 0) {
        perror("Failed to set I2C address");
        return EXIT_FAILURE;
    }

    seesaw_init(i2c_fd);

    while (1) {
        uint32_t buttons = gamepad_read_buttons(i2c_fd);
        printf("Button state: 0x%08x\n", buttons);

        if (buttons & BUTTON_X_MASK) {
        printf("Button X pressed\n");
        }
        if (buttons & BUTTON_Y_MASK) {
            printf("Button Y pressed\n");
        }
        if (buttons & BUTTON_A_MASK) {
            printf("Button A pressed\n");
        }
        if (buttons & BUTTON_B_MASK) {
            printf("Button B pressed\n");
        }
        if (buttons & BUTTON_SELECT_MASK) {
            printf("Button SELECT pressed\n");
        }
        if (buttons & BUTTON_START_MASK) {
            printf("Button START pressed\n");
        }
    
        uint16_t x = seesaw_analog_read(i2c_fd, JOYSTICK_X_CHANNEL);
        uint16_t y = seesaw_analog_read(i2c_fd, JOYSTICK_Y_CHANNEL);
        printf("Joystick X: %d, Y: %d\n", x, y);

        sleep(1);
    }

    close(i2c_fd);
    return EXIT_SUCCESS;
}

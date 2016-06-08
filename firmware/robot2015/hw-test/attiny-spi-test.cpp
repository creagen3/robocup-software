#include "mbed.h"
#include "KickerBoard.hpp"
#include "pins-ctrl-2015.hpp"
#include "SharedSPI.hpp"

LocalFileSystem fs("local");

// DigitalOut cs(p8);
Serial pc(USBTX, USBRX);  // tx and rx
// DigitalOut n_kick(p9);

int main() {
    pc.baud(57600);  // set up the serial
    shared_ptr<SharedSPI> sharedSPI =
        make_shared<SharedSPI>(RJ_SPI_MOSI, RJ_SPI_MISO, RJ_SPI_SCK);
    sharedSPI->format(
        8, 0);  // 8 bits per transferrintf("About to flash kicker\r\n");
    KickerBoard kickerBoard(sharedSPI, RJ_KICKER_nCS, RJ_KICKER_nRESET,
                            "/local/rj-kickr.nib");  // nCs, nReset
    bool kickerReady = kickerBoard.flash(true, true);
    printf("Flashed kicker, success = %s\r\n", kickerReady ? "TRUE" : "FALSE");

    char getCmd = 'k';

    while (1) {
        //    n_kick = !n_kick;
        //    wait(1);
        if (pc.readable()) {
            getCmd = pc.getc();
            // if (getCmd == 'k') getCmd = 'c';
            // else if (getCmd == 'c') getCmd = 'r';
            // else if (getCmd == 'r') getCmd = 'k';
            pc.printf("%c: ", getCmd);
            switch (getCmd) {
                case 'k':
                    pc.printf("Resp: 0x%02X", kickerBoard.kick(20));
                    break;
                case 'c':
                    pc.printf("Resp: 0x%02X", kickerBoard.chip(20));
                    break;
                case 'r':
                    pc.printf("Volts: %d", kickerBoard.read_voltage());
                    break;
                case 'h':
                    pc.printf("Resp: 0x%02X", kickerBoard.charge());
                    break;
                case 'j':
                    pc.printf("Resp: 0x%02X", kickerBoard.stop_charging());
                    break;
                case 'p':
                    pc.printf("Resp: 0x%02X", kickerBoard.is_pingable());
                    break;
                case '1':
                    pc.printf("Kick Resp: 0x%02X",
                              kickerBoard.is_kick_debug_pressed());
                    break;
                case '2':
                    pc.printf("Chip Resp: 0x%02X",
                              kickerBoard.is_chip_debug_pressed());
                    break;
                case '3':
                    pc.printf("Charge Resp: 0x%02X",
                              kickerBoard.is_charge_debug_pressed());
                    break;
                default:
                    break;
            }
            pc.printf("\r\n");
            // wait_ms(500);
        }
    }
}
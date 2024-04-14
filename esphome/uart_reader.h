#include "esphome.h"
#define BUFFER_LEN 4096

class UARTReader : public Component, public UARTDevice, public TextSensor {
    protected:
      uint8_t uart_buffer[BUFFER_LEN]{0};
      int offset = 0;

    public:
        UARTReader(UARTComponent *parent) : UARTDevice(parent) {}

        void setup() override {
        // nothing to do here
        }

        int readline(int readch, char *buffer, int len) {
          static int pos = 0;
          int rpos;

          if (readch > 0) {
            switch (readch) {
              case '\n': // Ignore new-lines
                break;
              case '\r': // Return on CR
                rpos = pos;
                pos = 0;  // Reset position index ready for next time
                return rpos;
              default:
                if (pos < len-1) {
                  buffer[pos++] = readch;
                  buffer[pos] = 0;
                }
            }
          }
          // No end of line has been found, so return -1.
          return -1;
        }

        void loop() override {
          const int max_line_length = 180;
          static char buffer[max_line_length];
          while (available()) {
            if(readline(read(), buffer, max_line_length) > 0) {
              publish_state(buffer);
            }
          }
        }
};



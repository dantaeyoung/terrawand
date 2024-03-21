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
        void loop() override {
            while (available()) {
              char c = read();
              this->uart_buffer[offset++] = c;
              if (offset > BUFFER_LEN - 1) {
                offset = 0;
                break;
              }
              if (c == '\r') {
                this->uart_buffer[offset] = '\0';
                ESP_LOGD("BARCODE READ", "[%d] %s", offset, this->uart_buffer);
                publish_state(this->uart_buffer);
                offset = 0;
                break;
              }

            }
        }
};



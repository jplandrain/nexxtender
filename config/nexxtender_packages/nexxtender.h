#ifndef NEXXTENDER_UTILS_H
#define NEXXTENDER_UTILS_H

uint32_t getInt(String x, int i) {
    return (uint32_t(x[i+3]) << 24) | (uint32_t(x[i+2]) << 16) | (uint32_t(x[i+1]) << 8) | uint32_t(x[i]);
}

int16_t getByte(String x, int i) {
    return x[i] & 0xFF;
}

int16_t getShort(String x, int i) {
    return (int16_t(x[i+1]) << 8) | int16_t(x[i]);
}

int8_t getInt8(String x, int i) {
    uint8_t byte = x[i];
    if (byte & 0x80) {
        return static_cast<int8_t>(byte) | 0xFF00;
    } else {
        return static_cast<int8_t>(byte);
    }
}

uint16_t crc16(String data, size_t length) {
    uint16_t crc = 0xFFFF;
    const uint16_t polynomial = 0xA001;

    for (size_t i = 0; i < length; ++i) {
        crc ^= static_cast<uint16_t>(data[i]);
        for (size_t j = 0; j < 8; ++j) {
            if ((crc & 0x0001) != 0) {
                crc = (crc >> 1) ^ polynomial;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

void add_crc_to_data(String& data, size_t length) {
    // Calculate CRC for the original data excluding the last two bytes
    uint16_t crc = crc16(data, length - 2);
    
    // Add the CRC to the end of the data array
    data[length - 2] = crc & 0xFF;           // LSB of CRC
    data[length - 1] = (crc >> 8) & 0xFF;    // MSB of CRC
}

bool check_crc(String& data, size_t length) {
    // Calculate CRC for the original data excluding the last two bytes
    uint16_t calculated_crc = crc16(data, length - 2);

    // Extract the CRC bytes from the data array
    uint16_t received_crc = (data[length - 1] << 8) | data[length - 2];

    ESP_LOGD("check_crc", "Checksum Check: received: %d, calculated: %d", received_crc, calculated_crc);

    // Compare the calculated CRC with the received CRC
    return calculated_crc == received_crc;
}

String getTimeString(int minutes) {
    char time_string[20];
    minutes += 60; // add 1 hour due to timezone
    int hours = minutes / 60;
    int remaining_minutes = minutes % 60;
    sprintf(time_string, "%dh%02d", hours, remaining_minutes);
    return time_string;
}

void logd_x(const char* device_name, String x) {
    auto hex_data = format_hex_pretty((uint8_t *) x.c_str(), x.size());
    ESP_LOGD(device_name, "%s", hex_data.c_str());
}

void logd_s(const char* device_name, String x) {
    ESP_LOGD(device_name, "%s", x.c_str());
}

#endif

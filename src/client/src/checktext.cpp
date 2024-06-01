#include "client.hpp"

bool isASCII(char byte) {
    return byte >= 0 && byte <= 127;
}

bool isUTF8StartByte(char byte) {
    return (byte & 0xC0) != 0x80;
}

bool isUTF8ContinuationByte(char byte) {
    return (byte & 0xC0) == 0x80;
}

bool isUTF8(std::ifstream& file) {
    char byte;
    while (file.get(byte)) {
        if ((byte & 0x80) == 0) {
            // Single-byte character (ASCII)
            continue;
        } else if ((byte & 0xE0) == 0xC0) {
            // Two-byte character
            char nextByte;
            if (!file.get(nextByte)) return false;
            if (!isUTF8ContinuationByte(nextByte)) return false;
        } else if ((byte & 0xF0) == 0xE0) {
            // Three-byte character
            char nextByte;
            if (!file.get(nextByte)) return false;
            if (!isUTF8ContinuationByte(nextByte)) return false;
            if (!file.get(nextByte)) return false;
            if (!isUTF8ContinuationByte(nextByte)) return false;
        } else if ((byte & 0xF8) == 0xF0) {
            // Four-byte character
            char nextByte;
            if (!file.get(nextByte)) return false;
            if (!isUTF8ContinuationByte(nextByte)) return false;
            if (!file.get(nextByte)) return false;
            if (!isUTF8ContinuationByte(nextByte)) return false;
            if (!file.get(nextByte)) return false;
            if (!isUTF8ContinuationByte(nextByte)) return false;
        } else {
            // Invalid UTF-8 sequence
            return false;
        }
    }
    return true;
}

bool isISO8859_1(std::ifstream& file) {
    char byte;
    while (file.get(byte)) {
        if ((byte & 0x80) != 0) {
            // ISO-8859-1 does not have any characters above 127
            return false;
        }
    }
    return true;
}

bool isASCII_Big5(std::ifstream& file) {
    // Big5 is a double-byte character encoding
    char byte1, byte2;
    while (file.get(byte1) && file.get(byte2)) {
        if (!isASCII(byte1) || !isASCII(byte2)) {
            // Non-ASCII character found
            return false;
        }
    }
    return true;
}

bool isTextFile(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        std::cerr << "Error: Failed to open file." << std::endl;
        return true;
    }

    // Check if the file is UTF-8 encoded
    if (isUTF8(file)) {
        return true;
    }

    // Check if the file is ISO-8859-1 encoded
    file.clear();
    file.seekg(0);
    if (isISO8859_1(file)) {
        return true;
    }

    // Check if the file is ASCII+Big5 encoded
    file.clear();
    file.seekg(0);
    if (isASCII_Big5(file)) {
        return true;
    }

    // The file is not ASCII, UTF-8, ISO-8859-1, or ASCII+Big5
    return false;
}
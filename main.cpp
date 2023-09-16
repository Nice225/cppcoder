#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <chrono>
#include <codecvt>
#include <locale>
#include <cppcodec/base64_rfc4648.hpp>

// Функция для конвертации текста в кодировку Windows-1251
std::string ConvertToWindows1251(const std::string& inputText, std::chrono::duration<double>& encodingTime) {
    auto start = std::chrono::high_resolution_clock::now();

    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, inputText.c_str(), -1, nullptr, 0);
    if (requiredSize == 0) {
        encodingTime = std::chrono::duration<double>::zero();
        return "";
    }

    wchar_t* wideBuffer = new wchar_t[requiredSize];
    MultiByteToWideChar(CP_UTF8, 0, inputText.c_str(), -1, wideBuffer, requiredSize);

    requiredSize = WideCharToMultiByte(1251, 0, wideBuffer, -1, nullptr, 0, nullptr, nullptr);
    if (requiredSize == 0) {
        encodingTime = std::chrono::duration<double>::zero();
        delete[] wideBuffer;
        return "";
    }

    char* resultBuffer = new char[requiredSize];
    WideCharToMultiByte(1251, 0, wideBuffer, -1, resultBuffer, requiredSize, nullptr, nullptr);

    encodingTime = std::chrono::high_resolution_clock::now() - start;

    std::string result(resultBuffer);
    delete[] wideBuffer;
    delete[] resultBuffer;
    return result;
}

// Функция для конвертации текста в кодировку IBM866 (OEM)
std::string ConvertToIBM866(const std::string& inputText, std::chrono::duration<double>& encodingTime) {
    auto start = std::chrono::high_resolution_clock::now();

    int requiredSize = MultiByteToWideChar(CP_UTF8, 0, inputText.c_str(), -1, nullptr, 0);
    if (requiredSize == 0) {
        encodingTime = std::chrono::duration<double>::zero();
        return "";
    }

    wchar_t* wideBuffer = new wchar_t[requiredSize];
    MultiByteToWideChar(CP_UTF8, 0, inputText.c_str(), -1, wideBuffer, requiredSize);

    requiredSize = WideCharToMultiByte(866, 0, wideBuffer, -1, nullptr, 0, nullptr, nullptr);
    if (requiredSize == 0) {
        encodingTime = std::chrono::duration<double>::zero();
        delete[] wideBuffer;
        return "";
    }

    char* resultBuffer = new char[requiredSize];
    WideCharToMultiByte(866, 0, wideBuffer, -1, resultBuffer, requiredSize, nullptr, nullptr);

    encodingTime = std::chrono::high_resolution_clock::now() - start;

    std::string result(resultBuffer);
    delete[] wideBuffer;
    delete[] resultBuffer;
    return result;
}

// Функция для конвертации текста в UTF-16LE (little-endian)
std::string ConvertToUTF16LE(const std::string& inputText, std::chrono::duration<double>& encodingTime) {
    auto start = std::chrono::high_resolution_clock::now();

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utf16_conv;
    std::wstring utf16_text = utf16_conv.from_bytes(inputText);

    encodingTime = std::chrono::high_resolution_clock::now() - start;

    return std::string(reinterpret_cast<const char*>(utf16_text.c_str()), utf16_text.size() * sizeof(wchar_t));
}

// Функция для конвертации текста в UTF-32LE (little-endian)
std::string ConvertToUTF32LE(const std::string& inputText, std::chrono::duration<double>& encodingTime) {
    auto start = std::chrono::high_resolution_clock::now();

    std::wstring_convert<std::codecvt_utf8_utf32<wchar_t>> utf32_conv;
    std::wstring utf32_text = utf32_conv.from_bytes(inputText);

    encodingTime = std::chrono::high_resolution_clock::now() - start;

    return std::string(reinterpret_cast<const char*>(utf32_text.c_str()), utf32_text.size() * sizeof(wchar_t));
}

// Функция для конвертации текста в кодировку URL
std::string ConvertToURL(const std::string& inputText, std::chrono::duration<double>& encodingTime) {
    auto start = std::chrono::high_resolution_clock::now();

    std::ostringstream url_encoded;
    for (char c : inputText) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            url_encoded << c;
        } else {
            url_encoded << '%' << std::uppercase << std::hex << static_cast<int>(c);
        }
    }

    encodingTime = std::chrono::high_resolution_clock::now() - start;

    return url_encoded.str();
}

// Функция для конвертации текста в Base64
std::string ConvertToBase64(const std::string& inputText, std::chrono::duration<double>& encodingTime) {
    auto start = std::chrono::high_resolution_clock::now();

    std::string base64_encoded = cppcodec::base64_rfc4648::encode(inputText);

    encodingTime = std::chrono::high_resolution_clock::now() - start;

    return base64_encoded;
}

// Функция для записи времени в файл
void WriteTimeToFile(const std::string& filename, const std::chrono::duration<double>& time) {
    std::ofstream timeFile(filename);
    if (!timeFile) {
        std::cerr << "Не удалось открыть файл для записи времени: " << filename << std::endl;
        return;
    }

    timeFile << "Время выполнения: " << time.count() << " секунд" << std::endl;
    timeFile.close();
}
// Функция для записи текста в файл
void WriteToFile(const std::string& text, const std::string& filename) {
    std::ofstream outputFile(filename);
    if (!outputFile) {
        std::cerr << "Не удалось открыть файл для записи: " << filename << std::endl;
        return;
    }

    outputFile << text;
    outputFile.close();
}


int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "Не удалось открыть исходный файл." << std::endl;
        return 1;
    }

    std::string text;
    std::getline(inputFile, text);

    std::chrono::duration<double> encodingTime;

    // Конвертация в кодировку Windows-1251
    auto windows1251_text = ConvertToWindows1251(text, encodingTime);
    WriteToFile(windows1251_text, "output_windows1251.txt");
    WriteTimeToFile("encoding_time_windows1251.txt", encodingTime);

    // Конвертация в кодировку IBM866 (OEM)
    auto ibm866_text = ConvertToIBM866(text, encodingTime);
    WriteToFile(ibm866_text, "output_ibm866.txt");
    WriteTimeToFile("encoding_time_ibm866.txt", encodingTime);

    // Конвертация в UTF-16LE
    auto utf16_text = ConvertToUTF16LE(text, encodingTime);
    WriteToFile(utf16_text, "output_utf16.txt");
    WriteTimeToFile("encoding_time_utf16.txt", encodingTime);

    // Конвертация в UTF-32LE
    auto utf32_text = ConvertToUTF32LE(text, encodingTime);
    WriteToFile(utf32_text, "output_utf32.txt");
    WriteTimeToFile("encoding_time_utf32.txt", encodingTime);

    // Конвертация в URL-кодировку
    auto url_encoded = ConvertToURL(text, encodingTime);
    WriteToFile(url_encoded, "output_url.txt");
    WriteTimeToFile("encoding_time_url.txt", encodingTime);

    // Конвертация в Base64
    auto base64_encoded = ConvertToBase64(text, encodingTime);
    WriteToFile(base64_encoded, "output_base64.txt");
    WriteTimeToFile("encoding_time_base64.txt", encodingTime);

    std::cout << "Конвертация завершена успешно." << std::endl;
    return 0;
}

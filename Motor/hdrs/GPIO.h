#pragma once

#include <string>

enum class PinDir { In, Out };
enum class PinLvl { Low, High };

class GPIO
{
public:
    GPIO();
    virtual ~GPIO();

    static bool SetPinDir(unsigned int pinNum, PinDir dir);
    static bool SetPinLvl(unsigned int pinNum, PinLvl lvl);
    static bool GetPinLvl(unsigned int pinNum, PinLvl& lvl);

    static std::string PinGpioNum(unsigned int pinNum);

    static bool PinIsGpio(unsigned int pinNum);
    static bool PinIsValid(unsigned int pinNum);

    static std::string GpioFile(unsigned int pinNum);

    static bool ExportPin(unsigned int pinNum);
    static bool UnexportPin(unsigned int pinNum);

    static std::string PinDirToStr(PinDir dir);
    static std::string PinLvlToStr(PinLvl lvl);
    static bool LvlStrToPinLvl(const std::string& lvlStr, PinLvl& lvl);

    static void DelayMs(unsigned int ms);

private:
    static bool WriteToFile(const std::string& fname, const std::string& data);
    static bool ReadFromFile(const std::string& fname, std::string& data);
};

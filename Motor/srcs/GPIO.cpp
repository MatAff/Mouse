#include "GPIO.h"

#include <fstream>
#include <iostream>
#include <map>
#include <unistd.h>

#define LOWEST_PIN 1
#define HIGHEST_PIN 40

static const std::string GpioExport = "/sys/class/gpio/export";
static const std::string GpioUnexport = "/sys/class/gpio/unexport";
static const std::string GpioRoot = "/sys/class/gpio/gpio";

struct Pin
{
  std::string description;
  std::string gpioNum; // empty if not GPIO
};

// Pi3
static const std::map<unsigned int, Pin> PinMap =
{
    { 1, { "3.3V DC Power", "" }},
    { 2, { "5V DC Power", "" }},
    { 3, { "SDA1, I2C", "2" }},
    { 4, { "5V DC Power", "" }},
    { 5, { "SCL1, I2C", "3" }},
    { 6, { "GND", "" }},
    { 7, { "GPIO_GCLK", "4" }},
    { 8, { "TXD0", "14" }},
    { 9, { "GND", "" }},
    {10, { "RDX0", "15" }},
    {11, { "GPIO_GEN0", "17" }},
    {12, { "GPIO_GEN1", "18" }},
    {13, { "GPIO_GEN2", "27" }},
    {14, { "GND", "" }},
    {15, { "GPIO_GEN3", "22" }},
    {16, { "GPIO_GEN4", "23" }},
    {17, { "3.3V DC Power", "" }},
    {18, { "GPIO_GEN5", "24" }},
    {19, { "SPI_MOSI", "10" }},
    {20, { "GND", "" }},
    {21, { "SPI_MISO", "9" }},
    {22, { "GPIO_GEN6", "25" }},
    {23, { "SPI_CLK", "11" }},
    {24, { "SPI_CE0_N", "8" }},
    {25, { "GND", "" }},
    {26, { "SPI_CE1_N", "7" }},
    {27, { "ID_SD (I2C ID EEPROM)", "" }},
    {28, { "ID_SC (I2C ID EEPROM)", "" }},
    {29, { "GPIO5", "5" }},
    {30, { "GND", "" }},
    {31, { "GPIO6", "6" }},
    {32, { "GPIO12", "12" }},
    {33, { "GPIO13", "13" }},
    {34, { "GND", "" }},
    {35, { "GPIO19", "19" }},
    {36, { "GPIO16", "16" }},
    {37, { "GPIO26", "26" }},
    {38, { "GPIO20", "20" }},
    {39, { "GND", "" }},
    {40, { "GPIO21", "21" }}
};

GPIO::GPIO() {}

GPIO::~GPIO() {}

bool GPIO::SetPinDir(unsigned int pinNum, PinDir dir)
{
    bool didSetPinDir = false;
    std::string sDir = PinDirToStr(dir);

    if (PinIsGpio(pinNum) && ExportPin(pinNum) && !sDir.empty())
    {
        didSetPinDir = WriteToFile(GpioFile(pinNum) + "/direction",  sDir);
    }

    return didSetPinDir;
}

bool GPIO::SetPinLvl(unsigned int pinNum, PinLvl lvl)
{
    bool didSetPinLvl = false;
    std::string sLvl = PinLvlToStr(lvl);

    // Assumes pin direction is already set, and thus it was exported.
    if (PinIsGpio(pinNum) && !sLvl.empty())
    {
        didSetPinLvl = WriteToFile(GpioFile(pinNum) + "/value", sLvl);
    }

    return didSetPinLvl;
}

bool GPIO::GetPinLvl(unsigned int pinNum, PinLvl& lvl)
{
    bool didGetPinLvl = false;
    std::string sLvl = "";

    if (PinIsGpio(pinNum) && ReadFromFile(GpioFile(pinNum) + "/value", sLvl))
    {
        didGetPinLvl = LvlStrToPinLvl(sLvl, lvl);
    }

    return didGetPinLvl;
}

std::string GPIO::GpioFile(unsigned int pinNum)
{
    return GpioRoot + PinGpioNum(pinNum);
}

bool GPIO::ExportPin(unsigned int pinNum)
{
    return PinIsValid(pinNum) ? WriteToFile(GpioExport, PinGpioNum(pinNum)) : false;
}

bool GPIO::UnexportPin(unsigned int pinNum)
{
    return PinIsValid(pinNum) ? WriteToFile(GpioUnexport, PinGpioNum(pinNum)) : false;
}

bool GPIO::WriteToFile(const std::string& fname, const std::string& data)
{
    bool wroteToFile = false;

    std::ofstream ofs(fname.c_str());

    if (ofs.is_open())
    {
        ofs << data;
        ofs.close();
        wroteToFile = true;
    }
    else
    {
        std::cerr << "ERROR: GPIO could not write to file '" << fname << "'." << std::endl;
    }

    return wroteToFile;
}

bool GPIO::ReadFromFile(const std::string& fname, std::string& data)
{
    bool readFromFile = false;

    std::ifstream ifs(fname.c_str());

    if (ifs.is_open())
    {
        ifs >> data;
        ifs.close();
        readFromFile = true;
    }
    else
    {
        std::cerr << "ERROR: GPIO could not read from file '" << fname << "'." << std::endl;
    }

    return readFromFile;
}

bool GPIO::PinIsValid(unsigned int pinNum)
{
    return pinNum >= LOWEST_PIN && pinNum <= HIGHEST_PIN;
}

bool GPIO::PinIsGpio(unsigned int pinNum)
{
    return  PinGpioNum(pinNum) != "";
}

std::string GPIO::PinGpioNum(unsigned int pinNum)
{
    return  PinIsValid(pinNum) ? PinMap.find(pinNum)->second.gpioNum : "";
}

std::string GPIO::PinDirToStr(PinDir dir)
{
    std::string dirStr = "";

    switch (dir)
    {
    case PinDir::In: dirStr = "in"; break;
    case PinDir::Out: dirStr = "out"; break;
    default: std::cerr << "ERROR: Invalid PinDir provided to GPIO::PinDirToStr()." << std::endl;
    }

    return dirStr;
}

bool GPIO::LvlStrToPinLvl(const std::string& lvlStr, PinLvl& lvl)
{
    bool couldConvert = false;

    if (lvlStr == "0")
    {
        lvl = PinLvl::Low;
        couldConvert = true;
    }
    else if (lvlStr == "1")
    {
        lvl = PinLvl::High;
        couldConvert = true;
    }
    else
    {
        std::cerr << "ERROR: Could not convert string('" << lvlStr << "') to PinLvl." << std::endl;
    }

    return couldConvert;
}

std::string GPIO::PinLvlToStr(PinLvl lvl)
{
    std::string lvlStr = "";

    switch (lvl)
    {
    case PinLvl::Low: lvlStr = "0"; break;
    case PinLvl::High: lvlStr = "1"; break;
    default: std::cerr << "ERROR: Invalid PinLvl provided to GPIO::PinLvlToStr()." << std::endl;
    }

    return lvlStr;
}

void GPIO::DelayMs(unsigned int ms)
{
    usleep(ms*1000);
}

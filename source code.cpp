#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class Device {
protected:
    int deviceID;
    std::string deviceName;
    bool status; // ON/OFF

public:
    Device(int id, const std::string& name) : deviceID(id), deviceName(name), status(false) {}

    virtual void turnOn() {
        status = true;
        std::cout << deviceName << " is now ON.\n";
    }

    virtual void turnOff() {
        status = false;
        std::cout << deviceName << " is now OFF.\n";
    }

    virtual void showDetails() const {
        std::cout << "Device ID: " << deviceID
                  << ", Name: " << deviceName
                  << ", Status: " << (status ? "ON" : "OFF") << '\n';
    }

    int getID() const { return deviceID; }

    virtual void adjustSettings() {
        std::cout << "No adjustable settings for this device.\n";
    }

    virtual ~Device() {}
};

// Light Device
class Light : public Device {
private:
    int brightness; // 0 to 100

public:
    Light(int id, const std::string& name, int brightness = 50)
        : Device(id, name), brightness(brightness) {}

    void adjustBrightness(int level) {
        brightness = std::clamp(level, 0, 100);
        std::cout << "Brightness set to " << brightness << "%.\n";
    }

    void adjustSettings() override {
        int level;
        std::cout << "Enter new brightness level (0-100): ";
        std::cin >> level;
        adjustBrightness(level);
    }

    void showDetails() const override {
        Device::showDetails();
        std::cout << "Brightness: " << brightness << "%\n";
    }
};

// Fan Device
class Fan : public Device {
private:
    int speed; // 0 to 3 (OFF, LOW, MEDIUM, HIGH)

public:
    Fan(int id, const std::string& name, int speed = 0)
        : Device(id, name), speed(speed) {}

    void adjustSpeed(int level) {
        speed = std::clamp(level, 0, 3);
        std::cout << "Speed set to " << speed << " (0=OFF, 1=LOW, 2=MEDIUM, 3=HIGH).\n";
    }

    void adjustSettings() override {
        int level;
        std::cout << "Enter new speed level (0-3): ";
        std::cin >> level;
        adjustSpeed(level);
    }

    void showDetails() const override {
        Device::showDetails();
        std::cout << "Speed: " << speed << '\n';
    }
};

// Heater Device
class Heater : public Device {
private:
    int temperature; // in Celsius

public:
    Heater(int id, const std::string& name, int temp = 20)
        : Device(id, name), temperature(temp) {}

    void setTemperature(int temp) {
        temperature = temp;
        std::cout << "Temperature set to " << temperature << "°C.\n";
    }

    void adjustSettings() override {
        int temp;
        std::cout << "Enter new temperature in °C: ";
        std::cin >> temp;
        setTemperature(temp);
    }

    void showDetails() const override {
        Device::showDetails();
        std::cout << "Temperature: " << temperature << "°C\n";
    }
};

// Automatic Door with CCTV Camera
class AutomaticDoor : public Device {
private:
    bool isLocked;
    bool cameraStatus;

public:
    AutomaticDoor(int id, const std::string& name)
        : Device(id, name), isLocked(true), cameraStatus(false) {}

    void lockDoor() {
        isLocked = true;
        std::cout << deviceName << " is locked.\n";
    }

    void unlockDoor() {
        isLocked = false;
        std::cout << deviceName << " is unlocked.\n";
    }

    void turnOnCamera() {
        cameraStatus = true;
        std::cout << "CCTV Camera is now ON.\n";
    }

    void turnOffCamera() {
        cameraStatus = false;
        std::cout << "CCTV Camera is now OFF.\n";
    }

    void adjustSettings() override {
        int option;
        std::cout << "1. Lock Door\n2. Unlock Door\n3. Turn ON Camera\n4. Turn OFF Camera\nEnter your choice: ";
        std::cin >> option;

        switch (option) {
        case 1: lockDoor(); break;
        case 2: unlockDoor(); break;
        case 3: turnOnCamera(); break;
        case 4: turnOffCamera(); break;
        default: std::cout << "Invalid option!\n";
        }
    }

    void showDetails() const override {
        Device::showDetails();
        std::cout << "Door Status: " << (isLocked ? "Locked" : "Unlocked") << '\n';
        std::cout << "CCTV Status: " << (cameraStatus ? "ON" : "OFF") << '\n';
    }
};

// Smart Home Controller
class SmartHomeController {
private:
    std::vector<Device*> devices;

public:
    void addDevice(Device* device) {
        devices.push_back(device);
        std::cout << "Device added successfully.\n";
    }

    void removeDevice(int deviceID) {
        auto it = std::remove_if(devices.begin(), devices.end(), [deviceID](Device* d) {
            return d->getID() == deviceID;
        });

        if (it != devices.end()) {
            delete *it;
            devices.erase(it, devices.end());
            std::cout << "Device removed successfully.\n";
        } else {
            std::cout << "Device not found.\n";
        }
    }

    void showAllDevices() const {
        for (const auto& device : devices) {
            device->showDetails();
        }
    }

    void controlDevice(int deviceID, bool turnOn) {
        for (auto& device : devices) {
            if (device->getID() == deviceID) {
                if (turnOn) {
                    device->turnOn();
                } else {
                    device->turnOff();
                }
                return;
            }
        }
        std::cout << "Device not found.\n";
    }

    void adjustDeviceSettings(int deviceID) {
        for (auto& device : devices) {
            if (device->getID() == deviceID) {
                device->adjustSettings();
                return;
            }
        }
        std::cout << "Device not found.\n";
    }

    ~SmartHomeController() {
        for (auto device : devices) {
            delete device;
        }
    }
};

// Main Function
int main() {
    SmartHomeController controller;
    int choice;

    do {
        std::cout << "\n=== Smart Home Control System ===\n";
        std::cout << "1. Add Light\n";
        std::cout << "2. Add Fan\n";
        std::cout << "3. Add Heater\n";
        std::cout << "4. Add Automatic Door\n";
        std::cout << "5. Remove Device\n";
        std::cout << "6. Turn On Device\n";
        std::cout << "7. Turn Off Device\n";
        std::cout << "8. Adjust Device Settings\n";
        std::cout << "9. Show All Devices\n";
        std::cout << "10. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        int id;
        std::string name;
        switch (choice) {
        case 1:
            std::cout << "Enter Light ID and Name: ";
            std::cin >> id >> name;
            controller.addDevice(new Light(id, name));
            break;
        case 2:
            std::cout << "Enter Fan ID and Name: ";
            std::cin >> id >> name;
            controller.addDevice(new Fan(id, name));
            break;
        case 3:
            std::cout << "Enter Heater ID and Name: ";
            std::cin >> id >> name;
            controller.addDevice(new Heater(id, name));
            break;
        case 4:
            std::cout << "Enter Automatic Door ID and Name: ";
            std::cin >> id >> name;
            controller.addDevice(new AutomaticDoor(id, name));
            break;
        case 5:
            std::cout << "Enter Device ID to remove: ";
            std::cin >> id;
            controller.removeDevice(id);
            break;
        case 6:
            std::cout << "Enter Device ID to turn ON: ";
            std::cin >> id;
            controller.controlDevice(id, true);
            break;
        case 7:
            std::cout << "Enter Device ID to turn OFF: ";
            std::cin >> id;
            controller.controlDevice(id, false);
            break;
        case 8:
            std::cout << "Enter Device ID to adjust settings: ";
            std::cin >> id;
            controller.adjustDeviceSettings(id);
            break;
        case 9:
            controller.showAllDevices();
            break;
        case 10:
            std::cout << "Exiting the system...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 10);

    return 0;
}

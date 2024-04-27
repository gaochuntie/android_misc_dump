#include "boot_control_definition.h" 
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;


int main() {
    ifstream file("misc.img", ios::binary);
    if (!file.is_open()) {
        cout << "Failed to open misc.img file." << endl;
        return 1;
    }

    misc misc_data;
    file.read(reinterpret_cast<char*>(&misc_data), sizeof(misc_data));
    file.close();

    // Output contents
    cout << "Messages: " << misc_data.messages << endl;
    cout << "Bootloader Control:" << endl;
    cout << "Slot Suffix: " << misc_data.boot_control.slot_suffix << endl;
    cout << "Magic: " << misc_data.boot_control.magic << endl;
    cout << "Version: " << static_cast<int>(misc_data.boot_control.version) << endl;
    cout << "Number of Slots: " << static_cast<int>(misc_data.boot_control.nb_slot) << endl;
    cout << "Recovery Tries Remaining: " << static_cast<int>(misc_data.boot_control.recovery_tries_remaining) << endl;
    cout << "Merge Status: " << static_cast<int>(misc_data.boot_control.merge_status) << endl;
    cout << "CRC32 (little endian): " << misc_data.boot_control.crc32_le << endl;
    // Output slot info for each slot
    for (int i = 0; i < 4; ++i) {
        cout << "Slot " << i << " - Priority: " << static_cast<int>(misc_data.boot_control.slot_info[i].priority);
        cout << ", Tries Remaining: " << static_cast<int>(misc_data.boot_control.slot_info[i].tries_remaining);
        cout << ", Successful Boot: " << static_cast<int>(misc_data.boot_control.slot_info[i].successful_boot);
        cout << ", Verity Corrupted: " << static_cast<int>(misc_data.boot_control.slot_info[i].verity_corrupted);
        cout << ", Reserved: " << static_cast<int>(misc_data.boot_control.slot_info[i].reserved) << endl;
    }
    cout << "Update Channel: " << misc_data.update_channel << endl;
    
    return 0;
}
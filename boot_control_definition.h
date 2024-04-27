/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


/**
 * The A/B-specific bootloader message structure (4-KiB).
 *
 * We separate A/B boot control metadata from the regular bootloader
 * message struct and keep it here. Everything that's A/B-specific
 * stays after struct bootloader_message, which belongs to the vendor
 * space of /misc partition. Also, the A/B-specific contents should be
 * managed by the A/B-bootloader or boot control HAL.
 *
 * The slot_suffix field is used for A/B implementations where the
 * bootloader does not set the androidboot.ro.boot.slot_suffix kernel
 * commandline parameter. This is used by fs_mgr to mount /system and
 * other partitions with the slotselect flag set in fstab. A/B
 * implementations are free to use all 32 bytes and may store private
 * data past the first NUL-byte in this field. It is encouraged, but
 * not mandatory, to use 'struct bootloader_control' described below.
 *
 * The update_channel field is used to store the Omaha update channel
 * if update_engine is compiled with Omaha support.
 */


// 万佛朝宗 !!!!!!!!!!!!!!!!!
// misc对应最外层,就是misc.img
// 里面涉及的结构体都在该头文件内

// 大小4096
#include <cstdint>
#define BOOT_CTRL_MAGIC   0x42414342 /* Bootloader Control AB */
#define BOOT_CTRL_VERSION 1

// 大小 2
struct slot_metadata {
    // Slot priority with 15 meaning highest priority, 1 lowest
    // priority and 0 the slot is unbootable.
    uint8_t priority : 4;
    // Number of times left attempting to boot this slot.
    uint8_t tries_remaining : 3;
    // 1 if this slot has booted successfully, 0 otherwise.
    uint8_t successful_boot : 1;
    // 1 if this slot is corrupted from a dm-verity corruption, 0
    // otherwise.
    uint8_t verity_corrupted : 1;
    // Reserved for further use.
    uint8_t reserved : 7;
} __attribute__((packed));

/* Bootloader Control AB
 *
 * This struct can be used to manage A/B metadata. It is designed to
 * be put in the 'slot_suffix' field of the 'bootloader_message'
 * structure described above. It is encouraged to use the
 * 'bootloader_control' structure to store the A/B metadata, but not
 * mandatory.
 */

//大小 32
struct bootloader_control {
    // NUL terminated active slot suffix.
    char slot_suffix[4];
    // Bootloader Control AB magic number (see BOOT_CTRL_MAGIC).
    uint32_t magic;
    // Version of struct being used (see BOOT_CTRL_VERSION).
    uint8_t version;
    // Number of slots being managed.
    uint8_t nb_slot : 3;
    // Number of times left attempting to boot recovery.
    uint8_t recovery_tries_remaining : 3;
    // Status of any pending snapshot merge of dynamic partitions.
    uint8_t merge_status : 3;
    // Ensure 4-bytes alignment for slot_info field.
    uint8_t reserved0[1];
    // Per-slot information.  Up to 4 slots.
    struct slot_metadata slot_info[4];
    // Reserved for further use.
    uint8_t reserved1[8];
    // CRC32 of all 28 bytes preceding this field (little endian
    // format).
    uint32_t crc32_le;
} __attribute__((packed));

struct misc {
    //struct bootloader_message message; 源码没下完看不到这个结构体
    char messages[2048];
    struct bootloader_control boot_control;
    char update_channel[128];
    // Round up the entire struct to 4096-byte.
    char reserved[1888];
};




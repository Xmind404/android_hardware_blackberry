/*
 * SPDX-FileCopyrightText: 2019-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.blackberry"

#include "KeyDisabler.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

namespace {

constexpr const char* kControlPath = "/sys/devices/virtual/tp_device/tp_button/button_enable";

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

// "Enabled" means the capacitive navigation keys are disabled, i.e. the
// driver's button_enable node reads 0.
ndk::ScopedAStatus KeyDisabler::getEnabled(bool* _aidl_return) {
    std::string val;

    if (!android::base::ReadFileToString(kControlPath, &val)) {
        PLOG(ERROR) << "Failed to read " << kControlPath;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    // The driver reports its state as a hexadecimal string.
    *_aidl_return = android::base::Trim(val) == "0x0";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus KeyDisabler::setEnabled(bool enabled) {
    if (!android::base::WriteStringToFile(enabled ? "0" : "1", kControlPath)) {
        PLOG(ERROR) << "Failed to write " << kControlPath;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl

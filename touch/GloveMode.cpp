/*
 * SPDX-FileCopyrightText: 2021-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.blackberry"

#include "GloveMode.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

namespace {

constexpr const char* kGloveModePath = "/sys/devices/virtual/tp_device/tp_glove/glove_enable";

}  // anonymous namespace

namespace aidl {
namespace vendor {
namespace lineage {
namespace touch {

ndk::ScopedAStatus GloveMode::getEnabled(bool* _aidl_return) {
    std::string val;

    if (!android::base::ReadFileToString(kGloveModePath, &val)) {
        PLOG(ERROR) << "Failed to read " << kGloveModePath;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    // The driver reports its state as a hexadecimal string.
    *_aidl_return = android::base::Trim(val) == "0x1";
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus GloveMode::setEnabled(bool enabled) {
    if (!android::base::WriteStringToFile(enabled ? "1" : "0", kGloveModePath)) {
        PLOG(ERROR) << "Failed to write " << kGloveModePath;
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }

    return ndk::ScopedAStatus::ok();
}

}  // namespace touch
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl

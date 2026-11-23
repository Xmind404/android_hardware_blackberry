/*
 * SPDX-FileCopyrightText: 2021-2026 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.touch-service.blackberry"

#include "GloveMode.h"
#include "KeyDisabler.h"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

using aidl::vendor::lineage::touch::GloveMode;
using aidl::vendor::lineage::touch::KeyDisabler;

template <typename T>
static void registerService(const std::shared_ptr<T>& service) {
    const std::string instance = std::string(T::descriptor) + "/default";
    binder_status_t status =
            AServiceManager_addService(service->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK) << "Failed to add service " << instance << " " << status;
}

int main() {
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    registerService(ndk::SharedRefBase::make<GloveMode>());
    registerService(ndk::SharedRefBase::make<KeyDisabler>());

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}

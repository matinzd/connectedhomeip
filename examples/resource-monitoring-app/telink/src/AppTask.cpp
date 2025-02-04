/*
 *
 *    Copyright (c) 2023 Project CHIP Authors
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include "AppTask.h"

#include <StaticReplacementProductListManager.h>
#include <app/clusters/resource-monitoring-server/resource-monitoring-cluster-objects.h>
#include <app/server/Server.h>
#include <bitset>
#include <delegates/ActivatedCarbonFilterMonitoring.h>
#include <delegates/HepaFilterMonitoring.h>
#include <stdint.h>

LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace chip;
using namespace chip::app;
using namespace chip::app::Clusters;
using namespace chip::app::Clusters::ResourceMonitoring;

AppTask AppTask::sAppTask;

constexpr std::bitset<4> gHepaFilterFeatureMap{ static_cast<uint32_t>(Feature::kCondition) |
                                                static_cast<uint32_t>(Feature::kWarning) |
                                                static_cast<uint32_t>(Feature::kReplacementProductList) };
constexpr std::bitset<4> gActivatedCarbonFeatureMap{ static_cast<uint32_t>(Feature::kCondition) |
                                                     static_cast<uint32_t>(Feature::kWarning) |
                                                     static_cast<uint32_t>(Feature::kReplacementProductList) };

static HepaFilterMonitoringDelegate gHepaFilterDelegate;
static ResourceMonitoring::Instance gHepaFilterInstance(&gHepaFilterDelegate, 0x1, HepaFilterMonitoring::Id,
                                                        static_cast<uint32_t>(gHepaFilterFeatureMap.to_ulong()),
                                                        Clusters::ResourceMonitoring::DegradationDirectionEnum::kDown, true);

static ActivatedCarbonFilterMonitoringDelegate gActivatedCarbonFilterDelegate;
static ResourceMonitoring::Instance gActivatedCarbonFilterInstance(&gActivatedCarbonFilterDelegate, 0x1,
                                                                   ActivatedCarbonFilterMonitoring::Id,
                                                                   static_cast<uint32_t>(gActivatedCarbonFeatureMap.to_ulong()),
                                                                   Clusters::ResourceMonitoring::DegradationDirectionEnum::kDown,
                                                                   true);

CHIP_ERROR AppTask::Init(void)
{
    InitCommonParts();

    gHepaFilterInstance.Init();
    gActivatedCarbonFilterInstance.Init();

    CHIP_ERROR err = ConnectivityMgr().SetBLEDeviceName("TelinkResMonDevice");
    if (err != CHIP_NO_ERROR)
    {
        LOG_ERR("SetBLEDeviceName fail");
        return err;
    }

    return CHIP_NO_ERROR;
}

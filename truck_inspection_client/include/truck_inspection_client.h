#pragma once

/*
 * Copyright (C) 2019-2020 LEIDOS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */

#include <ros/ros.h>
#include <carma_utils/CARMAUtils.h>
#include <cav_msgs/MobilityOperation.h>
#include <cav_msgs/MobilityRequest.h>
#include "boost/format.hpp"

namespace truck_inspection_client
{

    class TruckInspectionClient
    {

    public:

        // general starting point of this node
        void run();

        const std::string INSPECTION_STRATEGY = "TruckInspection";

    private:

        // node handles
        std::shared_ptr<ros::CARMANodeHandle> nh_, pnh_;

        // publisher for generated Mobility Operation messages
        ros::Publisher  mo_pub_;

        // subscriber for Mobility Request messages
        ros::Subscriber request_sub_;

        // initialize this node
        void initialize();

        // callbacks for the subscriber
        void requestCallback(const cav_msgs::MobilityRequestConstPtr& msg);

        // truck info
        std::string vin_number_;
        std::string license_plate_;
        std::string carrier_name_;
        std::string carrier_id_;
        std::string weight_;
        std::string ads_software_version_;
        std::string date_of_last_state_inspection_;
        std::string date_of_last_ads_calibration_;
        int iss_score_;
        bool permit_required_;
        
    };

}
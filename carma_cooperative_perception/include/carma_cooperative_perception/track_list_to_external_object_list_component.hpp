// Copyright 2023 Leidos
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CARMA_COOPERATIVE_PERCEPTION__TRACK_LIST_TO_EXTERNAL_OBJECT_LIST_HPP_
#define CARMA_COOPERATIVE_PERCEPTION__TRACK_LIST_TO_EXTERNAL_OBJECT_LIST_HPP_

#include <proj.h>

#include <carma_cooperative_perception_interfaces/msg/track_list.hpp>
#include <carma_perception_msgs/msg/external_object_list.hpp>
#include <carma_ros2_utils/carma_lifecycle_node.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

#include <string>

#include "carma_cooperative_perception/msg_conversion.hpp"

namespace carma_cooperative_perception
{

auto transform_from_map_origin_to_local( // improve name scheme
  carma_cooperative_perception_interfaces::msg::TrackList track_list,
  const std::string &map_origin) -> carma_perception_msgs::msg::ExternalObjectList;


class TrackListToExternalObjectListNode : public carma_ros2_utils::CarmaLifecycleNode
{
  using input_msg_type = carma_cooperative_perception_interfaces::msg::TrackList;
  using input_msg_shared_pointer = typename input_msg_type::sjaredPtr;
  using output_msg_type = carma_perception_msgs::msg::ExternalObjectList;

  public:
    explicit TrackListToExternalObjectListNode(const rclcpp::NodeOptions &options);

  auto handle_on_configure(const rclcpp_lifecycle::State & /* previous_state */)
    -> carma_ros2_utils::CallbackReturn override;

  auto handle_on_cleanup(const rclcpp_lifecycle::State & /* previous_state */)
    -> carma_ros2_utils::CallbackReturn override;

  auto handle_on_shutdown(const rclcpp_lifecycle::State & /* previous_state */)
    -> carma_ros2_utils::CallbackReturn override;

  auto publish_as_external_object_list(const input_msg_type &msg) const -> void;


  private:
    rclcpp_lifecycle::LifecyclePublisher<output_msg_type>::SharedPtr publisher_{nullptr};
    rclcpp::Subscription<input_msg_type>::SharedPtr track_subscription_{nullptr};
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr georeference_subscription_{nullptr};
    std::string map_georeference_{""};
    // MotionModelMapping motion_model_mapping_{};
    OnSetParametersCallbackHandle::SharedPtr on_set_parameters_callback_{nullptr};
};

} // namespace carma_cooperative_perception

#endif // CARMA_COOPERATIVE_PERCEPTION__TRACK_LIST_TO_EXTERNAL_OBJECT_LIST_HPP_
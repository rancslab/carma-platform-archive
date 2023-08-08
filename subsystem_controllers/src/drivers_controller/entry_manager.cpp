/*
 * Copyright (C) 2023 LEIDOS.
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

#include "subsystem_controllers/drivers_controller/entry_manager.hpp"
#include <iostream>

namespace subsystem_controllers
{
    void EntryManager::update_entry(const Entry& entry)
    {
        entry_map_[entry.name_] = entry;
    }

    std::vector<Entry> EntryManager::get_entries() const
    {
        // returns the copy of the original data
        std::vector<Entry> entries;
        entries.reserve(entry_map_.size());

        for (const auto& e : entry_map_)
            entries.push_back(e.second);

        return entries;
    }

    std::vector<std::string> EntryManager::get_entry_names() const
    {
        std::vector<std::string> names;
        names.reserve(entry_map_.size());

        for (const auto& e : entry_map_)
            names.push_back(e.second.name_);
        
        return names;
    }

    void EntryManager::delete_entry(const std::string& name)
    {
        if (entry_map_.find(name) != entry_map_.end())
            entry_map_.erase(name);
    }

    boost::optional<Entry> EntryManager::get_entry_by_name(const std::string&  name) const
    {
        if (entry_map_.find(name) != entry_map_.end())
            return entry_map_.at(name);


        // use boost::optional because requested entry might not exist
        return boost::none;
    }

    bool EntryManager::is_entry_required(const std::string& name) const
    {
        for(auto i = required_entries_.begin(); i < required_entries_.end(); ++i)
        {
            if(i->compare(name) == 0)
            {
                return true;
            }
        }
        return false;
    }


    int EntryManager::is_lidar_gps_entry_required(const std::string& name) const
    {
        
        for(int i=0;i<lidar_gps_entries_.size();i++)
        {
            if(lidar_gps_entries_[i].compare(name) == 0)
            {
                return i;
            }

        }

        return -1;
    }

    int EntryManager::is_camera_entry_required(const std::string& name) const
    {
        for(int i = 0;i < camera_entries_.size(); i++)
        {
            if(camera_entries_[i].compare(name) == 0)
            {
                return i;
            }
        }
        
        // default like above?
        return -1;
    }


}
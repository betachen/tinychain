/**
 * Copyright (c) 2019-2020 CHENHAO.
 *
 * This file is part of tinychain.
 *
 * tinychain is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
#pragma once

#include <tinychain/elements/tinychain.hpp>
#include <tinychain/node/node.hpp>
#include <tinychain/utils/SimpleWeb.hpp>

namespace tinychain
{

class Rest : public RestServer
{
public:
    Rest(size_t cores) noexcept
    :RestServer{cores}
    {
    }

    Rest(const Rest&) noexcept = delete;
    Rest(Rest&&) noexcept = delete;

    Rest& operator=(Rest&&) noexcept = delete;
    Rest& operator=(const Rest&) noexcept = delete;

    ~Rest() override;

    void installHandlers() override;

    void print(){ std::cout<<"class network"<<std::endl; }
    void test();

    void getnewkey(RestResponsePtr rep, RestRequestPtr req);
    void listkeys(RestResponsePtr rep, RestRequestPtr req);
    void getheight(RestResponsePtr rep, RestRequestPtr req);
    void getlastblock(RestResponsePtr rep, RestRequestPtr req);
    void getbalance(RestResponsePtr rep, RestRequestPtr req);
    void startmining(RestResponsePtr rep, RestRequestPtr req);
    void stopmining(RestResponsePtr rep, RestRequestPtr req);
    void send(RestResponsePtr rep, RestRequestPtr req);

private:
    Node node_;
};

}// tinychain

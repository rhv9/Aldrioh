#pragma once

struct Upgrade
{
	std::string_view name;
	std::string_view description;
};

struct BaseStatUpgrade : public Upgrade
{

};

struct ShipModuleUpgrade : public Upgrade
{

};
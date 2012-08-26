/*
Warrior of Dreamworld, 2D Massivly Mutiplayer Online Role-playing Game
Copyright (C) 2012  Mislav Blazevic

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string ToString(int n)
{
    std::ostringstream out;
    out << n;
    return out.str();
}

int main()
{
    int MapID;
    std::cout << "Map ID: ";
    std::cin >> MapID;

    std::string fileName = "map" + ToString(MapID) + ".map";
    std::ofstream File(fileName);

    if (File)
    {
        float SizeX, SizeY;
        std::cout << "Size (x, y): ";
        std::cin >> SizeX >> SizeY;
        File << SizeX << " " << SizeY;

        std::string fileNameTileset;
        std::cout << "Tileset Filename: ";
        std::cin >> fileNameTileset;
        File << " " << fileNameTileset << " ";

        for (float y = 0 ; y < SizeY ; ++y)
            for (float x = 0 ; x < SizeX ; ++x)
                File << y << " " << x << " 1 15 ";
    }
}

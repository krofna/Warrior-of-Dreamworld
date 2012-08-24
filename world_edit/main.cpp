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
#include <sstream>
#include <boost/archive/binary_oarchive.hpp>

template<typename T>
std::string ToString(T const& Number)
{
    std::ostringstream out;
    out << Number;
    return out.str();
}

int main()
{
    std::cout << "Map ID: ";
    int mapID;
    std::cin >> mapID;

    std::string fileName = "map" + ToString(mapID) + ".map";
    std::ofstream File(fileName);
    if (File)
    {
        boost::archive::binary_oarchive out(File);

        int sizeY, sizeX;
        std::cout << "Size: ";
        std::cin >> sizeY >> sizeX;

        std::string fileNameTileset;
        std::cout << "Tileset Filename: ";
        std::cin >> fileNameTileset;

        out << fileNameTileset;

        for (int y = 0 ; y < sizeY ; ++y)
            for (int x = 0 ; x < sizeX ; ++x)
                File << x << " " << y << " 1 15";
    }
    else
        std::cerr << "Error when opening: " << fileName << std::endl;
}

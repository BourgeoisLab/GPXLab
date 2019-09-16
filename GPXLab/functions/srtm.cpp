/****************************************************************************
 *   Copyright (c) 2014 - 2015 Frederic Bourgeois <bourgeoislab@gmail.com>  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with This program. If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

#include <fstream>
#include <iomanip>
#include <sstream>
#include <math.h>
#include "srtm.h"

SRTM::SRTM(SRTMModel model) :
    directory(),
    tileSize((int)model),
    iLatitude(-1),
    iLongitude(-1),
    failed(false)
{
    lookup = new short[tileSize * tileSize];
}

SRTM::~SRTM()
{
    delete lookup;
}

string SRTM::getDirectory() const
{
    return directory;
}

void SRTM::setDirectory(const string& directory)
{
    this->directory = directory;
}

string SRTM::getFileName() const
{
    stringstream ss;
    ss  << setfill('0')
        << (iLatitude  >= 0 ? 'N' : 'S')
        << setw(2) << abs(iLatitude) << setw(0)
        << (iLongitude >= 0 ? 'E' : 'W')
        << setw(3) << abs(iLongitude) << setw(0)
        << ".hgt";
    return ss.str();
}

bool SRTM::isLittleEndian()
{
    int num = 1;
    if (*(char *)&num == 1)
        return true;
    else
        return false;
}

bool SRTM::parseHGT()
{
    ifstream file;

    // open HGT file
    file.open(directory + "/" + getFileName().c_str());
    if (!file)
    {
        failed = true;
        return false;
    }

    // read lookup table from file
    file.read((char*)lookup, tileSize * tileSize * sizeof(short));

    // close file
    file.close();

    // check endianness
    if (isLittleEndian())
    {
        unsigned long i, max = tileSize * tileSize;
        short *pHeight = lookup;
        for (i = 0; i < max; i++)
        {
            *pHeight = (((*pHeight & 0xFF) << 8) | ((*pHeight & 0xFF00) >> 8));
            ++pHeight;
        }
    }

    failed = false;
    return true;
}

short SRTM::bilinearInterpolate(short *inAltitudes, double inX, double inY)
{
    double alpha = inX - (int)inX;
    double beta = 1 - (inY - (int)inY);
    double alt = (1 - alpha)*(1 - beta)*inAltitudes[0] + alpha*(1 - beta)*inAltitudes[1]
               + (1 - alpha)*beta*inAltitudes[2] + alpha*beta*inAltitudes[3];
    return (short)alt;
}

short SRTM::averageNonVoid(short *inAltitudes)
{
    double totalAltitude = 0.0;
    int numAlts = 0;
    for (int i = 0; i < 4; i++)
    {
        if (inAltitudes[i] != SRTM::INVALID_DATA)
        {
            totalAltitude += inAltitudes[i];
            numAlts++;
        }
    }
    if (numAlts < 1)
        return SRTM::INVALID_DATA;
    return (short)(totalAltitude / numAlts);
}

short* SRTM::fixVoid(short *inAltitudes)
{
    for (int i = 0; i < 4; i++)
    {
        if (inAltitudes[i] == SRTM::INVALID_DATA)
            inAltitudes[i] = (int)round(averageNonVoid(inAltitudes));
    }
    return inAltitudes;
}

bool SRTM::getAltitude(double latitude, double longitude, short &altitude)
{
    int _iLatitude  = (int)floor(latitude);
    int _iLongitude = (int)floor(longitude);

    // check if need to parse new map file
    if (_iLatitude != iLatitude || _iLongitude != iLongitude || failed)
    {
        iLatitude  = _iLatitude;
        iLongitude = _iLongitude;
        if (parseHGT() == false)
            return false;
    }

    // get only fraction part
    latitude  -= iLatitude;
    longitude -= iLongitude;

    // get index of first point
    double x = longitude * tileSize - 1;
    double y = tileSize - latitude * tileSize - 1;
    int idx = ((int)y) * tileSize + (int)x;

    // get four point from the lookup table
    short fouralts[4] = {lookup[idx],
                         lookup[idx + 1],
                         lookup[idx - tileSize],
                         lookup[idx - tileSize -1]};

    // number of invalid values
    int numVoids = (fouralts[0] == SRTM::INVALID_DATA ? 1 : 0)
                 + (fouralts[1] == SRTM::INVALID_DATA ? 1 : 0)
                 + (fouralts[2] == SRTM::INVALID_DATA ? 1 : 0)
                 + (fouralts[3] == SRTM::INVALID_DATA ? 1 : 0);

    // choose interpolation/average strategy depending on number of invalid values
    switch (numVoids)
    {
    case 0:
        altitude = bilinearInterpolate(fouralts, x, y);
        break;
    case 1:
        altitude = bilinearInterpolate(fixVoid(fouralts), x, y);
        break;
    case 2:
    case 3:
        altitude = averageNonVoid(fouralts);
        break;
    default:
        altitude = INVALID_DATA;
        break;
    }
    return true;
}

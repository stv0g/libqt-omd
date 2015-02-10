/** Oi Helper functions
 *
 * @copyright	2015 Steffen Vogel
 * @license     http://www.gnu.org/licenses/gpl.txt GNU Public License
 * @author      Steffen Vogel <post@steffenvogel.de>
 * @link        https://dev.0l.de/wiki/projects/omd/start
 * @package     libqt-omd
 */
/*
 * This file is part of libqt-omd
 *
 * libqt-omd is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * libqt-omd is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libqt-omd. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef HELPERS_H
#define HELPERS_H

#include <QMap>
#include <QList>

namespace Oi {

template<typename K, typename V>
QMap<K, V> pairsToMap(const QList<QPair<K, V>> &list)
{
    QMap<K, V> map;

    for (QPair<K, V> pair : list)
        map[pair.first] = pair.second;

    return map;
}

template<typename K, typename V>
QList<QPair<K, V>> mapToPairs(const QMap<K, V> &map)
{
    QList<QPair<K, V>> list;

    for (K key : map.keys())
        list.append(qMakePair(key, map[key]));

    return list;
}

}


#endif // HELPERS_H

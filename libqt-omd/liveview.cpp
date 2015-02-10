/** Oi LiveView Class
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

#include "liveview.h"

using namespace Oi;

LiveView::LiveView(Camera *c) :
    mCam(c)
{ }

/* Copyright (C) 2026 Roman Frołow
 * This file is part of M2-Planet.
 *
 * M2-Planet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * M2-Planet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with M2-Planet.  If not, see <http://www.gnu.org/licenses/>.
 */

/* a[i]++ on an array of pointers steps by the pointee. */
static char buf[4];
static char *a[2];
int x[2];
int main(void)
{
	a[0] = buf;
	a[1] = buf;
	a[1]++;
	if(a[1] != buf + 1) return 1;
	if(a[0] != buf) return 2;
	a[1]--;
	if(a[1] != buf) return 3;
	x[1] = 7;
	x[1]++;
	if(x[1] != 8) return 4;
	if(x[0] != 0) return 5;
	return 0;
}

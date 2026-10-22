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

/* Compound assignment on an array element: scalar arrays, an array of
 * pointers (scaled by the pointee) and a member array. The neighbouring
 * elements must not change. */
static char buf[8];
static char *a[3];
int x[3];
struct S { int pad; int v[2]; };
struct S s;

int main(void)
{
	x[1] = 5;
	x[1] += 3;
	if(x[1] != 8) return 1;
	x[1] -= 10;
	if(x[1] != -2) return 2;
	if(x[0] != 0) return 3;
	if(x[2] != 0) return 4;

	a[1] = buf;
	a[1] += 2;
	if(a[1] != buf + 2) return 5;
	if(a[0] != 0) return 6;
	if(a[2] != 0) return 7;

	s.v[1] = 4;
	s.v[1] += 1;
	if(s.v[1] != 5) return 8;
	if(s.v[0] != 0) return 9;
	if(s.pad != 0) return 10;
	return 0;
}

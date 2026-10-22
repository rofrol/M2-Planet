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

/* a function-local static array of pointers. */
static char c0;
static char c1;
static int f(void)
{
	static char *w[2];
	w[0] = &c0;
	w[1] = &c1;
	if(w[0] != &c0) return 1;
	if(w[1] != &c1) return 2;
	if((long)&w[1] - (long)&w[0] != sizeof(char*)) return 3;
	return 0;
}
int main(void)
{
	return f();
}

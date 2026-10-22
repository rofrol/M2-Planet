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

/* sizeof(p->m) and sizeof(s.m). */
struct S { int m; long n; };
struct S s;
int main(void)
{
	struct S *p;
	p = &s;
	if(sizeof(p->m) != sizeof(int)) return 1;
	if(sizeof(p->n) != sizeof(long)) return 2;
	if(sizeof(s.n) != sizeof(long)) return 3;
	return 0;
}

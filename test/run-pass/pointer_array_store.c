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

/* A store into a static array of pointers: `rows[2] = &outs[1];`. The store
 * must be a pointer store (8 bytes on LP64); with the element size taken
 * from the pointee the compiler rejects it with "Got unsupported size 40
 * when storing number in register" (40 is struct outer in M2-Planet's padded
 * layout). The read-back and the untouched slots pin the whole table. */

struct outer {
	int a;
	int b;
};

static struct outer outs[4];
static struct outer *rows[4];
static int canary = 0x1234;

int main(void)
{
	struct outer *p;
	int i;

	outs[1].a = 42;
	for (i = 0; i < 4; i = i + 1) {
		rows[i] = 0;
	}
	rows[2] = &outs[1];
	p = rows[2];
	if (p != &outs[1]) return 1;
	if (p->a != 42) return 2;
	if (rows[0] != 0) return 3;
	if (rows[1] != 0) return 4;
	if (rows[3] != 0) return 5;
	if (canary != 0x1234) return 6;
	return 0;
}

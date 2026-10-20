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

/* A static array of pointers read to its zero end and indexed. The elements
 * must be laid out and read as pointers (8 bytes on LP64), not by the size of
 * the pointee: with the wrong element size the indexed load reads past the
 * table and the run segfaults on the first dereference (measured 2026-10-20
 * in a language project's chain-built binary, whose lexer scans exactly such
 * a table of keyword strings). Every nonzero exit names the check that
 * failed; the per-slot lengths name a wrong slot instead of crashing on it. */

static const char *const words[] = { "package", "import", "return", 0 };
static const char *sized[3] = { "ab", "cd", 0 };
static int canary = 0x1234;

static int len_of(const char *s)
{
	int n = 0;
	while (s[n] != 0) n = n + 1;
	return n;
}

int main(void)
{
	int i;
	int n;

	n = 0;
	for (i = 0; words[i] != 0; i++) {
		n = n + 1;
	}
	if (n != 3) return 1;
	if (len_of(words[0]) != 7) return 2;
	if (len_of(words[1]) != 6) return 3;
	if (len_of(words[2]) != 6) return 4;
	if (sized[0] == 0) return 5;
	if (len_of(sized[0]) != 2) return 6;
	if (len_of(sized[1]) != 2) return 7;
	if (sized[2] != 0) return 8;
	if (canary != 0x1234) return 9;
	return 0;
}

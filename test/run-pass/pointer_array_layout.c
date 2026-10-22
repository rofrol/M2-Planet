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

/* Diagnostic pin: pointer-array composition — T **a[N] with a[i][j], the
 * address of an element vs pointer arithmetic on the array name, and a
 * pointer array passed to a function that indexes the decayed parameter.
 * Expected to FAIL on the unfixed compiler (the element type becomes the
 * pointee, so scaling and loads are wrong). */

/* A helper function changes this file's layout; the composition shapes must
 * behave identically with and without it (measured 2026-10-21: without the
 * fix the failing check MOVES with the layout). It uses no libc: the run-pass
 * harness links libc-core only, which has no write(). */
static long sink;

static void note(const char *label, long x, long y)
{
    int n = 0;
    while (label[n] != 0) n = n + 1;
    sink = sink + n + (x ^ y);
}

static char c0;
static char c1;
static char *words[2];
static char **grid[2];

static char **first(char **a[])
{
    return a[0];
}

int main(void)
{
    long l1;
    long l2;

    words[0] = &c0;
    words[1] = &c1;
    grid[0] = words;
    grid[1] = words;

    if (sizeof(words) != 2 * sizeof(words[0])) return 8;
    if (grid[0][1] != &c1) return 1;
    if (grid[1][0] != &c0) return 2;
    if (first(grid)[1] != &c1) return 3;
    if (first(grid) != words) return 4;

    {
        char *local[2];
        char **pl;
        local[0] = &c0;
        local[1] = &c1;
        if (local[0] != &c0) return 9;
        if (local[1] != &c1) return 10;
        pl = local;
        if (pl[1] != &c1) return 11;
    }

    l1 = (long)&words[1];
    l2 = (long)(words + 1);
    note("check5", l1, l2);
    if (l1 != l2) return 5;
    if (*(words + 1) != &c1) return 6;
    if (&words[0] != words) return 7;

    return 0;
}

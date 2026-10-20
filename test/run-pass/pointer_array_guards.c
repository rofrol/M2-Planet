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

/* Diagnostic guards: shapes that WORK on the unfixed compiler and must not
 * regress when arrayness propagation is fixed (review round 20261020-160610
 * caught candidates breaking exactly these): scalar and struct arrays must
 * keep their element sizes (4 and the struct's, never 8), plain scalar
 * pointers keep scaling by their pointee, `*p[i] = v` stores through the
 * dereferenced element, and `a->p` after an array base is a normal member
 * access. Expected to PASS on the unfixed compiler. */

struct outer {
    int a;
    int b;
};

static int xs[4];
static struct outer rs[4];
static struct outer arr[2];
static int cells[4];
static int **pp;

int main(void)
{
    struct outer *a;
    int *p;
    int i;

    for (i = 0; i < 4; i = i + 1) {
        xs[i] = i + 1;
    }
    rs[2].a = 42;
    arr[1].a = 7;
    a = arr;
    p = cells;
    pp = &p;

    if (xs[2] != 3) return 1;
    if (rs[2].a != 42) return 2;
    if (rs[0].a != 0) return 3;
    if (a->a != 0) return 4;
    if (a[1].a != 7) return 5;
    if (p[0] != 0) return 6;

    *pp[0] = 9;
    if (cells[0] != 9) return 7;
    if (cells[1] != 0) return 8;

    return 0;
}

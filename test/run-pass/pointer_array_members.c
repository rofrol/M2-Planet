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

/* Diagnostic pin: arrays of pointers as struct/union MEMBERS (the face the
 * root cause predicts stays broken: fields have no symbol token). The review
 * round 20261020-160610-91c1 named the one-element shape specifically
 * (candidate-2's size heuristic misdetected it). Expected to FAIL on the
 * unfixed compiler (wrong element size on load/store). */

struct table {
    char *p[2];
    char *one[1];
    int n;
};

static struct table t;
static struct table *tp;
static char aa[2];
static char bb[2];
static int canary = 0x1234;

int main(void)
{
    t.p[0] = aa;
    t.p[1] = bb;
    t.one[0] = aa;
    t.n = 5;
    tp = &t;
    if (t.p[0] != aa) return 1;
    if (t.p[1] != bb) return 2;
    if (tp->p[0] != aa) return 3;
    if (tp->p[1] != bb) return 4;
    if (t.one[0] != aa) return 5;
    if (tp->one[0] != aa) return 6;
    if (t.n != 5) return 7;
    if (canary != 0x1234) return 8;
    return 0;
}

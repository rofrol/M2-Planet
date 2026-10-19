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

/* The address of a member through a subscripted pointer: `&p[i].member`.
 * The postfix chain under `&` is `[i]` then `.member`, and parsing the index
 * resets the address-of flag, so the member's handler used to load the member
 * by value where its address belongs: a struct member failed to compile with
 * "Got unsupported size N when trying to load value", and a scalar member
 * silently produced the value instead of its address. Any nonzero exit is a
 * failure. */

struct inner {
	int a;
	int b;
	int c;
	int d;
};

struct outer {
	struct inner in; /* a struct member: rejected before the fix */
	int tag;         /* a scalar member: silently wrong before the fix */
};

static struct outer outs[4];
static int outs_canary = 0x1122;

static struct inner *inner_addr(struct outer *p, int i)
{
	return &p[i].in;
}

static int *tag_addr(struct outer *p, int i)
{
	return &p[i].tag;
}

int main(void)
{
	struct inner *pi;
	int *pt;

	outs[0].in.a = 10;
	outs[1].in.a = 11;
	outs[2].in.a = 12;
	outs[1].tag = 42;

	pi = inner_addr(outs, 1);
	if(pi == 0) return 1;
	if(pi->a != 11) return 2;
	if(pi != &outs[1].in) return 3;
	pi->a = 99;
	if(outs[0].in.a != 10) return 4;
	if(outs[1].in.a != 99) return 5;
	if(outs[2].in.a != 12) return 6;

	pt = tag_addr(outs, 1);
	if(pt == 0) return 7;
	if(*pt != 42) return 8;
	if(pt != &outs[1].tag) return 9;
	*pt = 43;
	if(outs[1].tag != 43) return 10;
	if(outs[0].tag != 0) return 11;
	if(outs_canary != 0x1122) return 12;

	return 0;
}

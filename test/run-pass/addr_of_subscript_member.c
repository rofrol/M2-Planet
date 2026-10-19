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
 * used to reset the address-of flag, so the member's handler loaded the
 * member by value where its address belongs: a struct member failed to
 * compile with "Got unsupported size N when trying to load value", and a
 * scalar member silently produced the value instead of its address.
 *
 * The checks after that carry the flag through the rest of the postfix
 * chain: an intermediate pointer under `&` (`&mm[i][j]`), a member of a
 * member (`&outs[i].in.b`), a write through the taken address, and an index
 * that itself takes an address. Every nonzero exit names the check that
 * failed. `&p[i]->x` (subscript, then arrow) is not covered: that chain has
 * a separate defect (the member lookup gets the pointer type, not the
 * struct), measured the same day. */

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
static int *one_cell;      /* mm in cell_addr: &mm[i][j] */
static int grid_cells[16]; /* one dimension: M2-Planet has no T a[N][M] */
static int grid_canary = 0x5566;

static struct inner *inner_addr(struct outer *p, int i)
{
	return &p[i].in;
}

static int *tag_addr(struct outer *p, int i)
{
	return &p[i].tag;
}

/* An intermediate pointer under `&`: the subscript's element must be loaded
 * before the next subscript takes its address. The parameter is a pointer to
 * pointer on purpose: a global `int *cells[N]` takes us into a different
 * defect (array-of-pointer declarations), which is not what this case
 * measures. */
static int *cell_addr(int **mm, int i, int j)
{
	return &mm[i][j];
}

int main(void)
{
	struct inner *pi;
	int *pt;
	int *pj;

	outs[0].in.a = 10;
	outs[1].in.a = 11;
	outs[2].in.a = 12;
	outs[1].tag = 42;

	/* The reported shape: the address of a member of a subscripted
	 * pointer. */
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

	/* Chained subscripts through a pointer to pointer: the intermediate
	 * element is loaded, the last is taken in place. */
	one_cell = &grid_cells[4];
	pj = cell_addr(&one_cell, 0, 2);
	if(pj != &grid_cells[6]) return 13;
	*pj = 77;
	if(grid_cells[6] != 77) return 14;
	if(grid_cells[5] != 0) return 15;
	if(grid_cells[10] != 0) return 16;
	if(grid_canary != 0x5566) return 17;

	/* The address of a member of a member through the subscript. */
	pj = &outs[1].in.b;
	*pj = 88;
	if(outs[1].in.b != 88) return 18;
	if(outs[0].in.b != 0) return 19;
	if(*pj != 88) return 20;
	if(outs[1].in.a != 99) return 21;

	/* An index that itself takes an address: the inner `&` serves its own
	 * chain and the outer one must survive it. */
	pt = &outs[(&outs[0].tag != 0)].tag;
	if(pt != &outs[1].tag) return 22;

	return 0;
}

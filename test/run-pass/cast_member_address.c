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

/* A cast of a member address and of a postfix result: `return (long)&p->items;`
 * The cast used to parse only its primary operand and set the cast type before
 * the postfix chain ran, so the chain ran against the cast's type: `->` and `.`
 * failed their member lookup on the scalar ("ERROR in lookup_member
 * long->items does not exist"), and a subscript silently scaled by the cast
 * type's size instead of the operand's element size.
 *
 * The checks cover both member sizes (the 32-byte struct member and the 8-byte
 * pointer member of struct iset, the pair src/array.h and src/earley.c use),
 * the two cast targets (long, unsigned long), a cast of a plain pointer and of
 * the address of the pointer variable (controls: no postfix chain, they worked
 * before), the parenthesized operand form agreeing with the plain one, a cast
 * of a subscript result's value, a cast of a member address through a
 * subscripted pointer (the shape of the previous fix, composed with this one),
 * the dot form on a local struct, storing the cast and using it (cast back to
 * a pointer, write through it), and the offset between the two members. Every
 * nonzero exit names the check that failed. */

struct array {
	void *data;
	unsigned long len;
	unsigned long cap;
	unsigned long elem_size;
};

struct iset {
	struct array items; /* a 32-byte struct member */
	int *table;         /* an 8-byte pointer member */
	unsigned long cap;
};

static struct iset set;
static struct iset sets[2];
static int cells[4];
static int canary = 0x1234;

static long items_addr_long(struct iset *p)
{
	return (long)&p->items;
}

static long table_addr_long(struct iset *p)
{
	return (long)&p->table;
}

static unsigned long items_addr_ulong(struct iset *p)
{
	return (unsigned long)&p->items;
}

static unsigned long table_addr_ulong(struct iset *p)
{
	return (unsigned long)&p->table;
}

static long items_addr_long_paren(struct iset *p)
{
	return (long)(&p->items);
}

static long elem_items_addr_long(struct iset *group, int i)
{
	return (long)&group[i].items;
}

static long plain_ptr_long(struct iset *p)
{
	return (long)p;
}

static long ptr_slot_long(struct iset *p)
{
	return (long)&p;
}

int main(void)
{
	struct array *pa;
	long a;
	unsigned long u;

	set.table = cells;
	cells[2] = 42;

	/* The struct member's address cast to long: it is the struct itself. */
	a = items_addr_long(&set);
	if(a != (long)&set) return 1;

	/* The pointer member's address cast to long. */
	if(table_addr_long(&set) != (long)&set.table) return 2;

	/* The offset between the two members, taken by pointer subtraction
	 * through the casts: 32 is sizeof(struct array) on LP64. */
	u = table_addr_ulong(&set) - items_addr_ulong(&set);
	if(u != 32) return 3;

	/* The parenthesized operand form agrees with the plain one. */
	if(items_addr_long_paren(&set) != a) return 4;

	/* Controls: a cast of a plain pointer and of a variable's address take
	 * no postfix chain and worked before the fix. */
	if(plain_ptr_long(&set) != (long)&set) return 5;
	if(ptr_slot_long(&set) == 0) return 6;

	/* The dot form on a local struct agrees with the arrow form. */
	if((long)&set.items != a) return 7;
	if(table_addr_long(&set) == a) return 8;

	/* Store the cast and use it: cast the address back to a pointer and
	 * write through it. */
	pa = (struct array *)items_addr_long(&set);
	if(pa != &set.items) return 9;
	pa->len = 5;
	if(set.items.len != 5) return 10;
	if(set.items.cap != 0) return 11;

	/* A cast of a subscript result's value: the subscript scaled by the
	 * cast type's size (8) instead of the element size (4) and read past
	 * the array before the fix. */
	if((long)cells[2] != 42) return 12;
	if((long)cells[1] != 0) return 13;

	/* The previous fix's shape composed with this one: the address of a
	 * member of a subscripted pointer, cast. */
	a = elem_items_addr_long(sets, 1);
	if(a != items_addr_long(&sets[1])) return 14;
	if(elem_items_addr_long(sets, 0) != items_addr_long(&sets[0])) return 15;

	if(canary != 0x1234) return 16;
	if(cells[3] != 0) return 17;

	return 0;
}

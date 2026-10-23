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

/* `++` and `--` on an element a pointer subscript names must change that
 * element: the compiler used to skip loading the pointer before the subscript
 * under a prefix operator, so it indexed from the pointer variable's own
 * address and incremented a neighbour of it. The same test missed a prefix
 * operator ahead of a `*`, so `++*p` loaded *p and incremented the cell that
 * value addressed. The array subscript and the postfix forms are the
 * controls. Neighbouring elements are set so a store to the wrong element
 * shows. The pointers point at zeroed static storage, returned from functions
 * so their values are known only at run time, as with calloc; no header or
 * libc call is needed. The stack array and the globals around the global
 * pointer hold nonzero sentinels, so a store or load that strays into them
 * shows. Any nonzero exit is a failure. */
struct rule { int pad; int nt; };

/* `&rules[i]` rather than `rules + i`: M2-Planet adds a pointer and an int
 * without scaling by the pointee's size, which is not this case's subject. */
static struct rule* rule_at(struct rule* rules, int i)
{
	return &rules[i];
}

static int count_store[4];
static struct rule rule_store[2];
int guard_before;
int* global_counts;
int guard_after;

static int* count_cells(void)
{
	return count_store;
}

static struct rule* rule_cells(void)
{
	return rule_store;
}

int main(void)
{
	int* counts = count_cells();
	int array[4];
	struct rule* rules = rule_cells();
	int x;
	int i;

	array[0] = 101;
	array[1] = 102;
	array[2] = 103;
	array[3] = 104;
	guard_before = 201;
	guard_after = 202;

	counts[0] = 7;
	counts[2] = 9;
	x = ++counts[1];
	if(x != 1) return 1;
	if(counts[1] != 1) return 2;
	if(counts[0] != 7) return 3;
	if(counts[2] != 9) return 4;

	++counts[1];
	if(counts[1] != 2) return 5;
	--counts[1];
	if(counts[1] != 1) return 6;
	x = --counts[1];
	if(x != 0) return 7;
	if(counts[1] != 0) return 8;

	i = 3;
	x = ++counts[i];
	if(x != 1) return 9;
	if(counts[3] != 1) return 10;

	/* The real-world line: the subscript is a member of a call's result, and
	 * the result is stored through another call. */
	rules[1].nt = 2;
	rule_at(rules, 0)->pad = ++counts[rule_at(rules, 1)->nt];
	if(rules[0].pad != 10) return 11;
	if(counts[2] != 10) return 12;

	/* A global pointer. */
	global_counts = counts;
	x = ++global_counts[1];
	if(x != 1) return 13;
	if(counts[1] != 1) return 14;

	/* The postfix forms on the same element. */
	x = counts[1]++;
	if(x != 1) return 15;
	if(counts[1] != 2) return 16;
	x = counts[1]--;
	if(x != 2) return 17;
	if(counts[1] != 1) return 18;

	/* The array control. */
	array[0] = 7;
	array[1] = 0;
	array[2] = 9;
	x = ++array[1];
	if(x != 1) return 19;
	if(array[1] != 1) return 20;
	if(array[0] != 7) return 21;
	if(array[2] != 9) return 22;

	/* Through a dereference instead of a subscript. */
	counts[0] = 4;
	x = ++*counts;
	if(x != 5) return 23;
	if(counts[0] != 5) return 24;
	x = --*counts;
	if(x != 4) return 25;
	if(counts[0] != 4) return 26;
	if(counts[1] != 1) return 27;

	/* Through a dereference of the global pointer. */
	x = ++*global_counts;
	if(x != 5) return 28;
	if(counts[0] != 5) return 29;
	x = --*global_counts;
	if(x != 4) return 30;
	if(counts[0] != 4) return 31;
	if(counts[1] != 1) return 32;

	/* The sentinels nothing above should have touched. */
	if(array[3] != 104) return 33;
	if(guard_before != 201) return 34;
	if(guard_after != 202) return 35;
	if(global_counts != counts) return 36;

	return 0;
}

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


/* `++` and `--` on a global or static variable must work like on a local: the
 * compiler used to load the variable's value before the operator and then
 * dereference that value as an address, which segfaulted. Any nonzero exit is
 * a failure. */
static int static_counter = 0;
int global_counter = 0;

static int add_two(int value)
{
	return value + 2;
}

int main(void)
{
	int before;

	before = static_counter++;
	if(before != 0) return 1;
	if(static_counter != 1) return 2;
	before = ++static_counter;
	if(before != 2) return 3;
	if(static_counter != 2) return 4;
	before = static_counter--;
	if(before != 2) return 5;
	if(static_counter != 1) return 6;
	before = --static_counter;
	if(before != 0) return 7;
	if(static_counter != 0) return 8;

	global_counter++;
	global_counter++;
	if(global_counter != 2) return 9;
	--global_counter;
	if(global_counter != 1) return 10;

	/* A global counter in a loop, which is how the compiler's own grammar
	 * table is filled. */
	global_counter = 0;
	while(global_counter < 3)
		global_counter++;
	if(global_counter != 3) return 11;

	/* The incremented value is usable in an expression, not just as a
	 * statement. */
	global_counter = add_two(global_counter++);
	if(global_counter != 5) return 12;
	if(global_counter - 1 != 4) return 13;

	return 0;
}

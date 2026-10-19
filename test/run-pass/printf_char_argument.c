/* Copyright (C) 2026 Roman Frolow
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

/* An argument after a `%c`. `__va_arg` advanced the argument pointer by
 * sizeof(declared type), one byte for a char, while every argument occupies a
 * word-sized slot, so the next argument of the format was read from the wrong
 * place: `sprintf(buf, "[%c][%c]", 'A', 'B')` stopped at the 0 byte it read.
 * Any nonzero exit is a failure. */

#include <stdio.h>
#include <string.h>

int main(void)
{
	char buf[64];

	buf[0] = 0;
	sprintf(buf, "[%c][%c][%d]", 'A', 'B', 7);
	if(strcmp(buf, "[A][B][7]") != 0) return 1;

	buf[0] = 0;
	sprintf(buf, "%d%c%d%c", 1, 'x', 2, 'y');
	if(strcmp(buf, "1x2y") != 0) return 2;

	buf[0] = 0;
	sprintf(buf, "%c", 'Q');
	if(strcmp(buf, "Q") != 0) return 3;

	buf[0] = 0;
	sprintf(buf, "%s%c%s%c", "ab", 'C', "de", 'F');
	if(strcmp(buf, "abCdeF") != 0) return 4;

	/* Forty sub-word arguments: the argument area crosses a 0x100 boundary, so
	 * the pointer's low byte wraps and a one-byte store of the update would
	 * lose the carry. */
	buf[0] = 0;
	sprintf(buf,
	        "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c"
	        "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
	        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
	        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	        'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
	        '4', '5', '6', '7', '8', '9', '-', '+', '*', '/');
	if(strcmp(buf, "abcdefghijklmnopqrstuvwxyz0123456789-+*/") != 0) return 5;

	return 0;
}

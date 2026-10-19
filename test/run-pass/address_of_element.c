
/* `&array[i]` must address the element's storage. A global or static array is
 * a pointer to its storage, so `&arr[i]` used to index the pointer cell
 * instead: the address was wrong (and writing through it corrupted the data
 * after it), while `arr[i]` itself was right. The canaries catch both a wrong
 * base and a wrong element scale. Any nonzero exit is a failure. */
struct byte_item
{
	int a;
};

struct wide_item
{
	int a;
	int b;
	int c;
	int d;
	int e;
	int f;
	int g;
	int h;
	int i;
	int j;
};

static int ints[4];
static int ints_canary = 0x1234;
static struct byte_item bytes[4];
static int bytes_canary = 0x2345;
static struct wide_item wides[4];
static int wides_canary = 0x3456;
static char *label = "input";

int main(void)
{
	int *pi;
	struct byte_item *pb;
	struct wide_item *pw;

	ints[0] = 10;
	ints[1] = 11;
	ints[2] = 12;
	pi = &ints[1];
	if(pi == 0) return 1;
	if(*pi != 11) return 2;
	*pi = 99;
	if(ints[0] != 10) return 3;
	if(ints[1] != 99) return 4;
	if(ints[2] != 12) return 5;

	bytes[0].a = 20;
	bytes[1].a = 21;
	bytes[2].a = 22;
	pb = &bytes[1];
	if(pb == 0) return 6;
	if(pb->a != 21) return 7;
	pb->a = 98;
	if(bytes[0].a != 20) return 8;
	if(bytes[2].a != 22) return 9;

	wides[0].a = 30;
	wides[1].a = 31;
	wides[1].j = 41;
	wides[2].a = 32;
	pw = &wides[1];
	if(pw == 0) return 10;
	if(pw->a != 31) return 11;
	if(pw->j != 41) return 12;
	pw->j = 77;
	if(wides[0].a != 30) return 13;
	if(wides[2].a != 32) return 14;
	if(wides[1].j != 77) return 15;

	if(ints_canary != 0x1234) return 16;
	if(bytes_canary != 0x2345) return 17;
	if(wides_canary != 0x3456) return 18;
	if(label[0] != 'i') return 19;
	if(label[1] != 'n') return 20;
	if(label[4] != 't') return 21;

	return 0;
}

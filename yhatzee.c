#include <stdlib.h>
#include <stdio.h>

#define EMPTY -1

long **table;

long getTable(int amt, int dice){
	if(amt == 0 && dice == 0) return 1;
	if(amt <= 0) return 0;
	if(dice == 0) return 0;

	return table[amt][dice];
}

long dprolls(int amt, int dice, int sides){
	int a, d, s;

	for(a = 1; a <= amt; a++){
		for(d = 1; d <= dice; d++){
			int count = 0;
			for(s = 1; s <= sides; s++){
				count += getTable(a - s, d - 1);
			}
			table[a][d] = count;
		}
	}

	return getTable(amt, dice);
}

long mrolls(int amt, int dice, int sides){
	int count;
	int s;
	if(amt == 0 && dice == 0) return 1;
	if(amt <= 0) return 0;
	if(dice == 0) return 0;

	if(table[amt][dice] != EMPTY) return table[amt][dice];

	for(s = 1; s <= sides; s++){
		count += mrolls(amt - s, dice - 1, sides);
	}
	table[amt][dice] = count;
	return count;
}

long rolls(int amt, int dice, int sides){
	long count;
	int s;
	if(amt == 0 && dice == 0) return 1;
	if(amt < 0) return 0;
	if(dice == 0) return 0;

	for(s = 1; s <= sides; s++){
		count += rolls(amt - s, dice - 1, sides);
	}
	return count;
}

int main(void){
	int i,j;
	int dice = 8;
	int sides = 20;
	int amount = 50;

	table = malloc(sizeof(long *) * amount + 1);
	for(i = 0; i < amount + 1; i++){
		table[i] = malloc(sizeof(long) * (dice + 1));
		for(j = 0; j < (dice + 1); j++){
			table[i][j] = EMPTY;
		}
	}

	printf("Number of ways to roll %d with %d %d-sided dice: %ld\n", amount, dice, sides, dprolls(amount, dice, sides));
}
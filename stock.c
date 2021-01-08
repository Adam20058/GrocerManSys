#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "stock.h"

// CATEGORIES
char *cats[7] = {
		"produce",
		"bakery",
		"meat",
		"dairy",
		"baking",
		"house wares",
		"miscellaneous"};

//========================================================================
//Stock Reading Part
// Item Names
char itemNames[100][40];
// reading input line from stdin or file
void readInp(char buffer[], int readFromSTDIN, int bufferLength, FILE *fptr)
{
	if (readFromSTDIN)
	{
		fgets(buffer, bufferLength, stdin);
	}
	else if (fptr != NULL)
	{
		fgets(buffer, bufferLength, fptr);
	}
	buffer[strlen(buffer) - 1] = '\0';
}

// copy text from temp buffer to buffer
void copyTempCharInBuffer(char buffer[], char temp[], int start, int len)
{
	for (int i = start; i < start + len; i++)
	{
		buffer[i] = temp[i - start];
	}
}

// process the input line
void processInputLine(char buffer[], struct StockRecord storeStock[], int count, int readFromSTDIN, FILE *fptr)
{

	storeStock[count - 1].id = count; // id = count
	storeStock[count - 1].val = 0;		// initially no item is sold

	char temp[1024];
	int tempIndex = 0, currentPart = 0, size = strlen(buffer), lastcomma = 0;

	for (int buffIndex = 0; buffIndex < size; buffIndex++)
	{

		if (buffer[buffIndex] == ',' || (buffIndex == size - 1))
		{

			if (buffIndex == size - 1)
				temp[tempIndex++] = buffer[buffIndex];
			temp[tempIndex++] = '\0';
			tempIndex = 0;

			if (currentPart == 0)
			{
				int amount = atoi(temp);
				storeStock[count - 1].itemDet.amt = amount;
			}

			else if (currentPart == 1)
			{
				int category = atoi(temp);

				if (category > 7 || category < 1)
				{
					char temp2[1024];
					printf("Invalid Category - Enter a number between 1 and 7:");
					readInp(temp2, readFromSTDIN, sizeof(temp2), fptr);

					copyTempCharInBuffer(buffer, temp2, lastcomma + 1, strlen(temp2));

					buffIndex = lastcomma;
					tempIndex = 0;
					continue;
				}

				storeStock[count - 1].itemDet.cat = category;
			}

			else if (currentPart == 2)
			{
				char *priceredundant;
				storeStock[count - 1].itemDet.price = strtod(temp, &priceredundant);
			}

			else if (currentPart == 3)
			{
				int byWeight = atoi(temp);

				if (byWeight != 0 && byWeight != 1)
				{
					char temp2[1024];
					printf("Invalid soldByWeight - Enter a number between 0 and 1:");
					readInp(temp2, readFromSTDIN, sizeof(temp2), fptr);

					copyTempCharInBuffer(buffer, temp2, lastcomma + 1, strlen(temp2));

					buffIndex = lastcomma;
					tempIndex = 0;
					continue;
				}

				storeStock[count - 1].itemDet.byWeight = byWeight;
			}

			else if (currentPart == 4)
			{
				strcpy(itemNames[count - 1], temp);
			}
			lastcomma = buffIndex;
			currentPart++;
		}

		else
		{
			temp[tempIndex++] = buffer[buffIndex];
		}
	}
}

// reading input
int readStockItems(struct StockRecord storeStock[], int maxStockEntries, int readFromSTDIN)
{

	char buffer[2048];
	int count = 0; // no of stock items

	FILE *fptr = fopen("input.txt", "r");

	while (count < maxStockEntries)
	{
		//inputting the string
		readInp(buffer, readFromSTDIN, sizeof(buffer), fptr);

		// if amount is 0 stop inputting
		if (buffer[0] == '0')
		{
			break;
		}

		// incrementing count
		count++;

		//process the read string
		processInputLine(buffer, storeStock, count, readFromSTDIN, fptr);
	}

	if (fptr != NULL)
		fclose(fptr);
	return count;
}

//========================================================================
//Sale Reading Part

double updateInventory(double quantity, int id, struct StockRecord storeStock[])
{
	// rounding the quantity
	quantity = round(quantity);
	// available quantity
	int available = storeStock[id - 1].itemDet.amt;

	// if quantity required is greater then just sell the available quantity
	if (quantity > available)
		quantity = available;

	// update the inventory
	storeStock[id - 1].itemDet.amt -= quantity;

	// updating the total amount earned from this item in the inventory
	storeStock[id - 1].val += storeStock[id - 1].itemDet.price * quantity;

	return quantity;
}

void addItemToCart(int cartItemNo, int id, double quantity, struct SalesRecord saleItems[], struct StockRecord storeStock[])
{
	saleItems[cartItemNo].id = id;
	saleItems[cartItemNo].quantity = quantity;
	saleItems[cartItemNo].totalPrice = storeStock[id - 1].itemDet.price * quantity;
}

// reading sales input
int readSale(struct StockRecord storeStock[], int numStockItems, struct SalesRecord saleItems[])
{

	int cartItemNo = 0, id = numStockItems + 1;
	double quantity = 0;

	while (true)
	{

		id = numStockItems + 1;
		printf("Enter a product ID to purchase, and the quantity (0 to stop):");

		// inputting while id is not valid
		while (id > numStockItems && id != 0)
		{
			scanf("%d,", &id);
			if (id > numStockItems && id != 0)
			{
				scanf("%lf", &quantity);
				printf("Invalid Product - Enter a number between 0 and %d:", numStockItems);
				continue;
			}
		}

		// if id is 0 stop inputting
		if (id == 0)
			break;

		// inputting the quantity
		scanf("%lf", &quantity);
		// inputting quantity till it is a valid one
		while (quantity < 0 || quantity > 100)
		{
			int x;
			printf("Invalid quantity - Enter a number between 0.10 and 100.00:");
			scanf("%d,%lf", &x, &quantity);
		}

		// updating inventory
		quantity = updateInventory(quantity, id, storeStock);

		// storing itemNames in the sale cart
		addItemToCart(cartItemNo, id, quantity, saleItems, storeStock);

		cartItemNo++;
	}

	return cartItemNo;
}

//function to print a text in centre
void centreText(int len, char border, char *text)
{
	int borderLength = (len - strlen(text)) / 2;
	//print first half border
	for (int i = 0; i < borderLength; i++)
	{
		printf("%c", border);
	}
	//print the text
	printf("%s", text);
	//print next half of the border
	for (int i = 0; i < borderLength; i++)
	{
		printf("%c", border);
	}
}

//========================================================================
//Print StockReport Part
//printing the report of stock
void printStockReport(struct StockRecord storeStock[], int numStockItems)
{
	printf("ID\t\tProduct\t\tCategory\t    Price\tQuantity\n");
	for (int i = 0; i < numStockItems; i++)
	{
		struct StockRecord s = storeStock[i];
		printf("%-10d\t %-12s\t %-12s\t %8.2f %8d\t\n",
					 s.id, itemNames[i], cats[s.itemDet.cat - 1], s.itemDet.price, s.itemDet.amt);
	}
}

//========================================================================
//Print SalesReport Part

// prints the sales report
void printSalesReportBegin()
{
	printf("\n");
	centreText(70, '*', "Seneca Groceries");
	printf("\n");
	centreText(70, '=', "");
	printf("\n");
}
void printSalesReportEnd(double TP, double taxes, double amt)
{
	printf("Puchase Total\t%.2lf\n", TP);
	printf("Tax\t%.2lf\n", taxes);
	printf("Total\t%.2lf\n", amt);
	printf("Thank you for shopping at Seneca!\n\n");
}
double printSalesReport(struct StockRecord storeStock[], struct SalesRecord saleItems[], int numSaleItems)
{

	double TP = 0;
	double taxes = 0;

	printSalesReportBegin();

	for (int i = 0; i < numSaleItems; i++)
	{
		printf("\t%s\t%.2lf\t%.2lf\n",
					 itemNames[saleItems[i].id - 1], storeStock[saleItems[i].id - 1].itemDet.price, saleItems[i].totalPrice);

		TP += saleItems[i].totalPrice;

		// if category is 6 or 7 then 13% tax
		if (storeStock[saleItems[i].id - 1].itemDet.cat > 5)
			taxes += (0.13 * saleItems[i].totalPrice);
	}

	printSalesReportEnd(TP, taxes, TP + taxes);

	return TP;
}

//========================================================================
//TopSellers Part

void copy_SalesRecord(struct SalesRecord *from, struct SalesRecord *to)
{
	to->id = from->id;
	to->quantity = from->quantity;
	to->totalPrice = from->totalPrice;
}
//getting top sellers
void getTopSellers(struct StockRecord storeStock[], int numStockItems, struct SalesRecord topSellers[5], int count, int cat)
{

	// initializing all the ids in topSellers to 0
	for (int i = 0; i < count; i++)
	{
		topSellers[i].id = 0;
		topSellers[i].quantity = 0;
	}

	// Insertion Sort
	for (int i = 0; i < numStockItems; i++)
	{
		if (storeStock[i].itemDet.cat == cat + 1)
		{
			for (int j = 0; j < count; j++)
			{
				if ((topSellers[j].id == 0 && storeStock[i].val != 0) || (topSellers[j].totalPrice < storeStock[i].val && storeStock[i].val != 0))
				{
					for (int k = count - 1; k >= j + 1; k--)
					{
						copy_SalesRecord(&topSellers[k - 1], &topSellers[k]);
					}
					topSellers[j].id = storeStock[i].id;
					topSellers[j].quantity = storeStock[i].val / storeStock[i].itemDet.price;
					topSellers[j].totalPrice = storeStock[i].val;
					break;
				}
			}
		}
	}
}
void printTopSellerHead(int rows, int category)
{
	char temp[2], str[100] = " ";
	temp[0] = 48 + rows;
	temp[1] = '\0';

	strcat(str, "Top ");
	strcat(str, temp);
	strcat(str, " sellers in ");
	strcat(str, cats[category]);
	strcat(str, " ");

	centreText(50, '-', str);
	printf("\n");

	printf("Rank\tProduct\tSales\n");
}
void printTopSellerMid(int rank, char *name, double quantity)
{
	printf("%d\t%s\t%.2lf\n", rank, name, quantity);
}
// printing top sellers
void printTopSellers(struct StockRecord storeStock[], struct SalesRecord topSellers[], int count, int cat)
{
	printTopSellerHead(count, cat);

	for (int i = 0; i < count; i++)
	{
		printTopSellerMid(
				i + 1,
				topSellers[i].id == 0 ? "<none>" : itemNames[topSellers[i].id - 1],
				topSellers[i].quantity);
	}
}

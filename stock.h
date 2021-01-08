#ifndef STOCK_H
#define STOCKK_H

#define NUM_CATS 7            // number of different categories categories
#define MAX_ITEMS_IN_SALE 100 // maximum items in a sale

struct ItemDet
{
   int amt;      // total amount of product present in the inventory
   double price; // price of the product
   int cat;      // category of the product
   int byWeight; // 1 if sold by weight else 0 if sold by number
};

struct StockRecord
{
   struct ItemDet itemDet;
   int id;     // id of the product (starts from 1)
   double val; // total earning from the sale of current item
};

struct SalesRecord
{
   int id; // id of the item
   double quantity;
   // required quantity -> if greater than quantity in inventory then the present amount is only sold
   double totalPrice; // price * actual quantity sold
};

void centreText(int len, char border, char *text);

int readStockItems(struct StockRecord storeStock[], int maxStockEntries, int readFromSTDIN);

void printStockReport(struct StockRecord storeStock[], int numStockItems);

int readSale(struct StockRecord storeStock[], int numStockItems, struct SalesRecord saleItems[]);

double printSalesReport(struct StockRecord storeStock[], struct SalesRecord saleItems[], int numSaleItems);

void getTopSellers(struct StockRecord storeStock[], int numStockItems, struct SalesRecord topSellers[], int count, int cat);

void printTopSellers(struct StockRecord storeStock[], struct SalesRecord topSellers[], int count, int cat);

#endif
